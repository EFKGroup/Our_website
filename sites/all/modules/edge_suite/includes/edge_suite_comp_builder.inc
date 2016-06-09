<?php

/**
 * @file
 * The main class to extract, modify and import files from an edge archive
 *
 * The main workflow is:
 *
 * - Extract archive in tmp dir
 * - Find the working directory with the main JS Edge file (_edge.js) and get
 *   the project name and composition id from it
 * - Alter _edge.js file to call custom JS function at runtime which alters path
 *   prefixes in the DOM
 * - Alter _edgePreload.js file to call custom JS function at runtime which
 *   alters path prefixes in the DOM and the preloader object
 * - replace the general edge stage id (id of the main DIV container) with a
 *   project specific id. This is really important to run multiple compositions
 *   along each other on one page
 */


class EdgeCompositionBuilder {

  /**
   * Main archive file which contains the composition
   */
  protected $file = '';

  /**
   * Tmp directory that the archive will be extracted to
   */
  protected $tmpDirBase = '';

  /**
   * Tmp directory that contains the composition files, sub dir of $tmpDirBase
   */
  protected $tmpDir = '';

  /**
   * Path of the project
   */
  protected $destinationDir = '';

  /**
   * Path of the main edge file _edge.js
   */
  protected $mainEdgeJS = '';

  /**
   * Path of the preloader file _edgePreload.js
   */
  protected $preloaderFile = '';

  /**
   * Internal path of the poster image.
   */
  protected $posterImage = '';

  /**
   * External path to poster image, if it was extracted.
   */
  protected $posterImageExternal = '';

  /**
   * Name of the project
   */
  protected $projectName = '';

  /**
   * Composition Id
   */
  protected $stageClass = '';

  /**
   * Version of Edge that the composition uses.
   */
  protected $edgeVersion = '';

  /**
   * Dimensions of the composition.
   */
  protected $dimensions;


  /**
   * Init function to set directories and initial values.
   */
  protected function init() {
    global $user;
    $this->tmpDirBase = 'temporary://' . 'edge_suite/src_' . $user->uid . '_' . REQUEST_TIME;

    // Initialize with tmpDirBase and climb up dir structure after extraction.
    $this->tmpDir = $this->tmpDirBase;

    $this->dimensions = array(
      'width' => 0,
      'height' => 0,
      'min-width' => 0,
      'max-width' => 0,
      'min-height' => 0,
      'max-height' => 0,
    );
  }

  /**
   * Name of the project, after the composition has been processed.
   * @return string
   *   Project name
   */
  public function getProjectName() {
    return $this->projectName;
  }

  /**
   * Composition Id (class name) of the project.
   * @return string
   *   Composition Id (class name)
   */
  public function getCompositionClass() {
    return $this->stageClass;
  }

  /**
   * Return the edge version.
   * @return string
   *   Edge version as string, e.g. 1.5.0
   */
  public function getEdgeVersion() {
    return $this->edgeVersion;
  }

  /**
   * Return the dimensions and its limits of the composition stage.
   * @return array
   *   dimension values for width, height including  min and max values
   */
  public function getDimensions() {
    return $this->dimensions;
  }

  /**
   * Return full path to temporary poster image.
   * @return string
   *   Path to the poster image (including file wrapper), if it was extracted.
   */
  public function getPosterImageExternal() {
    return $this->posterImageExternal;
  }

  /**
   * Path to poster within the project directory.
   * @return string
   *   Path to poster image, if poster was set in OAM.
   */
  public function getPosterImageInternal() {
    return $this->posterImage;
  }

  /**
   * Extract the OAM temporarily and validate the composition.
   *
   * @param string $file_uri
   *   File uri of OAM
   * @param string $poster_dir
   *   Destination directory for the temporary copy of the poster.
   */
  public function validate($file_uri, $poster_dir = NULL) {
    $this->file = $file_uri;

    // Extract & validate the OAM and modify files.
    $this->process($poster_dir);

    // Delete the tmp archive directory with all remaining files.
    $this->cleanup();
  }

