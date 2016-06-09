<?php
/**
 * @file
 * Edge suite views functionality.
 */

/**
 * Implements hook_field_views_data().
 */
function edge_suite_field_views_data($field) {
  $data = field_views_field_default_views_data($field);
  foreach ($data as $table_name => $table_data) {
    // Add the relationship only on the instance field, definition can be
    // referenced through the instance.
    $data[$table_name][$field['field_name'] . '_instance_id']['relationship'] = array(
      'handler' => 'views_handler_relationship',
      'base' => 'edge_suite_instance',
      'base field' => 'instance_id',
      'label' => t('Edge Instance !field_name', array('!field_name' => $field['field_name'])),
    );
  }
  return $data;
}
