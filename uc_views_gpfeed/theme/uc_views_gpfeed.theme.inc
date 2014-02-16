<?php

/**
 * @file
 *
 */

/**
 * Preprocess xml output template.
 */
function template_preprocess_uc_views_gpfeed_rss_header(&$vars) {
  // Compute the root XML node, using the base table, and appending an 's' if needed.
  $root_node = 'channel';
  $vars['root_node'] = _views_data_export_xml_tag_clean($root_node);
  $vars['rss_title'] = check_plain($vars['view']->style_options['rss_title']);
  $vars['rss_description'] = check_plain($vars['view']->style_options['rss_description']);
}

/**
 * Preprocess xml output template.
 */
function template_preprocess_uc_views_gpfeed_rss_footer(&$vars) {
  // Compute the root XML node, using the base table, and appending an 's' if needed.
  $root_node = 'channel';
  $vars['root_node'] = _views_data_export_xml_tag_clean($root_node);
}

/**
 * Preprocess xml output template.
 */
function template_preprocess_uc_views_gpfeed_rss_body(&$vars) {
  _views_data_export_header_shared_preprocess($vars);
  _views_data_export_body_shared_preprocess($vars);

  // Compute the tag name based on the views base table, minus any trailing 's'.
  $vars['item_node'] = 'item';

  foreach ($vars['themed_rows'] as $num => $row) {
    foreach ($row as $field => $content) {
      if (!empty($content)) {
        // Prevent double encoding of the ampersand. Look for the entities produced by check_plain().
        $content = preg_replace('/&(?!(amp|quot|#039|lt|gt);)/', '&amp;', $content);
        // Convert < and > to HTML entities.
        $content = str_replace(array('<', '>'), array('&lt;', '&gt;'), $content);
        // Restoring special tags that can be found on the themed row following google feed specifications
        foreach (array('g:product_type', 'g:additional_image_link') as $tag) {
          $content = str_replace("&lt;$tag&gt;", "<$tag>", $content);
          $content = str_replace("&lt;/$tag&gt;", "</$tag>", $content);
        }

        $vars['themed_rows'][$num][$field] = $content;
      }
      else {
        unset($vars['themed_rows'][$num][$field]);
      }
    }
  }

  foreach ($vars['header'] as $field => $header) {
    // If there is no field label, use 'no name'.
    $vars['xml_tag'][$field] = !empty($header) ? $header : 'no name';
    if ($vars['options']['transform']) {
      switch ($vars['options']['transform_type']) {
        case 'dash':
          $vars['xml_tag'][$field] = str_replace(' ', '-', $header);
          break;
        case 'underline':
          $vars['xml_tag'][$field] = str_replace(' ', '_', $header);
          break;
        case 'camel':
          $vars['xml_tag'][$field] = str_replace(' ', '', ucwords(strtolower($header)));
          // Convert the very first character of the string to lowercase.
          $vars['xml_tag'][$field][0] = strtolower($vars['xml_tag'][$field][0]);
          break;
        case 'pascal':
          $vars['xml_tag'][$field] = str_replace(' ', '', ucwords(strtolower($header)));
          break;
      }
    }
    // We should always try to output valid XML.
    $vars['xml_tag'][$field] = _views_data_export_xml_tag_clean($vars['xml_tag'][$field]);
  }
}

