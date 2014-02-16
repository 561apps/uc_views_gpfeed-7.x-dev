<?php
/**
 * Field handle to display an altered price but rounded following google's specification
 */
class uc_views_gpfeed_handler_field_price extends uc_product_handler_field_price {

  function option_definition() {
    $options = parent::option_definition();
    $options['label']['default'] = 'g:price';
    return $options;
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    if ($this->options['format'] != 'numeric_altered') {
      return parent::render($values);
    }

    if ($this->options['format'] == 'numeric_altered') {
      $product = node_load($values->{$this->aliases['nid']});
      $context = array(
        'revision' => 'altered',
        'type' => 'product',
        'class' => array(
          'product',
          $this->field,
        ),
        'field' => "sell_price",
        'subject' => array(
          'node' => $product,
        ),
      );
      $options = array('label' => FALSE);

      $altered_price =  uc_price($product->sell_price, $context, $options);
      $values->{$this->field_alias} = $altered_price;
      $this->options['format'] = 'numeric';
      $value = parent::render($values);
      $this->options['format'] = 'numeric_altered';
      return $value;
    }
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['format']['#options']['numeric_altered'] = t('Numeric (altered)');

    foreach (array('set_precision', 'precision', 'decimal', 'separator', 'prefix', 'suffix') as $field) {
      $form[$field]['#dependency']['radio:options[format]'][] = 'numeric_altered';
    }

    $form['revision']['#weight'] = 2;
  }
}

