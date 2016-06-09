<?php
/**
 * @file
 * Edge suite field functionality.
 */

/**
 * Implements hook_field_info().
 */
function edge_suite_field_info() {
  return array(
    'edge_instance' => array(
      'label' => t('Edge Animate Instance'),
      'description' => t('An Edge Animate Composition'),
      'settings' => array(
        'uri_scheme' => 'private',
      ),
      'instance_settings' => array(
        'file_extensions' => 'oam',
        'file_directory' => '',
        'max_filesize' => '',
        'composition' => array(
          'view_mode' => 1,
          'label' => 1,
        ),
      ),
      'default_widget' => 'edge_instance',
      'default_formatter' => 'edge_suite_view_stage',
    ),
  );
}

/**
 * Implements hook_field_instance_settings_form().
 */
function edge_suite_field_instance_settings_form($field, $instance) {
  $settings = $instance['settings'];

  if (!file_stream_wrapper_valid_scheme("private")) {
    drupal_set_message(t('Private file directory needs to be <a href="@config">configured</a> to upload compositions.', array('@config' => url('admin/config/media/file-system'))), 'warning');
  }

  // Use the file field instance settings form as base.
  $form = file_field_instance_settings_form($field, $instance);

  // Remove the description option.
  unset($form['description_field']);

  // Remove file extensions.
  unset($form['file_extensions']);

  // Overwrite file_directory description.
  $dir_description = 'Optional subdirectory within the <b>PRIVATE</b> upload destination where files will be stored. ';
  $dir_description .= 'Do not include preceding or trailing slashes. Private filesystem needs to be configured. ';
  $dir_description .= 'Changes will only affect new uploads.';
  $form['file_directory']['#description'] = t($dir_description);

  $form['composition'] = array(
    '#type' => 'fieldset',
    '#title' => t('Composition data settings'),
  );

  $form['composition']['view_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('View mode'),
    '#default_value' => isset($settings['composition']['view_mode']) ? $settings['composition']['view_mode'] : 0,
    '#description' => t('Allow view mode (inline/iframe) selection for each instance.'),
  );

  $form['composition']['label'] = array(
    '#type' => 'checkbox',
    '#title' => t('Label'),
    '#default_value' => isset($settings['composition']['label']) ? $settings['composition']['label'] : 0,
    '#description' => t('Enable composition label.'),
  );

  $form['composition']['description'] = array(
    '#type' => 'checkbox',
    '#title' => t('Description'),
    '#default_value' => isset($settings['composition']['description']) ? $settings['composition']['description'] : 0,
    '#description' => t('Enable description.'),
  );

  $form['composition']['data_injection'] = array(
    '#type' => 'checkbox',
    '#title' => t('Data injection'),
    '#default_value' => isset($settings['composition']['data_injection']) ? $settings['composition']['data_injection'] : 0,
    '#description' => t('Enable data injection.'),
  );

  return $form;
}

/**
 * Implements hook_field_load().
 */
function edge_suite_field_load($entity_type, $entities, $field, $instances, $langcode, &$items, $age) {

  // Todo future: load multiple.
  foreach ($entities as $id => $entity) {
    foreach ($items[$id] as $delta => $item) {
      // If the instance id does not exist, mark the entire item as empty.
      if (!empty($item['instance_id'])) {
        $instance = edge_suite_instance_load($item['instance_id']);
        $wrapper = entity_metadata_wrapper('edge_suite_instance', $instance);
        if ($instance != NULL) {
          $items[$id][$delta]['instance'] = $instance;
          // Todo.
          $items[$id][$delta]['fid'] = $wrapper->definition->fid->value();
        }
        else {
          $items[$id][$delta] = NULL;
        }
      }
      else {
        $items[$id][$delta] = NULL;
      }
    }
  }
}

/**
 * Implements hook_field_presave().
 */
