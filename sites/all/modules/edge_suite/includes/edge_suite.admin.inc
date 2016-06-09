<?php
/**
 * @file
 * Edge suite administration functionality.
 */

/**
 * Form constructor for Edge Suite settings.
 */
function edge_suite_settings($form, &$form_state) {

  // Configuration group.
  $form['edge_suite_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configuration'),
  );

  $form['edge_suite_settings']['edge_suite_xua_compatible'] = array(
    '#type' => 'checkbox',
    '#title' => t('X-UA-Compatible'),
    '#description' => t('Enable X-UA-Compatible with IE=edge. Lets IE use the latest rendering engine.'),
    '#default_value' => variable_get('edge_suite_xua_compatible', TRUE),
  );

  $form['edge_suite_settings']['edge_suite_jquery_noconflict'] = array(
    '#type' => 'checkbox',
    '#title' => t('jQuery noConflict'),
    '#description' => t("This runs Edge Animate in jQuery noConflict mode to avoid conflicts with Drupal's jQuery version."),
    '#default_value' => variable_get('edge_suite_jquery_noconflict', TRUE),
  );

  $form['edge_suite_settings']['edge_suite_iframe_token'] = array(
    '#type' => 'checkbox',
    '#title' => t('iframe time sensitive tokens'),
    '#description' => t("Enable time sensitive tokens for iframes to protect your compositions from being accessed directly through urls copied from iframes by users."),
    '#default_value' => variable_get('edge_suite_iframe_token', TRUE),
  );

  $form['edge_suite_settings']['edge_suite_allowed_asset_formats'] = array(
    '#type' => 'textfield',
    '#title' => t('Asset formats'),
    '#description' => t('Set the file extensions that are allowed for asset files within a composition archive. JavaScript files are always allowed. Comma-separated, e.g. png, jpg, gif'),
    '#default_value' => variable_get('edge_suite_allowed_asset_formats', 'js, png, jpg, gif, svg, css, html, woff, eot, ttf, mp3, ogg, oga, wav, m4a, aac'),
    '#element_validate' => array('edge_suite_validate_asset_formats'),
  );

  $form['global_compositions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Global compositions'),
  );

  $form['global_compositions']['edge_suite_block_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Block usage'),
    '#description' => t('If enabled, a block will be automatically provided for every global composition instance.'),
    '#default_value' => variable_get('edge_suite_block_enable', TRUE),
  );

  $form['global_compositions']['edge_suite_global_oam_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Global OAM storage'),
    '#description' => t('Set the path for storing the global oam files.'),
    '#default_value' => variable_get('edge_suite_global_oam_path', 'edge_suite'),
  );

  $form['global_compositions']['edge_suite_max_archive_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Global OAM size'),
    '#description' => t('Maximum size of an OAM in MB. If left empty the size defaults to 8MB.'),
    '#size' => 3,
    '#default_value' => variable_get('edge_suite_max_archive_size', 8),
  );

  // Rebuild all, only add in devel mode.
  if (variable_get('edge_suite_debug_mode', 0)) {
    $form['edge_suite_rebuild'] = array(
      '#type' => 'fieldset',
      '#title' => t('Rebuild'),
    );

    $form['edge_suite_rebuild']['rebuild_all'] = array(
      '#type' => 'submit',
      '#value' => t('Rebuild all definitions'),
      '#submit' => array('edge_suite_definition_rebuild_all'),
    );
  }

  return system_settings_form($form);
}

/**
 * Validation callback for asset formats.
 */
function edge_suite_validate_asset_formats($element, &$form_state) {
  if (!empty($element['#value'])) {
    $element['#value'] = str_replace(' ', '', $element['#value']);
    $form_state['values']['edge_suite_allowed_asset_formats'] = $element['#value'];
    $valid = preg_match('/^([a-zA-Z0-9]{2,6},)*([a-zA-Z0-9]{2,6})$/', $element['#value']);
    if (!$valid) {
      form_error($element, t('Not a valid list of formats.'));
    }
  }
}


/**
 * Administrative form for edge compositions.
 */
