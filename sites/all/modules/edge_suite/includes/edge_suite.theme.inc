<?php
/**
 * @file
 * Edge suite theming.
 */


/***************************** THEME FUNCTIONS *****************************/

/**
 * Implements preprocess function for theme hook edge_suite_view_inline.
 */
function template_preprocess_edge_suite_view_inline(&$variables) {
  $instance = $variables['instance'];

  $global_js = &drupal_static('edge_suite_global_js', array());

  // Preloader file isn't needed for runtimes >= 5.x. (but can exist).
  $preload_url = $instance->definition->getProjectPath() . '/' . $instance->definition('project_name') . '_edgePreload.js';

  // Register composition.
  $global_js[] = array(
    'composition_id' => $instance->definition('composition_id'),
    'project_name' => $instance->definition('project_name'),
    'runtime_version' => $instance->definition('version', '0.0.0'),
    'project_path' => $instance->definition->getProjectPath(TRUE),
    // TODO: file_exists isn't performant.
    'preloader' => file_exists($preload_url) ? $preload_url : NULL,
  );

  // Add needed variables for HTML rendering.
  // Build the configuration string.
  $json_data = "";
  if ($json = $instance->getJsonConfig()) {
    $json_data = '<!-- Injected Data Begin -->' . "\n";
    $json_data .= '<script class="data" type="text/data">' . "\n";
    $json_data .= $json . "\n";
    $json_data .= '</script>' . "\n";
    $json_data .= '<!-- Injected Data End -->' . "\n";
  }
  $variables['json_data'] = $json_data;

  // Custom stage id generated by edge suite, can't be changed.
  $variables['stage_id'] = 'Stage_' . $instance->definition('project_name_unique') . '_' . $instance->instance_id;

  // Add dimension if it was possible to parse them. This avoids jumping of the
  // empty divs when using multiple instances on one page.
  $variables['height_css'] = ($instance->definition('height', NULL) != NULL) ? 'height:' . $instance->definition('height') . ';' : '';
  $variables['width_css'] = ($instance->definition('width', NULL) != NULL) ? 'width:' . $instance->definition('width') . ';' : '';

}

/**
 * View mode theme function. Main composition rendering function.
 */
function theme_edge_suite_view_inline($vars) {
  $instance = $vars['instance'];

  return '<div id="' . $vars['stage_id'] . '" style="' . $vars['width_css'] . $vars['height_css'] . '" class="' . $instance->definition('composition_id') . '">' . $vars['json_data'] . '</div>';
}


/**
 * Implements preprocess function for theme hook edge_suite_view_iframe.
 */
function template_preprocess_edge_suite_view_iframe(&$variables) {
  $instance = $variables['instance'];

  $variables['iframe_url'] = edge_suite_get_iframe_url($instance);
  $variables['width'] = $instance->definition('width', 0);
  $variables['height'] = $instance->definition('height', 0);
}

/**
 * View mode theme function. iframe rendering.
 */
function theme_edge_suite_view_iframe($vars) {
  $instance = $vars['instance'];
  return '<iframe src="' . $vars['iframe_url'] . '" name="composition-' . $instance->instance_id . '" width="' . $vars['width'] . '" height="' . $vars['height'] . '" scrolling="no" marginheight="0" marginwidth="0" frameborder="0"></iframe> ';
}


/**
 * Implements preprocess function for theme hook edge_suite_view_colorbox.
 */
function template_preprocess_edge_suite_view_colorbox(&$variables) {
  $instance = $variables['instance'];

  if (isset($variables['trigger_label'])) {
    $label = $variables['trigger_label'];
  }

  // Todo: this could be cached?
  if (!isset($label) && isset($instance->display_settings['trigger_label']) && !empty($instance->display_settings['trigger_label'])) {
    $label = $instance->display_settings['trigger_label'];
    $node = NULL;
    if (isset($instance->context_entity_id)) {
      $node = node_load($instance->context_entity_id);
    }
    elseif ($instance->definition('type') == 'global') {
      $node = menu_get_object();
    }
    $label = token_replace($label, array('node' => $node, 'edge_suite_instance' => $instance));
  }

  if (!isset($label)) {
    $label = t('View Overlay');
  }

  $variables['trigger_label'] = $label;

  drupal_add_js(drupal_get_path('module', 'edge_suite') . '/js/colorbox.js');
}

/**
 * View mode theme function. Colorbox overlay rendering.
 */
function theme_edge_suite_view_colorbox($vars) {
  $instance = $vars['instance'];
  return '<a href="" class="comp_link_view" data-id="' . $instance->instance_id . '">' . $vars['trigger_label'] . '</a>';
}


/**
 * Implements preprocess function for theme hook edge_suite_view_poster.
 */
function template_preprocess_edge_suite_view_poster(&$vars) {
  $instance = $vars['instance'];

  $path = $instance->definition('poster_uri_path');

  $vars['poster'] = '';
  if (!empty($path)) {
    if (isset($instance->display_settings['poster_image_style'])) {
      $image_style = $instance->display_settings['poster_image_style'];
      $path = image_style_url($image_style, $path);
    }

    $vars['poster'] = theme('image', array(
      'path' => $path,
      'title' => $instance->label,
      'alt' => $instance->description,
    ));

    if (isset($instance->display_settings['poster_trigger']) && $instance->display_settings['poster_trigger'] == 'colorbox' && module_exists('colorbox')) {
      $vars['trigger_label'] = $vars['poster'];
      $vars['poster_trigger'] = theme('edge_suite_view_colorbox', $vars);
    }
  }
}

/**
 * View mode theme function. Poster.
 */
function theme_edge_suite_view_poster($vars) {
  return isset($vars['poster_trigger']) ? $vars['poster_trigger'] : $vars['poster'];
}

/**
 * Returns HTML for a rendered edge suite field instance.
 *
 * @param array $variables
 *   An associative array containing:
 *   - item: An array of image data.
 *   - image_style: An optional image style.
 *   - path: An array containing the link 'path' and link 'options'.
 *
 * @return string
 *   Rendered HTML.
 *
 * @ingroup themeable
 */
function theme_edge_suite_instance($variables) {
  $instance = $variables['elements']['edge_suite_instance'];
  $classes = array('edge-suite-instance');

  if (isset($variables['elements']['#view_mode'])) {
    $classes[] = 'edge-suite-' . $variables['elements']['#view_mode'];
  }

  return '<div class="' . implode(' ', $classes) . '">' . render($instance) . '</div>';
}


/**
 * Renders a full HTML page for the given stage content.
 *
 * @param string $stage_content
 *   The rendered stage content.
 * @param string $javascript
 *   Header Javascript.
 *
 * @return string
 *   Returns the full HTML document, ready to be printed.
 */
function edge_suite_render_full_page($stage_content, $javascript) {
  $xua = '';
  if (variable_get('edge_suite_xua_compatible', TRUE)) {
    $xua = '<meta http-equiv="X-UA-Compatible" content="IE=edge"/>';
  }

  $html = '<!DOCTYPE html>' . "\n";
  $html .= '<html>' . "\n";
  $html .= '<head>' . "\n";
  $html .= $xua . "\n";
  $html .= $javascript . "\n";
  $html .= '</head>' . "\n";
  $html .= '<body style="margin:0px;">' . "\n";
  $html .= $stage_content . "\n";
  $html .= '<footer>' . "\n";
  $html .= '</footer>' . "\n";
  $html .= '</body>' . "\n";
  $html .= '</html>' . "\n";

  return $html;
}
