<?php
/**
 * @file
 * Edge suite entity functionality.
 */

/**
 * Implements hook_entity_info().
 */
function edge_suite_entity_info() {
  // Todo: What about the labels, where are they being used?
  $return['edge_suite_definition'] = array(
    'label' => t('Edge Suite Definition'),
    'entity class' => 'EdgeSuiteDefinition',
    'base table' => 'edge_suite_definition',
    'controller class' => 'EntityAPIController',
    'views controller class' => 'EntityDefaultViewsController',
    'access callback' => 'edge_suite_instance_access',
    'entity keys' => array(
      'id' => 'definition_id',
      'label' => 'label',
    ),
    'module' => 'edge_suite',
  );

  $return['edge_suite_instance'] = array(
    'label' => t('Edge Suite Instance'),
    'entity class' => 'EdgeSuiteInstance',
    'controller class' => 'EdgeSuiteInstanceController',
    'views controller class' => 'EntityDefaultViewsController',
    'base table' => 'edge_suite_instance',
    'access callback' => 'edge_suite_instance_access',
    'entity keys' => array(
      'id' => 'instance_id',
    ),
    'view modes' => array(),
    'module' => 'edge_suite',
  );

  // This will enable view modes for views and WHAT ELSE?
  $view_modes = edge_suite_composition_view_modes(TRUE);
  foreach ($view_modes as $key => $label) {
    $return['edge_suite_instance']['view modes'][$key] = array(
      'label' => $label,
      'custom settings' => FALSE,
    );
  }
  return $return;
}

/**
 * Get the overall used view modes for instances.
 *
 * @param bool $overview_mode
 *   True if generic view modes should be included.
 * @param bool $all
 *   Type of the source, global or field.
 *
 * @return array
 *   View modes with keys and labels.
 */
function edge_suite_composition_view_modes($overview_mode, $all = FALSE) {
  // Todo: Change usage of $all, this is not really consistent. Split
  // 'formatters' form view modes? Detect dynamically? drupal_alter?
  // Default view modes.
  $view_modes = array();

  if (!$overview_mode || $all) {
    $view_modes['inline'] = t('Inline');
    $view_modes['iframe'] = t('iframe');
  }

  if ($overview_mode || $all) {
    $view_modes['stage'] = t('Stage');

    // View mode for field formatter and views.
    $view_modes['poster'] = t('Poster');

    // Colorbox plugin.
    if (module_exists('colorbox')) {
      $view_modes['colorbox'] = t('Colorbox overlay');
    }
  }

  return $view_modes;
}


/**
 * Implements hook_token_info_alter().
 */
function edge_suite_token_info_alter(&$info) {
  // Clean out internal data.
  if (isset($info['tokens']['edge_suite_instance']['config'])) {
    unset($info['tokens']['edge_suite_instance']['config']);
  }
  if (isset($info['tokens']['edge_suite_instance']['field-id'])) {
    unset($info['tokens']['edge_suite_instance']['field-id']);
  }
  if (isset($info['tokens']['edge_suite_definition']['info'])) {
    unset($info['tokens']['edge_suite_definition']['info']);
  }
}


/**
 * Implements access callback.
 */
function edge_suite_instance_access($op, $model = NULL, $account = NULL) {
  // Todo: access implementation.
  return TRUE;
}

/**
 * Implements hook_property_info_alter().
 */