function edge_suite_admin_form($form, &$form_state) {
  drupal_add_css(drupal_get_path('module', 'edge_suite') . '/includes/edge_suite_admin.css');

  // Get all available compositions.
  $definitions = edge_suite_definition_load_multiple(FALSE, TRUE, array('type' => 'global'));

  // Put together the overview table.
  $header = array(t('Compositions'), t('Poster'), t('Operations'));
  $rows = array();
  if (count($definitions) > 0) {
    foreach ($definitions as $definition) {

      $path_base = 'admin/structure/edge_suite/' . $definition->definition_id;
      $def_actions = array(
        l(t('add block'), $path_base . '/add'),
        l(t('rebuild'), $path_base . '/rebuild'),
        l(t('reupload'), $path_base . '/reupload'),
        l(t('delete'), $path_base . '/delete'),
      );

      // Add poster.
      $poster = '';
      // Todo: getter callback for full poster path?
      if ($definition->definition('poster_uri_path', NULL) != NULL) {
        $poster_uri = $definition->definition('poster_uri_path');

        $variables = array(
          'style_name' => 'thumbnail',
          'path' => $poster_uri,
        );

        $img_meta = image_get_info($poster_uri);
        if (is_array($img_meta)) {
          $variables['width'] = $img_meta['width'];
          $variables['height'] = $img_meta['height'];
        }
        else {
          $variables['width'] = $variables['height'] = NULL;
        }
        $poster = theme('image_style', $variables);
      }

      $def_cells = array();
      $def_cells[] = array(
        'data' => $definition->definition('project_name') . ' - ' . $definition->definition('composition_id'),
        'class' => 'es-definition-label',
      );
      $def_cells[] = array(
        'data' => $poster,
        'class' => 'es-definition-poster',
      );
      $def_cells[] = array(
        'data' => implode(' | ', $def_actions),
        'class' => 'es-definition-operations',
      );
      $rows[] = array(
        'data' => $def_cells,
        'no_striping' => TRUE,
        'class' => array('even', 'definition-row'),
      );

      foreach ($definition->instances as $inst_id => $instance) {
        $instance_cells = array();
        $instance_cells[] = array(
          'data' => $instance->instance('label'),
          'class' => 'es-instance-label',
        );

        $instance_cells[] = array(
          'data' => '',
        );

        $inst_actions = array(
          l(t('test'), $path_base . '/test/' . $inst_id),
          l(t('edit'), $path_base . '/edit/' . $inst_id),
          l(t('delete'), $path_base . '/delete/' . $inst_id),
        );
        $instance_cells[] = array(
          'data' => implode(' | ', $inst_actions),
          'class' => 'es-instance-operations',
        );
        $rows[] = array(
          'data' => $instance_cells,
          'no_striping' => TRUE,
          'class' => array('instance-row'),
        );
      }
    }
  }
  $table = theme('table', array('header' => $header, 'rows' => $rows));

  // Add the table to the form.
  $form['edge_suite_composition_table'] = array(
    '#markup' => $table,
  );

  // Check if private files are configured, to be able to store archive sources.
  if (file_stream_wrapper_valid_scheme("private")) {
    // Form field to upload a new archive.
    $form['edge_suite_comp_upload'] = array(
      '#type' => 'file',
      '#title' => t('Upload new composition'),
      '#description' => t('Use an Edge Animate OAM file which gets generated when publishing the project with option "Animate Deployment Package".'),
      '#size' => 40,
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Upload'),
    );
  }
  else {
    drupal_set_message(t('Private file directory needs to be <a href="@config">configured</a> to upload compositions.', array('@config' => url('admin/config/media/file-system'))), 'warning');
  }

  return $form;
}

/**
 * Validation handler for the administration form.
 *
 * The uploaded file is checked and the edge composition gets created on
 * success.
 */
function edge_suite_admin_form_validate(&$form, &$form_state) {
  edge_suite_process_archive_upload($form, $form_state);
}

/**
 * Callback to validate and process an uploaded archive.
 */
