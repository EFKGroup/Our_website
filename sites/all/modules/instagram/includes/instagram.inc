<?php
/**
 * @file
 * Objects, properties, and methods to communicate with the RLIS API
 */

/**
 * Ability to authorize and communicate with the Instagram API.
 */
class Instagram {

  /**
   * Constructor which initializes the API connection object.
   */
  public function __construct($client_id, $client_secret) {
    $this->client_id = $client_id;
    $this->client_secret = $client_secret;
    $this->api_url = variable_get('instagram_endpoint', 'https://api.instagram.com/');
  }

  /**
   * Determine if this connection is fully configured.
   */
  public function isAuthorized() {
    // @todo should test that the keys work
    return (!empty($this->client_id) && !empty($this->client_secret));
  }

  /**
   * Make a call to the Instagram API.
   *
   * @param array $params
   * @param string $path
   *
   * @return mixed
   *   The requested response.
   */
  public function apiCall($params, $path, $request_type = 'POST') {
    $this->response = $this->httpRequest($params, $path, $request_type);
    $clean = drupal_json_decode($this->response->data);
    if ($clean['meta']['code'] == 200) {
      return $clean;
    }
    return NULL;
  }

  /**
   * Make the HTTP request. Wrapper around drupal_http_request().
   */
  protected function httpRequest($options, $path, $request_type) {
    $params = array(
      'client_id' => $this->client_id,
      'client_secret' => $this->client_secret,
    );
    $params = array_merge($params, $options);
    $query_string = drupal_http_build_query($params);
    $data = array('method' => $request_type);
    switch ($request_type) {
      case 'POST':
        $data['data'] = $query_string;
        return drupal_http_request($this->api_url . $path, $data);

      case 'GET':
      case 'DELETE':
        return drupal_http_request($this->api_url . $path . '/?' . $query_string, $data);

    }
  }

  /**
   * Get a list of existing subscriptions.
   */
  public function listSubscriptions() {
    $result = $this->apiCall(array(), 'v1/subscriptions', 'GET');
    $data = $result['data'];
    if (empty($data['error'])) {
      return $data;
    }
    return NULL;
  }

  /**
   * Get a list of existing subscriptions.
   */
  public function subscribe($filter, $type = 'tag') {
    $vars = array(
      'object' => $type,
      'aspect' => 'media',
      'object_id' => $filter,
      'verify_token' => variable_get('instagram_verify_token'),
      'callback_url' => $GLOBALS['base_url'] . '/admin/instagram/webhook',
    );
    $result = $this->apiCall($vars, 'v1/subscriptions');
    $data = $result['data'];
    if (empty($data['error'])) {
      return $data;
    }
    return NULL;
  }

  /**
   * Get a list of existing subscriptions.
   */
  public function unsubscribe($id) {
    $vars = array(
      'id' => $id,
    );
    $result = $this->apiCall($vars, 'v1/subscriptions', 'DELETE');
    $data = $result['data'];
    if (empty($data['error'])) {
      return $data;
    }
    return NULL;
  }

  /**
   * Get a list of existing subscriptions.
   */
  public function getToken($vars) {
    $result = $this->apiCall($vars, 'oauth/access_token');
    $data = $result['data'];
    if (empty($data['error'])) {
      return $data;
    }
    return NULL;
  }

  /**
   * Get a list of existing subscriptions.
   */
  public function getRecent($type, $filter) {
    $search_history = variable_get('instagram_update_history', array());
    $last_pulled = isset($search_history[$type][$filter]) ? $search_history[$type][$filter] : 0;
    $search_history[$type][$filter] = time();
    variable_set('instagram_update_history', $search_history);
    $data = $this->getContentSince($type, $filter, $last_pulled);

    return $data;
  }

  /**
   * Recursive function to retrieve content tagged/created after a given time.
   */
  protected function getContentSince($type, $filter, $time, $offset = 0, $next_max = 0) {
    // Prevent wild api call loops for popular item types.
    $limit = variable_get('instagram_sync_max', 100);
    if ($offset > $limit) {
      return array();
    }
    // Make our API request:
    $url = 'v1/' . $type . 's/' . $filter . '/media/recent';
    $count = min(20, $limit - $offset);
    $vars = array(
      'COUNT' => $count,
    );
    if ($next_max) {
      $vars['max_tag_id'] = $next_max;
    }
    $result = $this->apiCall($vars, $url, 'GET');
    $data = $result['data'];
    // Figure out if we have all the items we want. If not, we keep collecting:
    $last_item = end($data);
    $oldest_timestamp = $type == 'tag' ? $last_item['caption']['created_time'] : $last_item['created_time'];
    reset($data);
    if (count($data) == $count && $oldest_timestamp > $time) {
      // We need more items, so we recur:
      $data = array_merge($data, $this->getContentSince($type, $filter, $time, $offset + $count, $result['pagination']['next_max_id']));
    }
    else {
      // This is the last recurrence, let's trim off any items that are too old:
      foreach ($data as $index => $item) {
        $timestamp = $type == 'tag' ? $item['caption']['created_time'] : $item['created_time'];
        if ($timestamp < $time) {
          unset($data[$index]);
        }
      }
    }
    return $data;
  }
}