  /**
   * Extract and validate OAM file and build a usable modified composition.
   *
   * @param string $file_uri
   *   OAM uri
   * @param string $destination_dir
   *   Destination folder
   * @param string $library_base_dir
   *   Path to edge_includes directory, usually the edge_suite base directory
   * @param bool $replace_libraries
   *   Flag to replace libraries
   */
  public function build($file_uri, $destination_dir, $library_base_dir, $replace_libraries = FALSE) {
    $this->file = $file_uri;
    $this->destinationDir = $destination_dir;

    // Extract & validate the OAM and modify files.
    $this->process();

    // Crate destination folder.
    $this->createDestFolder();

    // Read in all the libraries and copy them to a shared edge library folder.
    $this->readLibs($library_base_dir, $replace_libraries);

    // Move all assets (and the _edgeAction.js)
    $this->moveEdgeFiles($this->tmpDir, $this->destinationDir);

    // Delete the tmp archive directory with all remaining files.
    $this->cleanup();
  }


  /**
   * Delete the tmp archive directory with all remaining files.
   */
  public function cleanup() {
    if (file_exists($this->tmpDirBase)) {
      file_unmanaged_delete_recursive($this->tmpDirBase);
    }
  }


  /**
   * Main function to process an edge composition.
   *
   * Extract, validate and modify files and structure. No files will be
   * persistently saved.
   *
   * @param string $poster_dir
   *   Destination directory for the temporary copy of the poster.
   *
   * @return bool
   *   true if build process was successful, exception thrown otherwise
   * @throws Exception
   */
  protected function process($poster_dir = NULL) {
    $this->init();

    // Extract the archive.
    $this->extract($this->file, $this->tmpDirBase);

    // Find the working directory, set _edge.js file, project name and comp id.
    $this->validateArchiveStructure();

    // Get the preloader file _edgePreload.js.
    $this->getPreloader();

    // Get poster image.
    $this->extractPoster($poster_dir);

    // Alter main JS edge file to so it calls custom JS from edge_drupal.js.
    $altered_main = $this->alterMainEdgeJs();

    // Alter preload JS file so it calls custom JS from edge_drupal.js.
    $altered_preloader = $this->alterPreloaderJs();

    if (!$altered_main && !$altered_preloader && empty($this->edgeVersion)) {
      $this->parseRuntimeVersion();
    }

    // Return build success.
    return TRUE;
  }


  /**
   * Sets the message followed by a list of file names.
   *
   * @param array $file_list
   *   List of file names
   * @param string $message
   *   Message to be set on top of the list (will be processed through t())
   * @param string $type
   *   Standard Drupal message type
   */
  protected function setFileMessages($file_list, $message, $type = 'status') {
    // Todo future: Messages need to be printed somewhere else.
    if (!empty($file_list)) {
      $files_list_string = t($message) . '</br>';
      foreach ($file_list as $file) {
        $files_list_string .= check_plain($file) . '</br>';
      }
    }
    // Todo: Print messages.
  }

  /**
   * Extracts the archive and sets the path to the working directory (tmpDir).
   *
   * @throws Exception
   */
  protected function extract($file, $destination) {
    // Check if file really exists.
    if (!file_exists($file)) {
      throw new Exception(t('Could not find archive: %file.', array('%file' => $file)));
    }

    // Check if zip lib is available.
    if (!class_exists('ZipArchive')) {
      throw new Exception(t('The server does not support zip extraction, class ZipArchive not found.', array('%file' => $file)));
    }

    // Copy OAM file.
    if (substr($file, -4) == '.oam') {
      $file_oam = $file;
      $file_zip = substr($file_oam, 0, count($file_oam) - 5) . '.zip';
      $success = file_unmanaged_copy($file_oam, $file_zip);
      if ($success) {
        $file_archive = $file_zip;
        $oam_source = TRUE;
      }
    }
    else {
      $oam_source = FALSE;
      $file_archive = $file;
    }

    // Get archiver.
    $archiver = archiver_get_archiver($file_archive);

    if (!$archiver) {
      // Clean up copied zip file.
      if ($oam_source) {
        file_unmanaged_delete($file_archive);
      }
      throw new Exception(t('Cannot extract %file, not a valid archive.', array('%file' => $file)));
    }

    // Delete old edge tmp dir if it exists.
    if (file_exists($destination)) {
      file_unmanaged_delete_recursive($destination);
    }

    // Extract archive.
    $archiver->extract($destination);

    // Close archive. Reported issues on win.
    $archiver->getArchive()->close();

    // Clean up copied zip file.
    if ($oam_source) {
      file_unmanaged_delete($file_archive);
    }
  }