function edge_suite_process_archive_upload(&$form, &$form_state) {
  $validators = array(
    'file_validate_extensions' => array('oam'),
    'file_validate_size' => array(variable_get('edge_suite_max_archive_size', 8) * 1024 * 1024),
  );

  // Save the file as a temporary file.
  $oam_path = 'private://' . check_plain(variable_get('edge_suite_global_oam_path', 'edge_suite'));
  if (!file_exists($oam_path)) {
    if (!drupal_mkdir($oam_path)) {
      form_set_error('edge_suite_comp_upload', t("Global upload directory could not be created, please check the settings page."));
    };
  }
  $file = file_save_upload('edge_suite_comp_upload', $validators, $oam_path);
  if ($file === FALSE) {
    form_set_error('edge_suite_comp_upload', t("Failed to upload the archive."));
  }
  elseif ($file !== NULL) {
    // File extension validation is weird, e.g. a JS file test.js gets renamed
    // to test.js.txt, the extension .txt will be added to the validator and the
    // file gets uploaded. Basically we can't be sure we have an archive here so
    // far.
    if (!preg_match('/\.oam$/i', $file->filename)) {
      form_set_error('edge_suite_comp_upload', t("Wrong file extension, only OAM files are allowed."));
      file_delete($file);
    }
    else {
      // Accept file and create.
      $form_state['values']['edge_suite_comp_upload'] = $file;
      $definition_id = 0;
      // Check if its a replace and if its a valid definition id.
      if (isset($form_state['values']['edge_suite_comp_replace']) && $form_state['values']['edge_suite_comp_replace'] == TRUE && isset($form_state['values']['edge_suite_definition_id']) && $form_state['values']['edge_suite_definition_id'] > 0) {
        $definition_id = $form_state['values']['edge_suite_definition_id'];
      }
      // Create new definition and instance.
      if ($definition_id == 0) {
        $instance_values = array('label' => 'Default');
        $definition_id = edge_suite_definition_create($file->fid, 'global', $instance_values);
      }
      // Replace definition.
      else {
        edge_suite_definition_replace($file->fid, $definition_id);
      }

      $form_state['values']['edge_suite_definition_id'] = $definition_id;
    }
  }
}

/**
 * Menu callback for confirmation of edge composition reupload.
 */
function edge_suite_definition_reupload_confirm($form, &$form_state, $definition_id) {
  // Load composition and show confirmation dialog.
  $definition_id = intval($definition_id);
  $definition = edge_suite_definition_load($definition_id);
  $form['edge_suite_definition_id'] = array(
    '#type' => 'value',
    '#value' => $definition_id,
  );

  // Check if private files are configured, to be able to store archive sources.
  if (file_stream_wrapper_valid_scheme("private")) {
    // Form field to upload the new archive.
    $form['edge_suite_comp_upload'] = array(
      '#type' => 'file',
      '#title' => t('Upload new composition'),
      '#description' => t('Use an Edge Animate OAM file which gets generated when publishing the project with option "Animate Deployment Package".'),
      '#size' => 40,
    );
  }
  else {
    drupal_set_message(t('Private file directory needs to be <a href="@config">configured</a> to upload compositions.', array('@config' => url('admin/config/media/file-system'))), 'warning');
  }

  return confirm_form($form,
    t('Are you sure you want to reupload %title?', array('%title' => $definition->project_name)),
    'admin/structure/edge_suite',
    t('This action cannot be undone.'),
    t('Reupload'),
    t('Cancel')
  );
}

/**
 * Trigger edge composition reupload.
 */
function edge_suite_definition_reupload_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $definition = edge_suite_definition_load($form_state['values']['edge_suite_definition_id']);
    $form_state['values']['edge_suite_comp_replace'] = TRUE;
    edge_suite_process_archive_upload($form, $form_state);

    if ($form_state['values']['edge_suite_definition_id'] > 0) {
      watchdog('edge_suite', 'Reupload of Edge composition %title.', array('%title' => $definition->project_name));
      drupal_set_message(t('Reupload of Edge composition %title was successful.', array('%title' => $definition->project_name)));
    }
    else {
      drupal_set_message(t('Reupload of Edge composition %title was not successful.', array('%title' => $definition->project_name)));
    }
  }
  // Redirect to admin form.
  $form_state['redirect'] = 'admin/structure/edge_suite';
}


/**
 * Menu callback for confirmation of edge composition deletion.
 */
