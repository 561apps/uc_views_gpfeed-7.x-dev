<?php
/**
 * @file
 * We are overriding default content multiple handler in order for
 * us to provide a proper separator to feed multiple tags of the same item to
 * the feed
 *
 */
class uc_views_gpfeed_content_handler_field_multiple extends content_handler_field_multiple {
function option_definition() {
    $options = parent::option_definition();

    $options['multiple']['contains']['multiple_type'] = array('default' => 'default');
    $options['multiple']['contains']['multiple_separator'] = array('default' => ', ');
    $options['multiple']['contains']['multiple_separator_check_plain'] = array('default' => FALSE);

    return $options;
  }

  /**
   * Provide 'group multiple values' option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['multiple']['multiple_type'] = array(
      '#type' => 'radios',
      '#title' => t('Grouping display'),
      '#options' => array(
        'default' => t('CCK default'),
        'separator' => t('Simple separator'),
      ),
      '#default_value' => $this->options['multiple']['multiple_type'],
    );

    $form['multiple']['multiple_separator'] = array(
      '#type' => 'textfield',
      '#title' => t('Separator'),
      '#default_value' => $this->options['multiple']['multiple_separator'],
      '#process' => array('views_process_dependency'),
      '#dependency' => array('radio:options[multiple][multiple_type]' => array('separator')),
    );
    $form['multiple']['multiple_separator_check_plain'] = array(
      '#type' => 'checkbox',
      '#title' => t('Check plain'),
      '#description' => t('We are providing this in order to allow us to use XML tags as separator so this multiple CCK field can be used in the feed as multiple entries. If you are planning on using this somewhere else, we recommend checking this box."'),
      '#default_value' => $this->options['multiple']['multiple_separator_check_plain'],
      '#process' => array('views_process_dependency'),
      '#dependency' => array('radio:options[multiple][multiple_type]' => array('separator')),
    );
  }

  /**
   * We are overriding render to provide separator logic
   */
  function render($values) {
    // if it's default, just do the default
    if ($this->options['multiple']['multiple_type'] == 'default') {
      return parent::render($values);
    }

    // if not, let's do ours, which is basically a copy but we handle our own
    // grouping display

    // If this is not a grouped field, use content_handler_field::render().
    if (!$this->defer_query) {
      return parent::render($values);
    }

    // We're down to a single node here, so we can retrieve the actual field
    // definition for the node type being considered.
    $field = content_fields($this->content_field['field_name'], $values->{$this->aliases['type']});

    // If the field does not appear in the node type, then we have no value
    // to display, and can just return.
    if (empty($field)) {
      return '';
    }

    $options = $this->options;

    $vid = $values->{$this->field_alias};
    if (isset($this->field_values[$vid])) {
      // Gather items, respecting the 'Display n values starting from m' settings.
      $count_skipped = 0;
      $items = array();
      foreach ($this->field_values[$vid] as $item) {
        if (empty($options['multiple']['multiple_from']) || ($count_skipped >= $options['multiple']['multiple_from'])) {
          if (empty($options['multiple']['multiple_number']) || (count($items) < $options['multiple']['multiple_number'])) {
            // Grab the nid - needed for render_link().
            $nid = $item['_nid'];
            unset($item['_nid']);
            $items[] = $item;
          }
          else {
            break;
          }
        }
        $count_skipped++;
      }

      // Build a pseudo-node from the retrieved values.
      $node = drupal_clone($values);
      // content_format and formatters will need a 'type'.
      $node->type = $values->{$this->aliases['type']};
      $node->nid = $values->{$this->aliases['nid']};
      $node->vid = $values->{$this->aliases['vid']};

      // Some formatters need to behave differently depending on the build_mode
      // (for instance: preview), so we provide one.
      $node->build_mode = NODE_BUILD_NORMAL;

      // Render items.
      $formatter_name = $options['format'];
      if ($items && ($formatter = _content_get_formatter($formatter_name, $field['type']))) {
        $rendered = array();
        if (content_handle('formatter', 'multiple values', $formatter) == CONTENT_HANDLE_CORE) {
          // Single-value formatter.
          foreach ($items as $item) {
            $output = content_format($field, $item, $formatter_name, $node);
            if (!empty($output)) {
              $rendered[] = $this->render_link($output, (object) array('nid' => $nid));
            }
          }
        }
        else {
          // Multiple values formatter.
          $output = content_format($field, $items, $formatter_name, $values);
          if (!empty($output)) {
            $rendered[] = $this->render_link($output, (object) array('nid' => $nid));
          }
        }
        if (count($rendered) > 1) {
          // TODO: could we use generic field display ?
          if ($options['multiple']['multiple_type'] == 'separator') {
            $separator = $options['multiple']['multiple_separator'];
            // allowing \n in the separator to be treated as newline
            $separator = str_replace('\n', "\n", $separator);
            if ($options['multiple']['multiple_separator_check_plain']) {
              $separator = check_plain($separator);
            }
            return implode($separator, $rendered);
          }
        }
        elseif ($rendered) {
          return $rendered[0];
        }
      }
    }

    return '';
  }
}