  /**
   * Finds the main edge file _edge.js, sets the project name.
   *
   * @throws Exception
   */
  protected function validateArchiveStructure() {
    // Find all files that match the pattern.
    $main_edge_files = file_scan_directory($this->tmpDir, '/_edge\.js$/', array('recurse' => TRUE));
    if (count($main_edge_files) == 0) {
      throw new Exception(t('Aborting, no file found that matches the main edge JS filename pattern. This is not a valid composition archive.'));
    }
    else {
      // Todo: pop or shift? Avoid choosing edge.js in the published folder.
      $main_edge_file = array_pop($main_edge_files);
      $this->tmpDir = drupal_dirname($main_edge_file->uri);
      $this->mainEdgeJS = $main_edge_file->uri;
      $project_name_tmp = substr($main_edge_file->name, 0, strlen($main_edge_file->name) - strlen('_edge'));

      // Sanitize project name.
      if (preg_match('/^[a-zA-Z0-9_-]*$/', $project_name_tmp) == 1) {
        $this->projectName = $project_name_tmp;
      }
      else {
        throw new Exception(t('Aborting, the project name is not valid.'));
      }

      // Check if there were multiple main js files.
      if (count($main_edge_files) > 0) {
        $msg_dir = substr($this->tmpDir, strlen($this->tmpDirBase));
        drupal_set_message(t('Multiple edge.js files found. Everything outside @dir will be ignored.', array('@dir' => $msg_dir)));
      }
    }
  }

  /**
   * Finds the preloader file _edgePreload.js.
   *
   * @throws Exception
   */
  protected function getPreloader() {
    if (!empty($this->projectName)) {
      $js_file_tmp = $this->tmpDir . '/' . $this->projectName . '_edgePreload.js';
    }
    else {
      throw new Exception('Cannot read main JS edge file, project name not set.');
    }

    if (file_exists($js_file_tmp)) {
      $this->preloaderFile = $js_file_tmp;
    }
  }

  /**
   * Alters the main edge file.
   *
   * A calls to alterDomPaths() will be added to add path prefixes to the DOM
   * assets.
   *
   * @throws Exception
   */
  protected function alterMainEdgeJs() {
    $altered = FALSE;
    $file = $this->mainEdgeJS;
    $content = file_get_contents($file);

    // Parse the dimensions.
    $this->parseDimensions($content);

    // Extract the composition id.
    // The relevant JS structure (might be minified).
    // (function($, Edge, compId){....})(jQuery, AdobeEdge, "MyProject");.
    // (function($, Edge, compId){....})(AdobeEdge.$, AdobeEdge, "MyProject");
    $pattern = '/}\)\((?:AdobeEdge\.\$|jQuery),\s?AdobeEdge,\s?\"([a-zA-Z0-9_-]*)\"/';
    $matches = array();
    $found = preg_match($pattern, $content, $matches);
    if (!$found) {
      throw new Exception(t('Unable to parse stage name from main edge file'));
    }
    $this->stageClass = $matches[1];

    // Replace alterRegisterCompositionDefn() with custom call to alter
    // symbols and fonts. Older EA versions don't have the opts parameter.
    $register_pattern = '/Edge\.registerCompositionDefn\(compId,\s?symbols,\s?fonts,\s?resources(,\s?opts)?\);/';
    $register_matches = array();
    if (preg_match($register_pattern, $content, $register_matches)) {
      $altered = TRUE;
      $opts = 'null';
      if (isset($register_matches[1])) {
        $opts = 'opts';
      }
      $register_replace = 'AdobeEdge.alterRegisterCompositionDefn(compId,symbols,fonts,resources,' . $opts . ',Edge.registerCompositionDefn);';
      $register_pattern_found = 0;
      $content = preg_replace($register_pattern, $register_replace, $content, -1, $register_pattern_found);

      // Write modified file.
      $fh = fopen($file, "w");
      fwrite($fh, $content);
      fclose($fh);
    }
    return $altered;
  }

  /**
   * Parse runtime version form main .html file.
   */
  protected function parseRuntimeVersion() {
    $file = $this->tmpDir . '/' . $this->projectName . '.html';

    if (!file_exists($file)) {
      throw new Exception(t('Main .html file does not exist.'));
    }

    $content = file_get_contents($file);

    $runtime_pattern = '/animate\.adobe\.com\/runtime\/(\d\.\d\.\d)\//';
    $matches = array();
    if (preg_match($runtime_pattern, $content, $matches)) {
      $this->edgeVersion = $matches[1];
    }
  }