function edge_suite_definition_delete_confirm($form, &$form_state, $definition_id) {
  // Load composition and show confirmation dialog.
  $definition_id = intval($definition_id);
  $definition = edge_suite_definition_load($definition_id);
  $form['edge_suite_definition_id'] = array(
    '#type' => 'value',
    '#value' => $definition_id,
  );
  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $definition->project_name)),
    'admin/structure/edge_suite',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Trigger edge composition deletion.
 */
function edge_suite_definition_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    // Load composition and delete it.
    $definition = edge_suite_definition_load($form_state['values']['edge_suite_definition_id']);
    $deleted = edge_suite_definition_delete($form_state['values']['edge_suite_definition_id'], TRUE);

    if ($deleted) {
      watchdog('edge_suite', 'Deleted Edge composition %title.', array('%title' => $definition->project_name));
      drupal_set_message(t('Edge composition %title has been deleted.', array('%title' => $definition->project_name)));
    }
    else {
      drupal_set_message(t('Edge composition %title has not been deleted.', array('%title' => $definition->project_name)), 'warning');
    }
  }
  // Redirect to admin form.
  $form_state['redirect'] = 'admin/structure/edge_suite';
}

/**
 * Menu callback for confirmation of edge composition deletion.
 */
function edge_suite_instance_delete_confirm($form, &$form_state, $definition_id, $instance_id) {
  // Load instance and show confirmation dialog.
  $instance_id = intval($instance_id);
  $instance = edge_suite_instance_load($instance_id);
  $form['edge_suite_instance_id'] = array(
    '#type' => 'value',
    '#value' => $instance_id,
  );
  return confirm_form($form,
    t('Are you sure you want to delete the instance %title?', array('%title' => $instance->label)),
    'admin/structure/edge_suite',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Trigger edge composition deletion.
 */
function edge_suite_instance_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    // Load composition instance and delete it.
    $instance = edge_suite_instance_load($form_state['values']['edge_suite_instance_id']);
    $num_deleted = edge_suite_instance_delete($form_state['values']['edge_suite_instance_id'], FALSE);

    if ($num_deleted > 0) {
      watchdog('edge_suite', 'Deleted Edge instance %title.', array('%title' => $instance->label));
      drupal_set_message(t('Edge instance %title has been deleted.', array('%title' => $instance->label)));
    }
    else {
      drupal_set_message(t('Edge instance %title has not been deleted.', array('%title' => $instance->label)));
    }
  }
  // Redirect to admin form.
  $form_state['redirect'] = 'admin/structure/edge_suite';
}

/**
 * Menu callback for confirmation of edge composition rebuild.
 */
function edge_suite_definition_rebuild_confirm($form, &$form_state, $id) {
  // Load composition and show confirmation dialog.
  $id = intval($id);
  $definition = edge_suite_definition_load($id);
  $form['edge_suite_definition_id'] = array(
    '#type' => 'value',
    '#value' => $id,
  );
  $form['edge_suite_replace_libraries'] = array(
    '#type' => 'checkbox',
    '#title' => t('Update/replace shared libraries'),
    '#description' => t('If enabled, the currently shared libraries on the server will be replaced with the libraries form the chosen composition.'),
    '#default_value' => FALSE,
  );
  return confirm_form($form,
    t('Are you sure you want to rebuild %title?', array('%title' => $definition->project_name)),
    'admin/structure/edge_suite',
    t('This action cannot be undone.'),
    t('Rebuild'),
    t('Cancel')
  );
}

/**
 * Trigger edge composition rebuild.
 */
function edge_suite_definition_rebuild_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {

    // Check if libraries should be replaced.
    $replace_libraries = FALSE;
    if (isset($form_state['values']['edge_suite_replace_libraries']) && $form_state['values']['edge_suite_replace_libraries'] == TRUE) {
      $replace_libraries = TRUE;
    }
    // Rebuild composition.
    edge_suite_definition_rebuild($form_state['values']['edge_suite_definition_id'], $replace_libraries);

    // Redirect to admin form.
    $form_state['redirect'] = 'admin/structure/edge_suite';
  }
}

/**
 * Form handler for edit instance form.
 */