function edge_suite_entity_property_info_alter(&$info) {

  // Definition properties.
  $definition_props = &$info['edge_suite_definition']['properties'];

  $definition_props['project_name_unique'] = array(
    'label' => t('Unique project name'),
    'entity views field' => TRUE,
    'description' => t('Unique name of the project.'),
    'getter callback' => 'edge_suite_definition_general_property_getter',
    'type' => 'text',
  );

  $definition_props['poster_uri_path'] = array(
    'label' => t('Poster'),
    'entity views field' => TRUE,
    'description' => t('URI of the poster.'),
    'getter callback' => 'edge_suite_definition_general_property_getter',
    'type' => 'text',
  );

  $definition_props['poster_uri'] = array(
    'label' => t('Poster'),
    'description' => t('Relative URI of the poster.'),
    'getter callback' => 'edge_suite_definition_info_property_getter',
    'type' => 'text',
  );

  $definition_props['version'] = array(
    'label' => t('Version'),
    'entity views field' => TRUE,
    'description' => t('Edge Animate runtime version.'),
    'getter callback' => 'edge_suite_definition_info_property_getter',
    'type' => 'text',
  );

  $definition_props['width'] = array(
    'label' => t('Width'),
    'entity views field' => TRUE,
    'description' => t('Width of the composition.'),
    'getter callback' => 'edge_suite_definition_info_property_getter',
    'type' => 'text',
  );

  $definition_props['height'] = array(
    'label' => t('Height'),
    'entity views field' => TRUE,
    'description' => t('Height of the composition.'),
    'getter callback' => 'edge_suite_definition_info_property_getter',
    'type' => 'text',
  );

  // Instance properties.
  $instance_props = &$info['edge_suite_instance']['properties'];

  $instance_props['definition'] = array(
    'label' => t("Definition"),
    'type' => 'edge_suite_definition',
    'description' => t("Definition of the instance."),
    'schema field' => 'definition_id',
  );

  unset($instance_props['definition_id']);
}

/**
 * Entity property getter for general definition information.
 */
function edge_suite_definition_general_property_getter(&$definition, $options, $name, $type, $info) {
  if ($name == 'poster_uri_path') {
    return $definition->getPosterUri();

  }
  elseif ($name == 'project_name_unique') {
    return $definition->getProjectNameUnique();
  }
  return NULL;
}

/**
 * Entity property getter for definition information.
 */
function edge_suite_definition_info_property_getter(&$definition, $options, $name, $type, $info) {
  if (isset($definition->info[$name])) {
    return $definition->info[$name];
  }
  return NULL;
}


/***** CRUD and other API functions ******/

/**
 * Load multiple definitions.
 */
function edge_suite_definition_load_multiple($definition_ids = array(), $load_instances = FALSE, $conditions = array()) {
  $definitions = entity_load('edge_suite_definition', $definition_ids, $conditions);

  if ($load_instances) {
    foreach ($definitions as $definition) {
      $definition->loadInstances();
    }
  }
  return $definitions;
}

/**
 * Load a single definition.
 */
function edge_suite_definition_load($definition_id, $load_instances = FALSE) {
  $definitions = edge_suite_definition_load_multiple(array($definition_id), $load_instances);
  return reset($definitions);
}

/**
 * Load multiple instances.
 */
function edge_suite_instance_load_multiple($instance_ids = array(), $conditions = array()) {
  return entity_load('edge_suite_instance', $instance_ids, $conditions);
}

/**
 * Load an instances.
 */
function edge_suite_instance_load($instance_id) {
  $instances = edge_suite_instance_load_multiple(array($instance_id), array());
  return reset($instances);
}

/**
 * Get the rendered array for an instance in the passed view mode.
 *
 * Output ready to be rendered through drupal_render() to printable HTML.
 */
function edge_suite_instance_view($instance_id, $mode = NULL) {
  $instance = edge_suite_instance_load($instance_id);
  if ($instance != NULL) {
    return $instance->view($mode);
  }
  return NULL;
}

/**
 * Create a new instance with the given values.
 *
 * @param array $values
 *   Array of instance values that should be saved.
 *
 * @return int|null
 *   instance id on success, null otherwise.
 */
function edge_suite_instance_create($values) {
  $instance_id = NULL;

  if (!isset($values['label'])) {
    $values['label'] = t('Default');
  }
  if (!isset($values['description'])) {
    $values['description'] = '';
  }
  if (!isset($values['config'])) {
    $values['config'] = array();
  }

  $instance = entity_get_controller('edge_suite_instance')->create($values);

  // Todo future: do validation on save.
  $instance->save();

  if (isset($instance->instance_id)) {
    $instance_id = $instance->instance_id;
  }
  else {
    drupal_set_message(t('Instance could not be created.'));
  }

  return $instance_id;
}

/**
 * Delete a particular instance.
 *
 * @param int $instance_id
 *   Id of the composition instance.
 * @param bool $delete_definition
 *   If true it will be checked if definition can be deleted.
 *
 * @return bool
 *   True if instance was deleted, false otherwise
 */