  /**
   * Parse dimensions from the content and set them in the dimension array.
   *
   * @param string $content
   *   Content of the main edge.js file.
   */
  protected function parseDimensions($content) {

    $xml_file = NULL;
    if (!empty($this->projectName)) {
      $xml_file = $this->tmpDir . '/' . $this->projectName . '_oam.xml';
      $xml = simplexml_load_file($xml_file);
      foreach ($xml->properties->children() as $p) {
        $key = (string) $p['name'];
        if ($key == 'default-width') {
          $this->dimensions['width-default'] = (string) $p['defaultValue'] . 'px';
        }
        if ($key == 'default-height') {
          $this->dimensions['height-default'] = (string) $p['defaultValue'] . 'px';
        }
      }
    }

    // Parse dimensions of the composition.
    // Find stage variable first.
    $stage_variable_match = array();
    $dimension_error_msg = '';
    if (preg_match('/(e\d*)=\'\$\{_Stage\}\'/', $content, $stage_variable_match)) {
      $stage_variable = $stage_variable_match[1];

      // Find property function calls for stage variable.
      $pattern_stage_style = '/;A1.A\(' . $stage_variable . '\)(?:\.[PT]\((?:(?:[a-z0-9%,"\-_\[\]])||(?:\'rgba\([\d,\.]+\)\'))+\))*;/i';
      $matches = array();
      if (preg_match($pattern_stage_style, $content, $matches)) {
        $properties = array();
        // Explode property values.
        if (preg_match_all('/P\(([a-z0-9,%"_]+)\)/i', $matches[0], $properties)) {
          foreach ($properties[1] as $property_string) {
            $property_values = explode(',', $property_string);
            // Full properties: P(h,280,_,_,p) or P(w,100,_,_,"%").
            if (count($property_values) == 5) {
              // Check if it's pixel or relative measure.
              $measure = '';
              if ($property_values[4] == '"%"') {
                $measure = '%';
              }
              else {
                if ($property_values[4] == 'p') {
                  $measure = 'px';
                }
                else {
                  continue;
                }
              }
            }
            // Compressed properties: P(xw,600).
            if (count($property_values) == 2) {
              $measure = 'px';
            }

            if (!empty($measure)) {
              switch ($property_values[0]) {
                case 'h':
                  $this->dimensions['height'] = $property_values[1] . $measure;
                  break;

                case 'w':
                  $this->dimensions['width'] = $property_values[1] . $measure;
                  break;
              }
            }
          }
        }
        else {
          $dimension_error_msg = t('Unable to explode stage properties.');
        }
      }
      else {
        $dimension_error_msg = t('Unable to spot stage property calls.');
      }
    }
    else {
      $dimension_error_msg = t('Unable to spot stage variable.');
    }

    if ($this->dimensions['height'] == 0 || $this->dimensions['width'] == 0) {
      if (isset($this->dimensions['width-default']) && isset($this->dimensions['height-default'])) {
        $this->dimensions['width'] = $this->dimensions['width-default'];
        $this->dimensions['height'] = $this->dimensions['height-default'];
        drupal_set_message(t('Auto detection of stage dimensions failed:') . ' ' . $dimension_error_msg);
        drupal_set_message(t('OAM stage pixel dimensions defaults detected, using those instead.'));
      }
      else {
        drupal_set_message(t('Auto detection of stage dimensions failed:') . ' ' . $dimension_error_msg, 'warning');
      }
    }
  }


  /**
   * Alters the edge preloader file by injecting custom function calls.
   *
   * A call to loadResources will be replaced by a call to the Edge-Drupal
   * bridge JS function alterPreloadPaths() which will add path prefixes to
   * all resource and the call the original loader.
   * Calls to alterDomPaths() will be added to add path prefixes to the DOM
   * assets as well
   */
  protected function alterPreloaderJs() {
    $altered = FALSE;

    if ($this->preloaderFile != NULL) {
      $file = $this->preloaderFile;
      $content = file_get_contents($file);

      // Search for the loadResource function call.
      $load_pattern = '/loadResources\(aLoader,\s?doDelayLoad\);/';

      if (preg_match($load_pattern, $content)) {
        $altered = TRUE;

        // Replace original call with call to custom function, alters preload
        // paths.
        $load_replace = 'AdobeEdge.alterPreloadPaths(compId, aLoader, doDelayLoad, loadResources);';
        $content = preg_replace($load_pattern, $load_replace, $content);

        // Search for the okToLaunchComposition function call.
        $launch_pattern = '/AdobeEdge\.okToLaunchComposition\(compId\)/';

        // Replace original call with call to custom function, alters jQuery.
        $launch_replace = 'AdobeEdge.alterOkToLaunchComposition(compId)';
        $content = preg_replace($launch_pattern, $launch_replace, $content);

        // Find the end of the main JS function and inject to function calls to
        // alter preContent and dlContent DOM, see JS function doc.
        // Expected structure (might be minified).
        // (function(compId){...})("MyProject").
        $dom_pattern = '/\}\)\(\s?\"' . $this->stageClass . '\"\);/';
        $dom_replace = 'AdobeEdge.alterDomPaths(preContent.dom, compId);' . "\n";
        $dom_replace .= 'AdobeEdge.alterDomPaths(dlContent.dom, compId);' . "\n";
        $dom_replace .= '})("' . $this->stageClass . '");';
        $content = preg_replace($dom_pattern, $dom_replace, $content);

        // Write new file.
        $fh = fopen($file, "w");
        fwrite($fh, $content);
        fclose($fh);
      }
    }

    return $altered;
  }


