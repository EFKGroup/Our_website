<?php
/**
 * @file
 * Admin pages and forms.
 */

/**
 * Settings form for the RLIS API.
 */
function instagram_api_settings_form() {
  $form = array();

  $form['instagram_client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Client ID'),
    '#default_value' => variable_get('instagram_client_id', ''),
  );

  $form['instagram_client_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Client Secret'),
    '#default_value' => variable_get('instagram_client_secret', ''),
  );

  $form['instagram_verify_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Verification Token'),
    '#default_value' => variable_get('instagram_verify_token', substr(md5(rand()), 0, 7)),
  );

  $form['instagram_sync_max'] = array(
    '#type' => 'textfield',
    '#title' => t('Sync Maximum'),
    '#description' => t('For new tags, how many old media items should the system pull in on the initial notification? WARNING: Setting this value above 200 may cause stability issues.'),
    '#default_value' => variable_get('instagram_sync_max', '100'),
  );

  return system_settings_form($form);
}

/**
 * Administrative display showing existing subscriptions and allowing CRUD.
 */
function instagram_subscriptions_overview_page() {
  $instagram_api = instagram_get_api();
  $subscriptions = $instagram_api->listSubscriptions();
  $rows = array();
  $history = variable_get('instagram_update_history', array());
  foreach ($subscriptions as $sub) {
    $delete_link = 'admin/config/services/instagram/subscriptions/delete/' . $sub['object'] . '/' . $sub['object_id'] . '/' . $sub['id'];
    $reset_link = 'admin/config/services/instagram/subscriptions/reset/' . $sub['object'] . '/' . $sub['object_id'];
    $last_sync = isset($history[$sub['object']][$sub['object_id']]) ?
      format_date($history[$sub['object']][$sub['object_id']]) : 'Never';
    $rows[] = array(
      $sub['object'],
      $sub['object_id'],
      $last_sync,
      l(t('Reset'), $reset_link) . ' | ' . l(t('Delete'), $delete_link),
    );
  }
  $table = array(
    'header' => array(
      t('Type'),
      t('Filter'),
      t('Last Sync'),
      t('Actions'),
    ),
    'rows' => $rows,
  );

  $table['caption'] = t("A list of existing subscriptions<br/><br/>
    The following Subscriptions were found:");
  return theme('table', $table);
}

/**
 * Form to create a subscription.
 */
function instagram_subcriptions_add($form, &$form_state) {
  $form['object'] = array(
    '#type' => 'select',
    '#title' => t('Filter Type'),
    '#required' => TRUE,
    '#options' => array(
      'tag' => t('Tag'),
      'user' => t('User'),
      'location' => t('Location'),
    ),
  );
  $form['object_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Filter'),
    '#required' => TRUE,
  );

  $form['pull_recent'] = array(
    '#type' => 'checkbox',
    '#title' => 'Pull Recent Images Immediately',
    '#description' => 'This option may take a moment to process.',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Subscribe'),
  );
  $form['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'admin/config/services/instagram/subscriptions',
  );
  return $form;
}

/**
 * Form processor to submit subscription requests.
 */
function instagram_subcriptions_add_submit($form, &$form_state) {
  $instagram_api = instagram_get_api();
  if ($form_state['values']['pull_recent']) {
    drupal_set_message(
      t('Triggering sync for @type "@filter"', array(
        '@type' => $form_state['values']['object'],
        '@filter' => $form_state['values']['object_id'])),
      'status',
      FALSE);
    $new_media = $instagram_api->getRecent($form_state['values']['object'], $form_state['values']['object_id']);
    module_invoke_all('instagram_process_new', $new_media);
  }
  $instagram_api->subscribe($form_state['values']['object_id'], $form_state['values']['object']);
  drupal_set_message(
    t('Instagram subscription request sent for @type "@filter"', array(
      '@type' => $form_state['values']['object'],
      '@filter' => $form_state['values']['object_id'])),
    'status',
    FALSE);
  $form_state['redirect'] = 'admin/config/services/instagram/subscriptions';
}

/**
 * Signup sync reset form.
 */
function instagram_subcriptions_reset_form($form, &$form_state, $type, $filter) {
  $form_state['sub'] = array(
    'type' => $type,
    'filter' => $filter,
  );
  $limit = variable_get('instagram_sync_max', 100);
  return confirm_form($form,
    t('Are you sure you want to reset the sync history for @type "@filter"?', array('@type' => $type, '@filter' => $filter)),
    'admin/config/services/instagram/subscriptions',
    t('Next time the system pulls images based on this filter, it will pull the last @limit images tagged this way.', array('@limit' => $limit)),
    t('Reset Sync History'));
}

/**
 * Submit handler for instagram_subcriptions_reset_form().
 */
function instagram_subcriptions_reset_form_submit($form, &$form_state) {
  $history = variable_get('instagram_update_history', array());
  unset($history[$form_state['sub']['type']][$form_state['sub']['filter']]);
  variable_set('instagram_update_history', $history);
  drupal_set_message(
    t('Instagram sync history reset for @type "@filter"', array(
      '@type' => $form_state['sub']['type'],
      '@filter' => $form_state['sub']['filter'])),
    'status',
    FALSE);
  $form_state['redirect'] = 'admin/config/services/instagram/subscriptions';
}

/**
 * Signup deletion form.
 */
function instagram_subcriptions_delete_form($form, &$form_state, $type, $filter, $id) {
  $form_state['sub'] = array(
    'type' => $type,
    'filter' => $filter,
    'id' => $id,
  );
  return confirm_form($form,
    t('Are you sure you want to cancel your subscription to @type "@filter"?', array('@type' => $type, '@filter' => $filter)),
    'admin/config/services/instagram/subscriptions',
    t('(You can always re-subscribe later.)'),
    t('Cancel Subscription'));
}

/**
 * Submit handler for instagram_subcriptions_delete_form().
 */
function instagram_subcriptions_delete_form_submit($form, &$form_state) {
  $instagram_api = instagram_get_api();
  $instagram_api->unsubscribe($form_state['sub']['id']);
  drupal_set_message(
    t('Instagram subscription deletion request sent for @type "@filter"', array(
      '@type' => $form_state['sub']['type'],
      '@filter' => $form_state['sub']['filter'])),
    'status',
    FALSE);
  $form_state['redirect'] = 'admin/config/services/instagram/subscriptions';
}