function edge_suite_instance_delete($instance_id, $delete_definition) {
  $instance = edge_suite_instance_load($instance_id);
  if ($instance != NULL) {
    return $instance->delete($delete_definition);
  }
  return FALSE;
}

/**
 * Validates the given OAM and extracts meta information.
 *
 * @param int $fid
 *   The file id of the OAM.
 *
 * @return array|null
 *   Returns meta information of the composition on success.
 */
function edge_suite_definition_validate($fid) {
  $file = file_load($fid);

  $info = NULL;

  // Set up composition builder.
  $edge_build = new EdgeCompositionBuilder();
  try {
    $poster_dir = edge_suite_get_path('poster_tmp');
    if (!file_exists($poster_dir)) {
      mkdir($poster_dir);
    }
    $edge_build->validate($file->uri, $poster_dir);

    // Collect info, Edge Version not available through validation.
    $info = array(
      'project_name' => $edge_build->getProjectName(),
      'dimensions' => $edge_build->getDimensions(),
      'composition_id' => $edge_build->getCompositionClass(),
    );

    $poster_uri = $edge_build->getPosterImageExternal();
    if (file_exists($poster_uri)) {
      global $user;

      // Save tmp image as managed file so it will be deleted through cron.
      $poster_file = new stdClass();
      $poster_file->uri = $poster_uri;
      $poster_file->filename = drupal_basename($poster_uri);
      $poster_file->filemime = file_get_mimetype($poster_file->uri);
      $poster_file->uid = $user->uid;

      file_save($poster_file);

      $info['poster_uri'] = $poster_uri;
      $info['poster_fid'] = $poster_file->fid;
    }

  }
  catch(Exception $e){
    drupal_set_message(t('Error occured during validation: %err', array('%err' => $e->getMessage())));
  }

  return $info;
}

/**
 * Create a definition, and an according default instance.
 *
 * @param int $fid
 *   Fid of the OAM.
 * @param string $type
 *   Global or field
 * @param null $instance_values
 *   Instance values, if an instance should be created with the definition.
 * @param bool $replace_libraries
 *   Replace existing general edge animate libraries.
 *
 * @return null|int
 *   Return definition id on success, null otherwise.
 * @throws Exception
 */
function edge_suite_definition_create($fid, $type, &$instance_values = NULL, $replace_libraries = FALSE) {
  global $user;

  /* @var EdgeSuiteDefinition $definition */
  $definition = entity_get_controller('edge_suite_definition')->create(array(
    'uid' => $user->uid,
    'created' => REQUEST_TIME,
    'changed' => REQUEST_TIME,
    'fid' => $fid,
    'type' => $type,
  ));

  $definition_id = NULL;

  // Rollback to delete definition if instance can't be created.
  $transaction = db_transaction();
  try{
    // Build composition.
    $definition->build(FALSE, FALSE, $replace_libraries);

    // This should actually never happen as rebuild() checks this and also has
    // its own rollback, but just to make sure.
    if (!isset($definition->definition_id) || empty($definition->definition_id)) {
      throw new Exception(t('No definition id returned.'));
    }
    $definition_id = $definition->definition_id;

    // Build instance if definition is valid and instance was requested.
    if ($definition_id && $instance_values != NULL) {
      // Check instance values.
      $instance_values['definition_id'] = $definition_id;
      $instance_id = edge_suite_instance_create($instance_values);

      if ($instance_id == NULL || empty($instance_id)) {
        throw new Exception(t('Instance could not be created.'));
      }

      // Return instance id through values.
      $instance_values['instance_id'] = $instance_id;
    }
  }
  catch(Exception $e){
    $transaction->rollback();
    // If definition was build it needs to be deleted.
    if (isset($definition->definition_id)) {
      $definition->delete();
    }
    drupal_set_message($e->getMessage(), 'error');
  }
  return $definition_id;
}


/**
 * Replace an existing definition.
 *
 * @param int $fid
 *   Fid of the new OAM.
 * @param int $definition_id
 *   Id of the definition.
 * @param bool $replace_libraries
 *   Replace existing general edge animate libraries.
 */
