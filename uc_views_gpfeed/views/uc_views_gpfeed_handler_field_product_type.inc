<?php

/**
 * Field handler for terms.
 */
class uc_views_gpfeed_handler_field_product_type extends views_handler_field_term_node_tid {
  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_taxonomy'] = array('default' => FALSE);
    $options['label']['default'] = 'g:product_type';
    return $options;
  }

  /**
   * Provide "link to term" option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    unset($form['link_to_taxonomy']);
    unset($form['type']);
    unset($form['separator']);
    $form['limit']['#description'] = t('Only the product\'s vocabularies can be filtered here.');

    $types = uc_product_types();
    $vocabularies = array();
    foreach ($types as $type) {
      $vocabularies += taxonomy_get_vocabularies($type);
    }

    foreach($form['vids']['#options'] as $key => $value) {
      if (!in_array($key, array_keys($vocabularies))) {
        unset($form['vids']['#options'][$key]);
      }
    }
  }

  function pre_render($values) {
    parent::pre_render($values);
    $this->field_alias = $this->aliases['vid'];
    $vids = array();
    foreach ($values as $result) {
      if (!empty($result->{$this->aliases['vid']})) {
        $vids[] = $result->{$this->aliases['vid']};
      }
    }

    if ($vids) {
      $voc = '';
      if (!empty($this->options['limit']) && !empty($this->options['vids'])) {
        $voc = " AND td.vid IN (" . implode(', ', array_keys(array_filter($this->options['vids']))) . ")";
      }

      $result = db_query("SELECT tn.vid AS node_vid, td.*, v.name as vocabulary FROM {term_data} td INNER JOIN {term_node} tn ON td.tid = tn.tid INNER JOIN {vocabulary} v ON v.vid = td.vid WHERE tn.vid IN (" . implode(', ', $vids) . ")$voc ORDER BY td.weight, td.name");

      while ($term = db_fetch_object($result)) {
        $this->items[$term->node_vid][$term->tid]['google_taxonomy'] = uc_views_gpfeed_taxonomy_mapping($term->tid);
      }
    }
  }

  function render_item($count, $item) {
    if (!empty($item['google_taxonomy'])) {
      return $item['google_taxonomy'];
    }
    else {
      return $item['name'];
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
    if (!empty($items)) {
      return implode("</g:product_type>\n    <g:product_type>", $items);
    }
  }
}

