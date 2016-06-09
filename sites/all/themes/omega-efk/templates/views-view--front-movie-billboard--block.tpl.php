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
<div class="<?php print $classes; ?>">
  <?php print render($title_prefix); ?>
  <?php if ($title): ?>
    <?php print $title; ?>
  <?php endif; ?>
  <?php print render($title_suffix); ?>
  <?php if ($header): ?>
    <div class="view-header">
      <?php print $header; ?>
    </div>
  <?php endif; ?>

  <?php if ($exposed): ?>
    <div class="view-filters">
      <?php print $exposed; ?>
    </div>
  <?php endif; ?>

  <?php if ($attachment_before): ?>
    <div class="attachment attachment-before">
      <?php print $attachment_before; ?>
    </div>
  <?php endif; ?>

  <?php if ($rows): ?>
  <div class="view-content">
    <?php 
    $content = $view->style_plugin->rendered_fields;
    
        foreach($content as $item){
         // echo '<img src="sites/all/themes/omega-efk/css/images/video_images/Mobile-Hero-Landscape.jpg" style="width:100%; height:100%;">';
            echo '<video width="736" height="414"  muted="muted" loop="loop" autoplay="autoplay" poster="sites/default/files/trans-poster.png">
            <source type="video/mp4" src="'.$item['field_movie'].'"></source>
            <source type="video/webm" src="'.$item['field_web_movie'].'"></source>
            <source type="video/ogg" src="'.$item['field_ogg_movie'].'"></source>
            </video>';
        }
      
    ?>
    <script type="text/javascript">
    var min_w = 736; // minimum video width allowed
    var vid_w_orig;  // original video dimensions
    var vid_h_orig;

    jQuery(function() { // runs after DOM has loaded
        
        vid_w_orig = parseInt(jQuery('.view-front-movie-billboard video').attr('width'));
        vid_h_orig = parseInt(jQuery('.view-front-movie-billboard video').attr('height'));
        $('#debug').append("<p>DOM loaded</p>");
        
        jQuery(window).resize(function () { resizeToCover(); });
        jQuery(window).trigger('resize');
    });

    function resizeToCover() {
        
        // set the video viewport to the window size
        jQuery('.view-front-movie-billboard').width(jQuery(window).width());
        jQuery('.view-front-movie-billboard').height(jQuery(window).height());

        // use largest scale factor of horizontal/vertical
        var scale_h = jQuery(window).width() / vid_w_orig;
        var scale_v = jQuery(window).height() / vid_h_orig;
        var scale = scale_h > scale_v ? scale_h : scale_v;

        // don't allow scaled width < minimum video width
        if (scale * vid_w_orig < min_w) {scale = min_w / vid_w_orig;}

      
      
        // now scale the video
        jQuery('.view-front-movie-billboard video').width(scale * vid_w_orig);
        jQuery('.view-front-movie-billboard video').height(scale * vid_h_orig);
        // and center it by scrolling the video viewport
        jQuery('.view-front-movie-billboard').animate({marginLeft:"-"+(jQuery('.view-front-movie-billboard video').width() - jQuery(window).width()) / 2}, 0);
        jQuery('.view-front-movie-billboard').animate({marginTop:"-"+(jQuery('.view-front-movie-billboard video').height() - jQuery(window).height()) / 2}, 0);

       // alert((jQuery('.view-front-movie-billboard video').width() - jQuery(window).width()) / 2);
    };


    </script>
  </div>
  <?php elseif ($empty): ?>
    <div class="view-empty">
      <?php print $empty; ?>
    </div>
  <?php endif; ?>

  <?php if ($pager): ?>
    <?php print $pager; ?>
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