function edge_suite_definition_replace($fid, $definition_id, $replace_libraries = FALSE) {
  // Load definition.
  $definition = edge_suite_definition_load($definition_id);

  // Set new fid.
  $definition->fid = $fid;

  if ($definition) {
    $definition->build(TRUE, TRUE, $replace_libraries);
  }
}

/**
 * Rebuild project files for a definition.
 *
 * @param int $definition_id
 *   Id of the definition.
 * @param bool $replace_libraries
 *   Replace existing general edge animate libraries.
 */
function edge_suite_definition_rebuild($definition_id, $replace_libraries = FALSE) {
  // Load definition.
  $definition = edge_suite_definition_load($definition_id);

  // Rebuild definition.
  if ($definition) {
    $definition->build(TRUE, FALSE, $replace_libraries);
  }
}

/**
 * Rebuilds all compositions.
 */
function edge_suite_definition_rebuild_all() {
  // Get all definition ids.
  $definitions = edge_suite_definition_load_multiple(FALSE);

  // Rebuild all definitions.
  if (count($definitions) > 0) {
    foreach ($definitions as $definition) {
      $definition->build(TRUE, FALSE);;
    }
  }
}

/**
 * Delete definition and if possible source OAM.
 *
 * Deletes the definition, all instances and all files of the composition.
 *
 * @param int $definition_id
 *   Id of the definition
 * @param bool $delete_instances
 *   If true all instances will be deleted.
 *
 * @return bool
 *   True if composition was deleted, false otherwise
 */
function edge_suite_definition_delete($definition_id, $delete_instances) {

  $definition = edge_suite_definition_load($definition_id);
  if ($definition != NULL) {
    return $definition->delete($delete_instances);
  }
  return FALSE;
}


/**
 * EdgeSuiteDefinition class.
 */
class EdgeSuiteDefinition extends Entity {
  protected $controller;
  protected $wrapper;

  /**
   * Constructor.
   */
  public function __construct(array $values = array(), $entity_type = NULL) {
    parent::__construct($values, $entity_type);
    $this->controller = entity_get_controller('edge_suite_definition');
    $this->wrapper = entity_metadata_wrapper('edge_suite_definition', $this);
  }

  /**
   * Label callback.
   */
  public function label() {
    // Todo: Where is this being used?
    return 'Edge Suite Definition Custom Label';
  }

  /**
   * Get definition property value.
   *
   * @param string $key
   *   Value key.
   * @param mixed $default
   *   Default value in case value is not set, empty or null.
   *
   * @return mixed
   *   Return the definition value for the given key.
   */
  public function definition($key, $default = NULL) {
    $value = NULL;
    if (isset($this->wrapper) && isset($this->wrapper->{$key})) {
      $value = $this->wrapper->{$key}->value();
    }

    if (empty($value) || $value == NULL) {
      $value = $default;
    }
    return $value;
  }

  /**
   * Builds a unique name for a composition definition.
   *
   * The unique name will be build through the user id, the definition id and
   * the project name, to provide maximal readability.
   *
   * @return null|string
   *   Returns the name or null if the definition is not valid
   */
  public function getProjectNameUnique() {
    return strtolower($this->definition('project_name')) . '_' . $this->definition_id;
  }

  /**
   * Get poster uri.
   *
   * @param bool $real
   *   Returns the real path if set to true, otherwise the stream wrapper.
   *
   * @return null|string
   *   Poster uri.
   */
  public function getPosterUri($real = FALSE) {
    $value = NULL;
    if (isset($this->info['poster_uri'])) {
      $value = $this->getProjectPath($real) . '/' . $this->info['poster_uri'];
    }
    return $value;
  }

  /**
   * Get project path.
   *
   * @param bool $real
   *   Returns the real project path if set to true, otherwise the stream
   *   wrapper.
   *
   * @return null|string
   *   Project path.
   */
  public function getProjectPath($real = FALSE) {
    $unique_name = $this->definition('project_name_unique');
    $project_dir = NULL;
    if ($unique_name != NULL && !empty($unique_name)) {
      $project_dir = edge_suite_get_path('project', $real) . '/' . $unique_name;
    }
    return $project_dir;
  }