function edge_suite_instance_form_edit($form, &$form_state, $definition_id, $instance_id) {

  // Validate definition id.
  $definition_id = intval($definition_id);
  $definition = edge_suite_definition_load($definition_id);
  if ($definition == NULL) {
    drupal_set_message(t('Not a valid definition id: @id', array('@id' => $definition_id)), 'error');
    // TODO: watchdog.
    return array();
  }

  $form['definition_id'] = array(
    '#type' => 'value',
    '#value' => $definition_id,
  );

  // Validate instance id. If its valid add it to the form.
  $instance_id = intval($instance_id);
  $instance = NULL;
  if ($instance_id > 0) {
    $instance = edge_suite_instance_load($instance_id);
    if (!isset($instance)) {
      drupal_set_message(t('Not a valid instance id: @id', array('@id' => $instance_id)), 'error');
      // TODO: watchdog.
      return array();
    }
    else {
      $form['instance_id'] = array(
        '#type' => 'value',
        '#value' => $instance_id,
      );
    }
  }

  // Add instance form fields (also used by fields).
  $instance_form_values = array(
    'label' => isset($instance->label) ? $instance->label : '',
    'description' => isset($instance->description) ? $instance->description : '',
    'view_mode' => isset($instance->config['view_mode']) ? $instance->config['view_mode'] : '',
    'json_config' => isset($instance->config['json_config']) ? $instance->config['json_config'] : '',
  );
  edge_suite_instance_instance_form_fields('global', $form, $instance_form_values);

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save instance'),
  );

  return $form;
}

/**
 * Common instance form elements for field and global instances.
 *
 * @param string $mode
 *   Global or field.
 * @param string $form
 *   Form element.
 * @param array $values
 *   Values for the fields.
 */
function edge_suite_instance_instance_form_fields($mode, &$form, $values = NULL) {

  if ($values === NULL) {
    $values = array(
      'label' => '',
      'description' => '',
      'view_mode' => '',
      'json_config' => '',
    );
  }

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#maxlength' => 64,
    '#description' => t('The label for the instant, internal use only.'),
    '#default_value' => isset($values['label']) ? $values['label'] : '',
    '#access' => isset($values['label']),
    '#required' => ($mode == 'global'),
  );

  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#maxlength' => 255,
    '#description' => t('Short description of the composition to be used in views or in overlay mode.'),
    '#default_value' => isset($values['description']) ? $values['description'] : '',
    '#access' => isset($values['description']),
  );

  // Don't allow per field view mode.
  $view_modes = edge_suite_composition_view_modes(FALSE);
  $form['config']['view_mode'] = array(
    '#type' => 'select',
    '#title' => 'View mode',
    '#options' => $view_modes,
    '#description' => t('Inline embedding should be preferred (cleaner embedding). Iframe embedding should be used when experiencing any problems with the composition, especially when running multiple compositions alongside on one page.'),
    '#default_value' => isset($values['view_mode']) ? $values['view_mode'] : '',
    '#access' => isset($values['view_mode']),
  );

  $json_help = t('All keys and strings need to be double quoted. Use the token module for generic data.') . ' ';
  $json_help .= t('The JSON object will be injected into a SCRIPT tag within the scope of the stage DIV.') . ' ';
  $json_help .= t('JSON object example') . ': ' . '{"mytext": "Drupal rocks!", "title" : "[current-page:title]", "slide_delay":10}';

  $json_description = t('Use a JSON object to inject data into the Edge Animate composition, which can then easily be read through the Edge Commons library.');
  $json_description .= t('Resources') . ': ';
  $json_description .= l(t('Edge Commons'), 'http://simonwidjaja.github.io/EdgeCommons/doc/') . ' / ';
  $json_description .= l(t('Edge Docks'), 'http://edgedocks.com/edge-suite');

  $form['config']['data_injection'] = array(
    '#type' => 'fieldset',
    '#title' => t('Data injection - Edge Commons'),
    '#collapsible' => TRUE,
    '#access' => isset($values['json_config']),
    '#collapsed' => !isset($values['json_config']) || empty($values['json_config']),
    '#description' => $json_description,
  );

  $form['config']['data_injection']['json_config'] = array(
    '#title' => t('JSON object'),
    '#type' => 'textarea',
    '#default_value' => isset($values['json_config']) ? $values['json_config'] : '',
    '#access' => isset($values['json_config']),
    '#description' => $json_help,
  );

  if (module_exists('token')) {
    $form['config']['data_injection']['tokens'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('node', 'edge_suite_instance'),
      '#global_types' => TRUE,
      '#click_insert' => TRUE,
    );
  }
}


