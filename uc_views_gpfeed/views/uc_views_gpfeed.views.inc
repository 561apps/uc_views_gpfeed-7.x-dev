<?php

/**
 * Implementation of hook_views_data().
 */
function uc_views_gpfeed_views_data() {
  $data['uc_views_gpfeed']['table']['group'] = t('Google products feed');
  $data['uc_views_gpfeed']['table']['join'] = array(
    '#global' => array(),
  );

  $data['uc_views_gpfeed']['uc_views_gpfeed_google_product_category'] = array(
    'field' => array(
      'title' => t('Google product category'),
      'help' => t('Based on "All terms" from the taxonomy module. Display <b>only one</b> taxonomy term of the ones associated with a node from specified vocabularies with their configured google taxonomy mappings, specially formatted following google products feed specification. You should probably limit this field to sepecific vocabularies. If multiple terms are present, the first one (with no specific order) will be used.'),
      'handler' => 'uc_views_gpfeed_handler_field_google_product_category',
      'skip base' => 'term_data',
    ),
  );

  $data['uc_views_gpfeed']['uc_views_gpfeed_product_type'] = array(
    'field' => array(
      'title' => t('Product type'),
      'help' => t('Based on "All terms" from the taxonomy module. Display all taxonomy terms associated with a node from specified vocabularies with their configured google taxonomy mappings, specially formatted following google products feed specification.'),
      'handler' => 'uc_views_gpfeed_handler_field_product_type',
      'skip base' => 'term_data',
    ),
  );

  $data['uc_views_gpfeed']['uc_views_gpfeed_availability'] = array(
    'field' => array(
      'title' => t('Availability'),
      'help' => t('Shows whether the main SKU is "in stock" or "out of stock".'),
      'handler' => 'uc_views_gpfeed_handler_field_availability',
      'additional fields' => array(
        'model' => array(
          'table' => 'uc_products',
          'field' => 'model',
        ),
      ),
    ),
  );

  $data['uc_views_gpfeed']['uc_views_gpfeed_price'] = array(
    'field' => array(
      'title' => t('Price'),
      'help' => t('Display product price with numeric support for altered price - i.e. included of tax. This is not something you could currently do with the default price field handler of Ubercart.'),
      'handler' => 'uc_views_gpfeed_handler_field_price',
      'float' => TRUE,
      'additional fields' => array(
        'nid' => array(
          'table' => 'node',
          'field' => 'nid',
        ),
        'sell_price' => array(
          'table' => 'uc_products',
          'field' => 'sell_price',
        ),
      ),
    ),
  );

  return $data;
}

/**
 * Implementation of hook_views_handlers().
 */
function uc_views_gpfeed_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'uc_views_gpfeed') . '/views',
    ),
    'handlers' => array(
      'uc_views_gpfeed_handler_field_google_product_category' => array( 'parent' => 'uc_views_gpfeed_handler_field_product_type',),
      'uc_views_gpfeed_handler_field_product_type' => array( 'parent' => 'views_handler_field_term_node_tid',),
      'uc_views_gpfeed_content_handler_field_multiple' => array('parent' => 'content_handler_field_multiple'),
      'uc_views_gpfeed_handler_field_availability' => array('parent' => 'views_handler_field'),
      'uc_views_gpfeed_handler_field_price' => array('parent' => 'uc_product_handler_field_price'),
    ),
  );
}

/**
 * Implementation of hook_views_plugins().
 */
function uc_views_gpfeed_views_plugins() {
  $path = drupal_get_path('module', 'uc_views_gpfeed');

  $style_defaults = array(
    'path' => $path . '/views',
    'parent' => 'views_data_export',
    'theme' => 'views_data_export',
    'theme path' => $path . '/theme',
    'theme file' => 'uc_views_gpfeed.theme.inc',
    'uses row plugin' => FALSE,
    'uses fields' => TRUE,
    'uses options' => TRUE,
    'type' => 'data_export',
  );

  return array(
    'style' => array(
      'uc_views_gpfeed' => array(
        'title' => t('Google product feed file'),
        'help' => t('Display the view as a xml file.'),
        'handler' => 'uc_views_gpfeed_plugin_style_export_rss',
        'export headers' => array('Content-Type: text/xml'),
        'export feed type' => 'google_feed',
        'export feed text' => 'Google products feed',
        'export feed file' => '%view.xml',
        'export feed icon' => drupal_get_path('module', 'views_data_export') . '/images/xml.png',
        'additional themes' => array(
          'uc_views_gpfeed_rss_header' => 'style',
          'uc_views_gpfeed_rss_body' => 'style',
          'uc_views_gpfeed_rss_footer' => 'style',
        ),
        'additional themes base' => 'uc_views_gpfeed_rss',
      ) + $style_defaults,
    ),
  );
}

/**
 * Implementation of hook_views_data_alter().
 */
function uc_views_gpfeed_views_data_alter(&$data) {
  foreach ($data as $key => $data_info) {
    foreach ($data_info as $key_field => $field) {
      if ($field['field']['handler'] == 'content_handler_field_multiple') {
        $data[$key][$key_field]['field']['handler'] = 'uc_views_gpfeed_content_handler_field_multiple';
      }
    }
  }
}

