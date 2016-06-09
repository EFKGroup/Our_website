<?php print $doctype; ?>
<html lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>"<?php print $rdf->version . $rdf->namespaces; ?>>
<head<?php print $rdf->profile; ?>>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <link href='https://fonts.googleapis.com/css?family=Lato:400,100,100italic,300,300italic,400italic,700,700italic,900,900italic' rel='stylesheet' type='text/css'>
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js"></script>
  <script>
  jQuery(document).ready(function($) {  

// site preloader -- also uncomment the div in the header and the css style for #preloader
$(window).load(function(){
	$('#preloader').fadeOut('slow',function(){$(this).remove();});
    $("body").removeClass("noflowtwo");
});

});
  </script>
  <script src="js/moviescroll2.js"></script>
  <script src="js/wow.min.js"></script>
              <script>
              new WOW().init();
              </script>
  <?php print $styles; ?>
  <link rel="stylesheet" type="text/css" href="css/demo.css" />
  <link rel="stylesheet" type="text/css" href="css/style1.css" />
  <link rel="stylesheet" href="css/bigvideo.css">
  <?php print $scripts; ?>
  <script src="js/modernizr.custom.js"></script>
  <!--[if lt IE 9]><script src="http://html5shiv.googlecode.com/svn/trunk/html5.js"></script><![endif]-->
</head>
<body<?php print $attributes;?>>
<div id="preloader"></div>
  <div id="skip-link">
    <a href="#main-content" class="element-invisible element-focusable"><?php print t('Skip to main content'); ?></a>
  </div>
  <?php print $page_top; ?>
  <?php print $page; ?>
  <?php print $page_bottom; ?>
</body>


<script src="js/classie.js"></script>
<script src="js/demo1.js"></script>

  <script type="text/javascript">
	var clicky_site_ids = clicky_site_ids || [];
	clicky_site_ids.push(100620945);
	(function() {
	  var s = document.createElement('script');
	  s.type = 'text/javascript';
	  s.async = true;
	  s.src = '//static.getclicky.com/js';
	  ( document.getElementsByTagName('head')[0] || document.getElementsByTagName('body')[0] ).appendChild( s );
	})();
	</script>
	<noscript><p><img alt="Clicky" width="1" height="1" src="//in.getclicky.com/100620945ns.gif" /></p></noscript>

</html>