function edge_suite_field_presave($entity_type, $entity, $field, $field_instance, $langcode, &$items) {

  foreach ($items as &$item) {
    if (!isset($item['instance_id'])) {
      // Todo future: error handling.
      $instance_values = array(
        'config' => array(),
      );

      if (isset($item['label']) && !empty($item['label'])) {
        $instance_values['label'] = trim(check_plain($item['label']));
      }

      if (isset($item['description']) && !empty($item['description'])) {
        $instance_values['description'] = trim(check_plain($item['description']));
      }

      if (isset($item['json_config']) && !empty($item['json_config'])) {
        $instance_values['config']['json_config'] = $item['json_config'];
      }

      if (isset($item['view_mode'])) {
        $instance_values['config']['view_mode'] = $item['view_mode'];
      }

      $instance_values['field_id'] = $field_instance['id'];
      // Nid not set yet for new nodes.
      if (isset($entity->nid)) {
        $instance_values['context_entity_id'] = $entity->nid;
      }

      $definition_id = edge_suite_definition_create($item['fid'], 'field', $instance_values);
      if (!$definition_id || !isset($instance_values['instance_id'])) {
        throw new Exception('Error occurred while creating the composition.');
      }
      $item['instance_id'] = $instance_values['instance_id'];

      // Delete tmp saved poster image. After composition creation the poster
      // will be taken out of the project dir. Temporary poster image would get
      // deleted anyway as it's a managed file without permanent status.
      // Todo refactor: To complicated to have a managed file and delete it
      // manually. Do just one, probably move to unmanaged files... No, uploaded
      // and directly removed OAMs will keep posters?
      if (isset($item['poster_uri'])) {
        $query = new EntityFieldQuery();
        $query->entityCondition('entity_type', 'file')
              ->propertyCondition('uri', $item['poster_uri'])
              ->propertyCondition('status', 0);

        $files = $query->execute();
        if (isset($files['file'])) {
          $fid = array_pop(array_keys($files['file']));
          $file = file_load($fid);
          file_delete($file);
        }
      }
    }
    // Update instance.
    else {
      $instance = edge_suite_instance_load($item['instance_id']);
      if ($instance != NULL) {

        // Set values.
        if (isset($item['label'])) {
          $instance->label = trim(check_plain($item['label']));
        }

        if (isset($item['description'])) {
          $instance->description = trim(check_plain($item['description']));
        }

        $config = $instance->config;
        if (isset($item['config']['data_injection']['json_config'])) {
          $config['json_config'] = $item['config']['data_injection']['json_config'];
        }

        if (isset($item['config']['view_mode'])) {
          $config['view_mode'] = $item['config']['view_mode'];
        }

        $instance->field_id = $field_instance['id'];
        if (isset($entity->nid)) {
          $instance->context_entity_id = $entity->nid;
        }

        $instance->config = $config;

        $instance->save();

      }

    }
  }
}


/**
 * Implements hook_field_insert().
 */
function edge_suite_field_insert($entity_type, $entity, $field, $instance, $langcode, &$items) {
  // After a new node has been save add the nid to the instance.
  if (isset($entity->nid)) {
    foreach ($items as &$item) {
      if (isset($item['instance_id'])) {
        $instance = edge_suite_instance_load($item['instance_id']);
        $instance->context_entity_id = $entity->nid;
        $instance->save();
      }
    }
  }
}

/**
 * Implements hook_field_update().
 */
function edge_suite_field_update($entity_type, $entity, $field, $instance, $langcode, &$items) {
  // Collect submitted instances.
  $current_instance_ids = array();
  foreach ($items as $item) {
    $current_instance_ids[] = $item['instance_id'];
  }

  // Compare the original field values with the ones that are being saved.
  $original = $entity->original;
  if (!empty($original->{$field['field_name']}[$langcode])) {
    foreach ($original->{$field['field_name']}[$langcode] as $original_item) {
      if (isset($original_item['instance_id']) && !in_array($original_item['instance_id'], $current_instance_ids)) {
        edge_suite_instance_delete($original_item['instance_id'], TRUE);
      }
    }
  }
}

/**
 * Implements hook_field_delete().
 */
function edge_suite_field_delete($entity_type, $entity, $field, $instance, $langcode, &$items) {
  // Delete all instances and related definitions.
  foreach ($items as $delta => $item) {
    if (isset($item['instance_id'])) {
      edge_suite_instance_delete($item['instance_id'], TRUE);
    }
    // Todo future: error handling?
  }
}

/**
 * Implements hook_field_delete_revision().
 */
function edge_suite_field_delete_revision($entity_type, $entity, $field, $instance, $langcode, &$items) {
  // Todo future: How to handle revisions? New instances for every revision?
}

/**
 * Implements hook_field_is_empty().
 */
function edge_suite_field_is_empty($item, $field) {
  return file_field_is_empty($item, $field);
}


/**
 * Implements hook_field_widget_info().
 */