  /**
   * Retrieve poster image path and copy image if requested.
   *
   * The poster published with the OAM will be used in Edge Suite as a preview
   * image. For fields, this brings the problem that OAMs where uploaded but
   * are not stored yet. Therefore during validation the poster will be copied
   * temporarily so it can be used as a preview for unsaved fields. After
   * saving a node and its fields, the temporary poster will be deleted
   * and the poster within the project directory will be used. The distinction
   * between temporary and internal poster is based on poster_dir.
   *
   * @param string $poster_dir
   *   Destination directory for the temporary copy of the poster.
   */
  protected function extractPoster($poster_dir = NULL) {
    // Poster info is stored in OAM XML file.
    $xml = NULL;
    if (!empty($this->projectName)) {
      $xml = $this->tmpDir . '/' . $this->projectName . '_oam.xml';
    }

    $poster = NULL;

    if (file_exists($xml)) {
      $content = file_get_contents($xml);
      // Poster icon xml pattern.
      $poster_pattern = '/<icon src="([\w\/\.\-\\\\]*)"/';
      $match = array();
      if (preg_match($poster_pattern, $content, $match)) {
        // Convert win backslashes to slashes.
        $poster = str_replace('\\', '/', $match[1]);
      }
    }

    if ($poster != NULL) {
      // Internal path.
      $this->posterImage = $poster;

      $tmp_img = $this->tmpDir . '/' . $poster;

      // Check if temporary image is needed.
      if ($poster_dir != NULL && file_exists($poster_dir) && file_exists($tmp_img)) {
        global $user;
        $name = $user->uid . '_' . REQUEST_TIME;

        $info = image_get_info($tmp_img);
        // Possible exceptions on info will be caught.
        $new_img = $poster_dir . '/' . $name . '.' . $info['extension'];

        // External path.
        $this->posterImageExternal = file_unmanaged_copy($tmp_img, $new_img);
      }
    }
  }

  /**
   * Creates the destination folder.
   *
   * @throws Exception
   *   Throws an exception if directory can't be created or already exists and
   *   replace flag is not set.
   */
  protected function createDestFolder() {

    // Todo refactor: move deletion outside of build process and
    // just throw exception?
    if (file_exists($this->destinationDir)) {
      throw new Exception(t('The composition directory already exists.'));
    }
    elseif (!drupal_mkdir($this->destinationDir)) {
      throw new Exception(t('Directory %dir could not be created.', array('%dir' => check_plain($this->destinationDir))));
    }
  }