  /**
   * Ensures instances are loaded into the definition.
   */
  public function loadInstances() {
    if (!isset($this->instances)) {
      $this->instances = edge_suite_instance_load_multiple(FALSE, array('definition_id' => $this->definition_id));
    }
  }

  /**
   * Delete a definition, possibly with all instances.
   *
   * @param bool $delete_instances
   *   True if related instances should be deleted as well.
   *
   * @return bool|void
   *   True if deletion was succesful.
   */
  public function delete($delete_instances = FALSE) {

    $this->loadInstances();
    if (!$delete_instances && isset($this->instances) && count($this->instances) > 0) {
      drupal_set_message(t('Edge Suite definition can not be deleted, still being used by instances. Use according flag to delete all instances as well.'));
      return FALSE;
    }

    // Delete the unpacked files, means the project directory.
    $project_dir = $this->getProjectPath();
    if ($project_dir != NULL && is_dir($project_dir)) {
      file_unmanaged_delete_recursive($project_dir);
    }

    // Delete OAM.
    $file = file_load($this->fid);
    file_usage_delete($file, 'edge_suite', 'edge_suite_definition', $this->definition_id);
    $file_deleted = file_delete($file);
    if (!$file_deleted) {
      drupal_set_message(t('Unable to delete OAM file. File might be in use by other definition.'), 'warn');
    }

    // Try to delete OAM directory.
    edge_suite_dir_delete_empty(drupal_dirname($file->uri));

    // Delete all instances.
    if ($delete_instances) {
      if (isset($this->instances)) {
        foreach ($this->instances as $instance_id => $instance) {
          edge_suite_instance_delete($instance_id, FALSE);
        }
      }
    }

    parent::delete();

    return TRUE;
  }

