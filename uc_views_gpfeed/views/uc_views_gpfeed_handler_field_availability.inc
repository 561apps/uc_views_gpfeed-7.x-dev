<?php
/**
 * Field handler for stock availability.
 */
class uc_views_gpfeed_handler_field_availability extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();
    $options['label']['default'] = 'g:availability';
    return $options;
  }


  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $sku = $values->{$this->aliases['model']};
    return theme('uc_views_gpfeed_field_availability', $sku);
  }
}

