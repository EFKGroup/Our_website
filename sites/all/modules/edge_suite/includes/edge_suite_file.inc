<?php
/**
 * @file
 * Edge Suite directory definitions and general file functionality.
 */

// Define Edge Suite main file directory.
if (!defined('EDGE_SUITE_DIR_MAIN')) {
  define('EDGE_SUITE_DIR_MAIN', 'public://edge_suite');
}

// Define Edge Suite project directory.
if (!defined('EDGE_SUITE_DIR_PROJECT')) {
  define('EDGE_SUITE_DIR_PROJECT', EDGE_SUITE_DIR_MAIN . '/project');
}

// Temporary poster directory.
if (!defined('EDGE_SUITE_DIR_POSTER_TMP')) {
  define('EDGE_SUITE_DIR_POSTER_TMP', EDGE_SUITE_DIR_MAIN);
}


/**
 * Checks and creates Edge Suite directories.
 */
function edge_suite_dir_check() {
  $exist = TRUE;

  // Check/create main directory.
  if (!is_dir(EDGE_SUITE_DIR_MAIN) && !drupal_mkdir(EDGE_SUITE_DIR_MAIN)) {
    drupal_set_message(t('Edge Suite directory could not be created in %dir', array('%dir' => EDGE_SUITE_DIR_MAIN)));
    $exist = FALSE;
  }

  // Check/create project directory.
  if (!is_dir(EDGE_SUITE_DIR_PROJECT) && !drupal_mkdir(EDGE_SUITE_DIR_PROJECT)) {
    drupal_set_message(t('Edge Suite project directory could not be created in %dir', array('%dir' => EDGE_SUITE_DIR_PROJECT)));
    $exist = FALSE;
  }

  return $exist;
}


/**
 * Get all Edge Suite specific paths.
 *
 * @param string $type
 *   Path type: main, project, poster_tmp
 * @param bool $real
 *   Real or schema path.
 *
 * @return null|string
 *   Path
 */
function edge_suite_get_path($type = 'main', $real = FALSE) {
  $path = NULL;

  switch ($type) {
    case 'main':
      $path = EDGE_SUITE_DIR_MAIN;
      break;

    case 'project':
      $path = EDGE_SUITE_DIR_PROJECT;
      break;

    case 'poster_tmp':
      $path = EDGE_SUITE_DIR_POSTER_TMP;
      break;
  }

  if ($path != NULL && $real) {
    $path = convertSchemaPath($path);
  }

  return $path;

}


/**
 * Convert schema path to the according relative real path.
 *
 * Converts e.g. public://edge_suite to /sites/default/files/edge_suite
 *
 * @param string $schema_path
 *   Schema path
 *
 * @return null
 *   Returns the relative real path, null if conversion fails
 */
function convertSchemaPath($schema_path) {
  $path = NULL;
  $url = file_create_url($schema_path);
  $url_parts = parse_url($url);
  if (isset($url_parts['path'])) {
    $path = $url_parts['path'];
  }
  return $path;
}


/**
 * Deletes an empty directory.
 *
 * @param string $path
 *   Path of the directory.
 *
 * @return bool
 *   True if directory was deleted.
 */
function edge_suite_dir_delete_empty($path) {
  $is_empty = FALSE;
  if (is_dir($path)) {
    $is_empty = TRUE;
    $dir = dir($path);
    while (($entry = $dir->read()) !== FALSE) {
      if ($entry != '.' && $entry != '..') {
        $is_empty = FALSE;
        break;
      }
    }
    $dir->close();

    if ($is_empty) {
      drupal_rmdir($path);
    }

  }
  return $is_empty;
}
