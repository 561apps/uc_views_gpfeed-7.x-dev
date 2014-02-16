<?php

/**
 * Field handler for terms.
 */
class uc_views_gpfeed_handler_field_google_product_category extends uc_views_gpfeed_handler_field_product_type {
  function option_definition() {
    $options = parent::option_definition();
    $options['label']['default'] = 'g:google_product_category';
    return $options;
  }

  function render_item($count, $item) {
    if (!empty($item['google_taxonomy'])) {
      return $item['google_taxonomy'];
    }
  }

  /**
   * Render all items in this field together.
   *
   * When using advanced render, each possible item in the list is rendered
   * individually. Then the items are all pasted together.
   */
  function render_items($items) {
    $items = array_unique($items);
    $items = array_filter($items);
    if (!empty($items)) {
      // return first
      return $items[0];
    }
    else {
      // return information string
      return t('This is a required field which is being redered empty. Please check the fields settings and your taxonomy setup and this module\'s mappings.');
    }
  }

}