  /**
   * Main function to (re)build the project files from an OAM.
   *
   * @param bool $replace_build
   *   True if existing project files should be replaced.
   * @param bool $replace_oam
   *   True if OAM should be replaced.
   * @param bool $replace_libraries
   *   Replace existing edge animate libraries.
   *
   * @return bool
   *   True on success.
   * @throws Exception
   */
  public function build($replace_build = FALSE, $replace_oam = FALSE, $replace_libraries = FALSE) {

    if ($replace_oam && !$replace_build) {
      throw new Exception(t('OAM can not be replaced without rebuilding the project. Rebuild flag not set.'));
    }

    if (!edge_suite_dir_check()) {
      throw new Exception(t('Edge Suite main and project directory could not be created.'));
    }

    $dir_main = edge_suite_get_path('main');
    $dir_main_project = edge_suite_get_path('project');

    if (!isset($this->fid)) {
      drupal_set_message('Fid not set for definition.');
      return FALSE;
    }

    if (!isset($this->type)) {
      drupal_set_message('Definition type not set.');
      return FALSE;
    }

    $file = file_load($this->fid);

    global $user;

    $project_tmp_name = 'clean_u' . $user->uid . '_t' . time();
    $project_tmp_path = $dir_main_project . '/' . $project_tmp_name;

    // Set up composition builder.
    $edge_build = new EdgeCompositionBuilder();

    $transaction = db_transaction();
    try {
      // The composition is not being replaced right away, it will be build
      // first and then copied to the right location on success.
      $edge_build->build($file->uri, $project_tmp_path, $dir_main, $replace_libraries);

      $this->project_name = $edge_build->getProjectName();
      $this->composition_id = $edge_build->getCompositionClass();

      $this->info = array(
        'version' => $edge_build->getEdgeVersion(),
        'poster_uri' => $edge_build->getPosterImageInternal(),
      );
      $this->info += $edge_build->getDimensions();

      $definition_original = NULL;
      if (isset($this->definition_id)) {
        // We could load it into 'original' for later use, but not worse
        // possible side effects.
        $definition_original = entity_load_unchanged('edge_suite_definition', $this->definition_id);

        if (!$replace_build && file_exists($this->getProjectPath())) {
          throw new Exception(t('Project path exists and replace rebuild flag is not set.', array('%path' => $this->getProjectPath())));
        }
        if (!$replace_oam && $definition_original->fid != $this->fid) {
          throw new Exception(t('New OAM but replace OAM flag is not set.'));
        }
      }

      $this->save();

      if (!$this->definition_id) {
        throw new Exception(t('No definition id set after save.'));
      }

      $project_path = $this->getProjectPath();
      // Delete existing directory.
      if ($definition_original != NULL) {
        $old_project_path = $definition_original->getProjectPath();
        if (file_exists($old_project_path)) {
          if (!$replace_build) {
            throw new Exception(t('Project path exists and replace rebuild flag is not set.', array('%path' => $old_project_path)));
          }
          else {
            file_unmanaged_delete_recursive($old_project_path);
          }
        }
      }
      // Rename tmp folder to new source folder.
      rename($project_tmp_path, $project_path);

      if ($file->status != FILE_STATUS_PERMANENT) {
        // Mark OAM as permanent.
        $file->status = FILE_STATUS_PERMANENT;
        file_save($file);
        file_usage_add($file, 'edge_suite', 'edge_suite_definition', $this->definition_id);
      }

      if ($definition_original != NULL && $definition_original->fid != $this->fid) {
        if (!$replace_oam) {
          // Throwing an exception at this point would make things worse,
          // project directory has already been deleted.
          drupal_set_message(t('New OAM but replace OAM flag is not set.'));
        }
        else {
          $file_old = file_load($definition_original->fid);
          file_usage_delete($file_old, 'edge_suite', 'edge_suite_definition');
          file_delete($file_old);
        }
      }
      return TRUE;
    }
    catch(Exception $e){
      $transaction->rollback();

      unset($this->definition_id);

      // Clean out all files.
      drupal_set_message($e->getMessage(), 'error');

      // Make sure source extraction directory gets cleaned out.
      $edge_build->cleanup();

      // Delete project files. Make sure both directories get deleted.
      if (file_exists($project_tmp_path)) {
        file_unmanaged_delete_recursive($project_tmp_path);
      }
      if (isset($project_path) && file_exists($project_path)) {
        // Don't delete the currently active project dir, chances are good
        // the exception was thrown before directory renaming, therefore
        // a full rollback is possible.
        if ($replace_build && isset($old_project_path) && !file_exists($old_project_path)) {
          drupal_set_message(t('Error during rebuild, complete rollback of old project files can not be guaranteed. Please check the project for consistency!'));
        }
        else {
          file_unmanaged_delete_recursive($project_path);
        }
      }

      // Would be deleted on cron, delete it right away anyway.
      if ($file->status != FILE_STATUS_PERMANENT) {
        file_delete($file);
      }

      // Todo unclear: Libraries might have been copied. Critical?
      // Todo unclear: Do a validation run for the global instances as well?
      return FALSE;
    }
  }
}


/**
 * EdgeSuiteInstance class.
 */
class EdgeSuiteInstance extends Entity {
  protected $wrapper;
  protected $nodeTokenContext = NULL;
  public $definition = NULL;

  /**
   * Constructor.
   */
  public function __construct(array $values = array(), $entity_type = NULL) {
    parent::__construct($values, $entity_type);
    $this->wrapper = entity_metadata_wrapper('edge_suite_instance', $this);
    $this->definition = $this->wrapper->definition->value();

    // Todo: do basic validation of all values, provide defaults.
  }

  /**
   * Label function.
   */
  public function label() {
    // Todo: Where is this being used?
    return 'Edge Suite Instance Custom Label';
  }

  /**
   * Get instance property value.
   *
   * @param string $key
   *   Value key.
   * @param mixed $default
   *   Default value in case value is not set, empty or null.
   *
   * @return mixed
   *   Return the instance value for the given key.
   */
  public function instance($key, $default = NULL) {
    $value = NULL;
    if (isset($this->wrapper->{$key})) {
      $value = $this->wrapper->{$key}->value();
    }

    if (empty($value) || $value == NULL) {
      $value = $default;
    }

    return $value;
  }

  /**
   * Get definition property value.
   *
   * @param string $key
   *   Value key.
   * @param mixed $default
   *   Default value in case value is not set, empty or null.
   *
   * @return mixed
   *   Return the definition value for the given key.
   */
  public function definition($key, $default = NULL) {
    return $this->definition->definition($key, $default);
  }