function edge_suite_field_widget_info() {
  return array(
    'edge_instance' => array(
      'label' => t('OAM File upload'),
      'field types' => array('edge_instance'),
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
        'default value' => FIELD_BEHAVIOR_NONE,
      ),
    ),
  );
}

/**
 * Implements hook_field_widget_settings_form().
 */
function edge_suite_field_widget_settings_form($field, $instance) {
  // Use the file widget settings form.
  $form = file_field_widget_settings_form($field, $instance);
  return $form;
}


/**
 * Implements hook_field_widget_form().
 */
function edge_suite_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {

  // Add display_field setting to field because file_field_widget_form()
  // assumes it is set.
  $field['settings']['display_field'] = 0;

  $elements = file_field_widget_form($form, $form_state, $field, $instance, $langcode, $items, $delta, $element);

  foreach (element_children($elements) as $delta) {
    $elements[$delta]['#upload_validators']['file_validate_extensions'][0] = implode(' ', array('oam'));

    if ($elements[$delta]['#upload_location'] == 'private://') {
      $elements[$delta]['#upload_location'] .= 'edge_suite';
    }

    // Add all extra functionality provided by the image widget.
    $elements[$delta]['#process'][] = 'edge_suite_field_widget_process';
    $elements[$delta]['#element_validate'][] = 'edge_suite_field_widget_validate';
  }

  if ($field['cardinality'] == 1) {
    // If there's only one field, return it as delta 0.
    if (empty($elements[0]['#default_value']['fid'])) {
      $elements[0]['#description'] = theme('file_upload_help', array('description' => $instance['description'], 'upload_validators' => $elements[0]['#upload_validators']));
    }
  }
  else {
    $elements['#file_upload_description'] = theme('file_upload_help', array('upload_validators' => $elements[0]['#upload_validators']));
  }
  return $elements;
}

/**
 * An element #validate callback for edge_suite field type.
 */
function edge_suite_field_widget_validate($element, &$form_state) {
  // Validate json config.
  if (isset($element['config']['data_injection']['json_config']['#value']) && !empty($element['config']['data_injection']['json_config']['#value'])) {
    $json_element = $element['config']['data_injection']['json_config'];
    $error = edge_suite_json_validate($json_element['#value']);
    if (!empty($error)) {
      form_error($json_element, $error);
    }
  }
}

/**
 * An element #process callback for edge_suite field type.
 */
