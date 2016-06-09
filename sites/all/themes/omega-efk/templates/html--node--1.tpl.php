<?php print $doctype; ?>
<html lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>"<?php print $rdf->version . $rdf->namespaces; ?>>
<head<?php print $rdf->profile; ?>>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js"></script>
 <?php print $styles; ?>
  <?php print $scripts; ?>
  <script>
  jQuery(document).ready(function($) {  

    // site preloader -- also uncomment the div in the header and the css style for #preloader
    $(window).load(function(){
      $('#preloader').fadeOut('fast',function(){$(this).remove();});
        $("body").removeClass("noflowtwo");
    });

});
  </script>
 <script>
    wow = new WOW(
    {
      mobile:       false       
    });
    wow.init();

  </script>
</head>
<body<?php print $attributes;?>>
<!--<div id="preloader"></div>-->
  <div id="skip-link">
    <a href="#main-content" class="element-invisible element-focusable"><?php print t('Skip to main content'); ?></a>
  </div>
  <?php print $page_top; ?>
  <?php print $page; ?>
  <?php print $page_bottom; ?>
  
    <!-- Google Code for Remarketing Tag -->
<!--------------------------------------------------
Remarketing tags may not be associated with personally identifiable information or placed on pages related to sensitive categories. See more information and instructions on how to setup the tag on: http://google.com/ads/remarketingsetup
--------------------------------------------------->
<script type="text/javascript">
/* <![CDATA[ */
var google_conversion_id = 931413498;
var google_custom_params = window.google_tag_params;
var google_remarketing_only = true;
/* ]]> */
</script>
<script type="text/javascript" src="//www.googleadservices.com/pagead/conversion.js">
</script>
<noscript>
<div style="display:inline;">
<img height="1" width="1" style="border-style:none;" alt="" src="//googleads.g.doubleclick.net/pagead/viewthroughconversion/931413498/?value=0&amp;guid=ON&amp;script=0"/>
</div>
</noscript>
</body>



  

</html>