/**
 * Validation handler for edit instance form.
 */
function edge_suite_instance_form_edit_validate($form, &$form_state) {
  // Validate JSON.
  if (!empty($form_state['values']['json_config'])) {
    $json_msg = edge_suite_json_validate($form_state['values']['json_config']);
    if (!empty($json_msg)) {
      form_set_error('json_config', $json_msg);
    }
  }
}

/**
 * Submit handler for edit instance form.
 */
function edge_suite_instance_form_edit_submit($form, &$form_state) {
  // TODO: Is the use of checkplain() necessary?
  $instance = edge_suite_instance_load($form_state['values']['instance_id']);
  if ($instance != NULL) {
    $instance->label = trim(check_plain($form_state['values']['label']));
    $instance->description = trim(check_plain($form_state['values']['description']));
    $instance->config = array(
      'view_mode' => $form_state['values']['view_mode'],
      'json_config' => $form_state['values']['json_config'],
    );

    $instance->save();

    drupal_set_message(t('Instance @label was saved.', array('@label' => $instance->label)));
  }
  else {
    drupal_set_message(t('Not a valid instance id.'));
  }

  $form_state['redirect'] = 'admin/structure/edge_suite';
}

/* ADD INSTANCE FORM */
/**
 * Form handler for add instance form.
 */
function edge_suite_instance_form_add($form, &$form_state, $definition_id) {
  return edge_suite_instance_form_edit($form, $form_state, $definition_id, 0);
}

/**
 * Validation handler for add instance form.
 */
function edge_suite_instance_form_add_validate($form, &$form_state) {
  edge_suite_instance_form_edit_validate($form, $form_state);
}

/**
 * Submit handler for add instance form.
 */
function edge_suite_instance_form_add_submit($form, &$form_state) {
  // TODO: Is the use of checkplain() necessary?
  $values = array();
  $values['definition_id'] = $form_state['values']['definition_id'];
  $values['label'] = trim(check_plain($form_state['values']['label']));
  $values['description'] = trim(check_plain($form_state['values']['description']));
  $values['config'] = array(
    'view_mode' => $form_state['values']['view_mode'],
    'json_config' => $form_state['values']['json_config'],
  );

  $instance_id = edge_suite_instance_create($values);

  if ($instance_id > 0) {
    drupal_set_message(t('Instance @label was created.', array('@label' => $values['label'])));
  }
  else {
    drupal_set_message(t('Instance @label was not created.', array('@label' => $values['label'])), 'warning');
  }
  $form_state['redirect'] = 'admin/structure/edge_suite';

}

/**
 * Validation handler for the block config form.
 */
function edge_suite_json_validate($json) {

  // Check for PHP JSON errors.
  $config = json_decode($json);

  $error_msg = '';
  if (version_compare(phpversion(), '5.3') >= 0) {
    $error = '';
    switch (json_last_error()) {
      case JSON_ERROR_NONE:
        break;

      case JSON_ERROR_DEPTH:
        $error = 'Maximum stack depth exceeded';
        break;

      case JSON_ERROR_STATE_MISMATCH:
        $error = 'Underflow or the modes mismatch';
        break;

      case JSON_ERROR_CTRL_CHAR:
        $error = 'Unexpected control character found';
        break;

      case JSON_ERROR_SYNTAX:
        $error = 'Syntax error, malformed JSON';
        break;

      case JSON_ERROR_UTF8:
        $error = 'Malformed UTF-8 characters, possibly incorrectly encoded';
        break;

      default:
        $error = 'Unknown error';
        break;
    }
    if (!empty($error)) {
      $error_msg = t('Not a valid JSON object: @err', array('@err' => $error));
    }
  }
  else {
    if (!empty($json) && $config == NULL) {
      $error_msg = t('Not a valid JSON object.');
    }
  }

  return $error_msg;
}
