<?php

/**
 * @file
 * Main view template.
 *
 * Variables available:
 * - $classes_array: An array of classes determined in
 *   template_preprocess_views_view(). Default classes are:
 *     .view
 *     .view-[css_name]
 *     .view-id-[view_name]
 *     .view-display-id-[display_name]
 *     .view-dom-id-[dom_id]
 * - $classes: A string version of $classes_array for use in the class attribute
 * - $css_name: A css-safe version of the view name.
 * - $css_class: The user-specified classes names, if any
 * - $header: The view header
 * - $footer: The view footer
 * - $rows: The results of the view query, if any
 * - $empty: The empty text to display if the view is empty
 * - $pager: The pager next/prev links to display, if any
 * - $exposed: Exposed widget form/info to display
 * - $feed_icon: Feed icon to display, if any
 * - $more: A link to view more, if any
 *
 * @ingroup views_templates
 */
?>
<?php 
	$content = $view->style_plugin->rendered_fields;
	//dpm($content);
?>


<link rel="stylesheet" type="text/css" href="/css/demo_grid.css" />
		<link rel="stylesheet" type="text/css" href="/css/style_grid.css" />
		<script type="text/javascript" src="/js/modernizr.custom.26633.js"></script>
		
<div class="<?php print $classes; ?>">
  <?php print render($title_prefix); ?>
  <?php if ($title): ?>
    <?php print $title; ?>
  <?php endif; ?>
  <?php print render($title_suffix); ?>
   
    <?php if ($exposed): ?>
    <div class="view-filters clearfix">
      <?php print $exposed; ?>
  <?php endif; ?>

  <?php if ($header): ?>
    <div class="view-header clearfix">
      <?php print $header; ?>
  <?php endif; ?>


  <?php if ($attachment_before): ?>
    <div class="attachment attachment-before">
      <?php print $attachment_before; ?>
  <?php endif; ?>

  <?php if ($rows): ?>
		
  <div style="width:100%;">
     <div id="ri-grid" class="item-list ri-grid ri-grid-size-3 ri-shadow"> <img src="/images/loading.gif" class="ri-loading-image"> 
	 <ul>  
	  <?php //print $rows; 
	  		foreach($content as $item){
				echo '<li>'.$item['nothing'].'</li>';
			}
	  ?>
	  </ul>
	 </div>
</div>    
      <?php //print $rows; ?>
  <?php elseif ($empty): ?>
    <div class="view-empty">
      <?php print $empty; ?>
    </div>
  <?php endif; ?>

  <?php if ($pager): ?>
  <div class="clearfix"></div>
    <?php print $pager; ?>
	<div class="clearfix"></div>
  <?php endif; ?>

  <?php if ($attachment_after): ?>
    <div class="attachment attachment-after">
      <?php print $attachment_after; ?>
    </div>
  <?php endif; ?>

  <?php if ($more): ?>
    <?php print $more; ?>
  <?php endif; ?>

  <?php if ($footer): ?>
    <div class="view-footer">
      <?php print $footer; ?>
    </div>
  <?php endif; ?>

  <?php if ($feed_icon): ?>
    <div class="feed-icon">
      <?php print $feed_icon; ?>
    </div>
  <?php endif; ?>

</div><?php /* class view */ ?>
	
 <script type="text/javascript" src="http://ajax.googleapis.com/ajax/libs/jquery/1.8.3/jquery.min.js"></script>
		<script type="text/javascript" src="/js/jquery.gridrotator.js"></script>
		<script type="text/javascript">	
			$(function() {
			
				$( '#ri-grid' ).gridrotator( {
					// number of rows
rows			: 2,

// number of columns 
columns			: 6,

// rows/columns for different screen widths
// i.e. w768 is for screens smaller than 768 pixels
w1024			: {
	rows	: 4,
	columns	: 3
},

w768			: {
	rows	: 3,
	columns	: 4
},

w480			: {
	rows	: 2,
	columns	: 6
},

w320			: {
	rows	: 2,
	columns	: 6
},

w240			: {
	rows	: 2,
	columns	: 6
},

// step: number of items that are replaced at the same time
// random || [some number]
// note: for performance issues, the number should not be > options.maxStep
step			: 'random',
maxStep			: 3,

// prevent user to click the items
preventClick	: true,

// animation type
// showHide || fadeInOut || slideLeft || 
// slideRight || slideTop || slideBottom || 
// rotateLeft || rotateRight || rotateTop || 
// rotateBottom || scale || rotate3d || 
// rotateLeftScale || rotateRightScale || 
// rotateTopScale || rotateBottomScale || random
animType		: 'random',

// animation speed
animSpeed		: 500,

// animation easings
animEasingOut	: 'linear',
animEasingIn	: 'linear',

// the item(s) will be replaced every 3 seconds
// note: for performance issues, the time "can't" be < 300 ms
interval		: 3000,
// if false the animations will not start
// use false if onhover is true for example
slideshow		: true,
// if true the items will switch when hovered
onhover		: false,

				} );
			
			});
		</script>