function edge_suite_field_widget_process($element, &$form_state, $form) {
  $item = $element['#value'];
  $item['fid'] = $element['fid']['#value'];

  $widget_instance = field_widget_instance($element, $form_state);
  $settings = $widget_instance['settings'];

  $element['#theme'] = 'edge_suite_widget';
  $element['#attached']['css'][] = drupal_get_path('module', 'edge_suite') . '/includes/edge_suite_admin.css';

  // If it is an empty widget return here, for clearer code structure.
  // Existing, validated or new OAM/instance.
  if (!isset($item['instance_id']) && !isset($item['instance_info']) && empty($item['fid'])) {
    return $element;
  }

  // If info is set in values it means the widget has been processed before.
  if (!isset($item['instance_info'])) {
    // Existing instance.
    if (isset($item['instance_id']) && !empty($item['instance_id'])) {

      // Load instance.
      $instance = edge_suite_instance_load($item['instance_id']);

      // Instance info.
      $item['instance_info'] = $instance->definition('project_name') . ' - ' . $instance->definition('width', 0) . ' / ' . $instance->definition('height', 0);

      if ($instance->instance('label')) {
        $item['label'] = $instance->instance('label');
      }

      if ($instance->instance('description')) {
        $item['description'] = $instance->instance('description');
      }

      if ($instance->config('view_mode')) {
        $item['view_mode'] = $instance->config('view_mode');
      }

      if ($instance->config('json_config')) {
        $item['json_config'] = $instance->config('json_config');
      }

      // Poster.
      if ($instance->definition('poster_uri_path', NULL) != NULL) {
        $item['poster_uri'] = $instance->definition('poster_uri_path');
      }
    }
    // New OAM.
    elseif (!empty($item['fid'])) {

      // Todo future: Set notifications in widget, not with drupal_set_message.
      // Validate OAM.
      $data = edge_suite_definition_validate($item['fid']);

      // Basic info.
      $item['instance_info'] = $data['project_name'] . ' - ' . $data['dimensions']['width'] . ' / ' . $data['dimensions']['height'];

      // Poster.
      if (isset($data['poster_uri'])) {
        $item['poster_uri'] = $data['poster_uri'];
      }
    }
  }

  // Put the form together.
  // Instance id, if composition exists.
  if (isset($item['instance_id']) && !empty($item['instance_id'])) {
    $element['instance_id'] = array(
      '#type' => 'hidden',
      '#value' => $item['instance_id'],
    );
  }

  // Instance info to display in the widget.
  if (isset($item['instance_info'])) {
    $element['instance_info'] = array(
      '#type' => 'hidden',
      '#value' => $item['instance_info'],
    );

    $element['instance_markup'] = array(
      '#markup' => '<span>' . $item['instance_info'] . '</span>',
    );
  }

  // Poster preview.
  if (isset($item['poster_uri']) && !empty($item['poster_uri']) && file_exists($item['poster_uri'])) {
    // Store for usage.
    $element['poster_uri'] = array(
      '#type' => 'hidden',
      '#value' => $item['poster_uri'],
    );

    // Put together style variables.
    // Todo: cache?
    $img_meta = image_get_info($item['poster_uri']);
    $variables = array(
      'style_name' => 'thumbnail',
      'path' => $item['poster_uri'],
      'width' => isset($img_meta['width']) ? $img_meta['width'] : NULL,
      'height' => isset($img_meta['height']) ? $img_meta['height'] : NULL,
      'attributes' => array('class' => array('poster')),
    );

    $element['poster'] = array(
      '#type' => 'markup',
      '#markup' => theme('image_style', $variables),
    );
  }

  $instance_form_values = array();

  if (isset($settings['composition']['label']) && !empty($settings['composition']['label'])) {
    $instance_form_values['label'] = isset($item['label']) ? $item['label'] : '';
  }
  if (isset($settings['composition']['description']) && !empty($settings['composition']['description'])) {
    $instance_form_values['description'] = isset($item['description']) ? $item['description'] : '';
  }
  if (isset($settings['composition']['view_mode']) && !empty($settings['composition']['view_mode'])) {
    $instance_form_values['view_mode'] = isset($item['view_mode']) ? $item['view_mode'] : '';
  }
  if (isset($settings['composition']['data_injection']) && !empty($settings['composition']['data_injection'])) {
    $instance_form_values['json_config'] = isset($item['json_config']) ? $item['json_config'] : '';
  }

  // Todo: move form function from admin.inc?
  require_once drupal_get_path('module', 'edge_suite') . '/includes/edge_suite.admin.inc';
  edge_suite_instance_instance_form_fields('field', $element, $instance_form_values);

  return $element;
}

/**
 * Returns HTML for the field widget.
 *
 * @param array $variables
 *   An associative array containing:
 *   - element: A render element representing the edge suite field widget.
 *
 * @return string
 *   The rendered HTML.
 */
function theme_edge_suite_widget($variables) {
  $element = $variables['element'];
  $output = '';
  $output .= '<div class="edge-suite-widget form-managed-file clearfix">';

  $output .= '<div class="edge-suite-meta clearfix">';
  if (isset($element['poster'])) {
    $output .= drupal_render($element['poster']);
  }
  if ($element['fid']['#value'] != 0) {
    $output .= drupal_render($element['filename']);
    $output .= ' <span class="file-size">(' . format_size($element['#file']->filesize) . ')</span> - ';
  }
  $output .= drupal_render($element['instance_markup']);
  $output .= drupal_render($element['remove_button']);
  $output .= '</div>';

  $output .= '<div class="edge-suite-widget-data">';
  $output .= drupal_render_children($element);
  $output .= '</div>';
  $output .= '</div>';

  return $output;
}


/**
 * Implements hook_field_formatter_info().
 */