  /**
   * Get config value.
   *
   * @param string $key
   *   Value key.
   * @param mixed $default
   *   Default value in case value is not set, empty or null.
   *
   * @return mixed
   *   Return the config value for the given key.
   */
  public function config($key, $default = NULL) {
    $value = NULL;
    if (isset($this->config[$key])) {
      $value = $this->config[$key];
    }

    if (empty($value) || $value == NULL) {
      $value = $default;
    }

    return $value;
  }

  /**
   * Delete an instance, possibly try to delete the related definition.
   *
   * @param bool $delete_definition
   *   If true related definition will be deleted, if no other instances exist.
   *
   * @return bool|void
   *   True on success.
   */
  public function delete($delete_definition = FALSE) {
    // Delete block entries. If blocks are not unregistered new edge blocks
    // sometimes don't show up in the overview listing. This is a bit strange as
    // there seems to be no API call for modules to unregister blocks.
    db_delete('block')
      ->condition('delta', 'edge_suite_composition_block_' . $this->instance_id)
      ->execute();

    parent::delete();

    // In case of fields, definition can be deleted if the instance is being
    // deleted.
    if ($delete_definition) {
      edge_suite_definition_delete($this->definition_id, FALSE);
    }

    return TRUE;
  }

  /**
   * Set node token context for e.g. data injection.
   */
  public function setNodeTokenContext($node) {
    $this->nodeTokenContext = $node;
  }

  /**
   * Get the evaluated json data for data injection.
   */
  public function getJsonConfig() {
    $json = NULL;
    if (isset($this->config['json_config']) && !empty($this->config['json_config'])) {
      $json = $this->config['json_config'];

      // Replace tokens.
      $node = NULL;
      if (isset($this->context_entity_id)) {
        $node = node_load($this->context_entity_id);
      }
      elseif ($this->definition('type') == 'global') {
        $node = menu_get_object();
        if ($node == NULL && isset($this->nodeTokenContext)) {
          $node = $this->nodeTokenContext;
        }
      }
      $json = token_replace($json, array('node' => $node, 'edge_suite_instance' => $this));
    }
    return $json;
  }

  /**
   * View instance in passed view mode.
   */
  public function view($view_mode = NULL, $langcode = NULL, $page = NULL) {
    $build = parent::view($view_mode, $langcode);
    // Set custom theme function for cleaner output.
    foreach ($build['edge_suite_instance'] as &$entity_build) {
      $entity_build['#theme'] = 'edge_suite_instance';
    }
    return $build;
  }

  /**
   * Get the field instance, if the instance is of type field.
   */
  public function getFieldInstance() {
    if (isset($this->field)) {
      return $this->field;
    }
    elseif (isset($this->field_id)) {
      // Load field instance.
      $fields = field_read_instances(array('id' => $this->field_id));
      if (!empty($fields)) {
        $this->field = reset($fields);
        return $this->field;
      }
    }
    return NULL;
  }
}

class EdgeSuiteInstanceController extends EntityAPIController {

  /**
   * Implements buildContent interface.
   */
  public function buildContent($entity, $view_mode = NULL, $langcode = NULL, $content = array()) {

    // Todo: check field config;
    // Check if specific view mode on instance level is defined and allowed.
    if ($view_mode == NULL || $view_mode == 'stage') {
      $view_mode = $entity->config('view_mode', NULL);
      if (isset($entity->display_settings) && isset($entity->display_settings['stage_formatter']) && !empty($entity->display_settings['stage_formatter'])) {
        $view_mode = $entity->display_settings['stage_formatter'];
      }
    }

    // Validation check, set default view mode.
    $view_modes = edge_suite_composition_view_modes(TRUE, TRUE);
    if (!array_key_exists($view_mode, $view_modes)) {
      $view_mode = 'inline';
    }
    $content['edge_suite_instance'] = array(
      '#theme' => 'edge_suite_view_' . $view_mode,
      '#instance' => $entity,
    );

    $build = parent::buildContent($entity, $view_mode, $langcode, $content);

    return $build;
  }
}
