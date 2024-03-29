<?php
/**
 * @file views-view-table.tpl.php
 * Template to display a view as a table.
 *
 * - $title : The title of this group of rows.  May be empty.
 * - $rows: An array of row items. Each row is an array of content
 *   keyed by field ID.
 * - $header: an array of headers(labels) for fields.
 * - $themed_rows: a array of rows with themed fields.
 * @ingroup views_templates
 */

// Short tags act bad below in the html so we print it here.
print '<?xml version="1.0"?>' . "\n";
print '<rss xmlns:g="http://base.google.com/ns/1.0" version="2.0">' . "\n";
?>
<<?php print $root_node; ?>>
  <title><?php print $rss_title; ?></title>
  <link><?php print url('', array('absolute' => TRUE)); ?></link>
  <description><?php print $rss_description; ?></description>