  /**
   * Library handling.
   *
   * Checks which libraries are included in the project library directory and
   * copies them to the shared library folder.
   */
  protected function readLibs($library_dir_base, $overwrite = FALSE) {
    // Read edge common lib files.
    $edge_lib_files = file_scan_directory($this->tmpDir . '/edge_includes', '/.*/', array('recurse' => FALSE));

    // Set up shared folder if it doesn't exist.
    $library_dir = $library_dir_base . '/edge_includes';
    if (!file_exists($library_dir) && !drupal_mkdir($library_dir)) {
      throw new Exception(t('Library directory %lib does not exists and could not be created.', array('%lib' => $library_dir)));
    }

    // Collect libs in array to save them in configuration.
    $lib_names = array();
    $lib_updates = $lib_ignored = $lib_added = array();
    foreach ($edge_lib_files as $lib) {
      // Only look at .js files.
      if (preg_match('/\.js$/', $lib->uri)) {
        // Check if lib already exists in shared folder.
        $exists = FALSE;
        if (file_exists($library_dir . '/' . $lib->filename)) {
          $exists = TRUE;
        }

        if ($overwrite || !$exists) {
          $moved = file_unmanaged_move($lib->uri, $library_dir, FILE_EXISTS_REPLACE);
          if (!$moved) {
            drupal_set_message(t('Library @lib could not be added/moved.', array('@lib' => check_plain($lib->filename))), 'warning');
          }
          if ($overwrite) {
            $lib_updates[] = $lib->filename;
          }
          else {
            $lib_added[] = $lib->filename;
          }
        }
        else {
          $lib_ignored[] = $lib->filename;
          file_unmanaged_delete($lib->uri);
        }
        // Add to lib array.
        $lib_names[] = $lib->filename;

        // Check for edge version, look for e.g. edge.0.1.6.min.js.
        if (empty($this->edgeVersion)) {
          $matches = array();
          preg_match('/edge\.([\.0-9]*)(?:\.min)?\.js$/', $lib->filename, $matches);
          if (count($matches) > 0) {
            $this->edgeVersion = $matches[1];
          }
        }
      }
    }

    // Output library file messages.
    $this->setFileMessages($lib_updates, 'The following libraries have been updated:');
    $this->setFileMessages($lib_added, 'The following libraries have been added:');
    $this->setFileMessages($lib_ignored, 'The following libraries are already present on the server, the existing versions will be used:');

  }

  /**
   * Copies all asset files with certain extensions.
   *
   * Copies files with js,png,... extension from the tmp
   * project directory to the permanent project directory.
   */
  public static function moveEdgeFiles($src, $dest_schema) {
    // TODO: Test.
    // TODO: Define global constant for default allowed extensions.
    $allowed_extensions = variable_get('edge_suite_allowed_asset_formats', 'js,png,jpg,gif,svg,css,html,woff,eot,ttf,mp3,ogg,oga,wav,m4a,aac');
    // Strip spaces.
    $allowed_extensions = str_replace(' ', '', $allowed_extensions);
    // Replace commas.
    $allowed_extensions = str_replace(',', '|', $allowed_extensions);
    // Check if list is valid.
    if (!preg_match('/^([a-zA-Z0-9]{2,6}\|)*([a-zA-Z0-9]{2,6})$/', $allowed_extensions)) {
      drupal_set_message(t('Asset file extension list not valid, default extensions (js|png|jpg|gif|svg|css|html|woff|eot|ttf|mp3|ogg|oga|wav|m4a|aac) will be used.'));
      $allowed_extensions = 'js|png|jpg|gif|svg|css|html|woff|eot|ttf|mp3|ogg|oga|wav|m4a|aac';
    }

    $files = file_scan_directory($src, '/\.(' . $allowed_extensions . ')$/');
    // Todo: Does this work with nested folders?
    // Todo: Check mimetype. Are mime types calculated from the file extension?
    foreach ($files as $f) {
      $dir = dirname($f->uri);
      // Create folder structure in the permanent directory.
      // Get dir path relative to project tmp base path without leading slash.
      $rel_dir = substr($dir, strlen($src) + 1);
      // Skip published folder.
      if (substr($rel_dir, 0, 8) === 'publish/') {
        continue;
      }
      if (!file_exists($dest_schema . '/' . $rel_dir)) {
        drupal_mkdir($dest_schema . '/' . $rel_dir);
      }
      // TODO: Feedback if file couldn't be moved.
      $moved = file_unmanaged_move($f->uri, $dest_schema . '/' . $rel_dir);
    }
  }

  /**
   * Retrieve ignored and obsolete files and deliver messages.
   */
  protected function setIgnoredFileMessages() {
    // Todo unclear: Not used, where to deliver them to?
    // Get a list of the remaining files and tell the user these files will
    // be ignored.
    $ignored_files = file_scan_directory($this->tmpDir, '/\.*$/');

    // Generate ignored and obsolete file messages.
    $files_names_obsolete = array();
    $files_names_ignored = array();
    foreach ($ignored_files as $f) {
      $obsolete_file_patterns = array(
        $this->projectName . '_oam.xml',
        $this->projectName . '.edge',
        $this->projectName . '.html',
      );
      if (in_array($f->filename, $obsolete_file_patterns)) {
        $files_names_obsolete[] = $f->filename;
      }
      else {
        $files_names_ignored[] = $f->filename;
      }
    }
    $this->setFileMessages($files_names_obsolete, 'The following files were ignored as they are not needed:');
    $this->setFileMessages($files_names_ignored, 'The following files were ignored as they are not supported:', 'warning');
  }
}