function edge_suite_field_formatter_info() {
  $formatter = array();

  $view_modes = edge_suite_composition_view_modes(TRUE);

  foreach ($view_modes as $key => $label) {
    $formatter['edge_suite_view_' . $key] = array(
      'label' => $label,
      'field types' => array('edge_instance'),
    );
  }

  if (isset($formatter['edge_suite_view_stage'])) {
    $formatter['edge_suite_view_stage']['settings'] = array(
      'stage_formatter' => '',
    );
  }

  if (isset($formatter['edge_suite_view_poster'])) {
    $formatter['edge_suite_view_poster']['settings'] = array(
      'poster_image_style' => '',
      'poster_trigger' => '',
    );
  }
  if (isset($formatter['edge_suite_view_colorbox'])) {
    $formatter['edge_suite_view_colorbox']['settings'] = array(
      'trigger_label' => '',
    );
  }

  return $formatter;
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function edge_suite_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  if ($display['type'] == 'edge_suite_view_stage') {
    $element['stage_formatter'] = array(
      '#title' => t('Stage view mode'),
      '#type' => 'select',
      '#default_value' => $settings['stage_formatter'],
      '#empty_option' => t('Default (per field)'),
      '#options' => edge_suite_composition_view_modes(FALSE),
      '#description' => t('Select a specific view mode to force rendering in this mode.'),
    );
  }
  elseif ($display['type'] == 'edge_suite_view_poster') {
    $image_styles = image_style_options(FALSE);
    $element['poster_image_style'] = array(
      '#title' => t('Poster image style'),
      '#type' => 'select',
      '#default_value' => $settings['poster_image_style'],
      '#empty_option' => t('None (original image)'),
      '#options' => $image_styles,
      '#description' => t('Image style to use for the poster.'),
    );

    if (module_exists('colorbox')) {
      $trigger_types = array(
        'colorbox' => t('Colorbox'),
      );
      $element['poster_trigger'] = array(
        '#title' => t('Trigger'),
        '#type' => 'select',
        '#default_value' => $settings['poster_trigger'],
        '#empty_option' => t('Nothing'),
        '#options' => $trigger_types,
      );
    }
  }
  elseif ($display['type'] == 'edge_suite_view_colorbox') {
    $element['trigger_label'] = array(
      '#title' => t('Trigger label'),
      '#type' => 'textfield',
      '#default_value' => $settings['trigger_label'],
      '#empty_option' => t('Default label'),
      '#description' => 'Label to use for the colorbox link that will trigger the overlay.',
    );

    if (module_exists('token')) {
      $element['trigger_token'] = array(
        '#type' => 'fieldset',
        '#title' => t('Replacement patterns'),
        '#theme' => 'token_tree',
        '#token_types' => array('node', 'edge_suite_instance'),
        '#global_types' => TRUE,
      );
    }
    else {
      $element['trigger_token'] = array(
        '#type' => 'fieldset',
        '#title' => t('Replacement patterns'),
        '#description' => t('For token support the <a href="@token_url">token module</a> must be installed.', array('@token_url' => 'http://drupal.org/project/token')),
      );
    }
  }
  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function edge_suite_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $summary = array();
  if ($display['type'] == 'edge_suite_view_stage') {
    if (isset($settings['stage_formatter']) && !empty($settings['stage_formatter'])) {
      $summary[] = $settings['stage_formatter'];
    }
    else {
      $summary[] = t('Default: per field');;
    }
  }
  elseif ($display['type'] == 'edge_suite_view_poster') {
    $image_styles = image_style_options(FALSE);
    // Unset possible 'No defined styles' option.
    unset($image_styles['']);
    // Styles could be lost because of enabled/disabled modules that define
    // their styles in code.
    if (isset($settings['poster_image_style']) && isset($image_styles[$settings['poster_image_style']])) {
      $summary[] = t('Image style: @style', array('@style' => $image_styles[$settings['poster_image_style']]));
    }
    else {
      $summary[] = t('Original');
    }

    if (isset($settings['poster_trigger']) && !empty($settings['poster_trigger'])) {
      $summary[] = t('Trigger: @trigger', array('@trigger' => $settings['poster_trigger']));
    }
  }
  elseif ($display['type'] == 'edge_suite_view_colorbox') {
    if (isset($settings['trigger_label']) && !empty($settings['trigger_label'])) {
      $summary[] = t('Label: custom');;
    }
    else {
      $summary[] = t('Label: default');;
    }
  }

  return implode('<br />', $summary);
}


/**
 * Implements hook_field_formatter_view().
 */
function edge_suite_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  $view_modes = edge_suite_composition_view_modes(TRUE);
  $view_mode_key = substr($display['type'], strlen('edge_suite_view_'));
  if (array_key_exists($view_mode_key, $view_modes)) {
    foreach ($items as $delta => $item) {
      if ($item['instance']) {
        $item['instance']->setNodeTokenContext($entity);
        // Todo urgent: needs to be added for non field calls.
        $item['instance']->field = $instance;
        $item['instance']->display_settings = $display['settings'];
        $item['instance']->display_mode = $display['type'];
        $element[$delta] = $item['instance']->view($view_mode_key);
      }
    }
  }
  return $element;
}
