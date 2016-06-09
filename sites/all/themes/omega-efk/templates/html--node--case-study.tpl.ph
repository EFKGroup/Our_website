<?php print $doctype; ?>
<html lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>"<?php print $rdf->version . $rdf->namespaces; ?>>
<head<?php print $rdf->profile; ?>>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
   <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js"></script>
  <link rel="stylesheet" type="text/css" href="css/demo.css" />
  <link rel="stylesheet" type="text/css" href="css/style1.css" />
  <?php print $styles; ?>
  <?php print $scripts; ?>
  <!--[if lt IE 9]><script src="http://html5shiv.googlecode.com/svn/trunk/html5.js"></script><![endif]-->
</head>
<body<?php print $attributes;?>>
  <div id="skip-link">
    <a href="#main-content" class="element-invisible element-focusable"><?php print t('Skip to main content'); ?></a>
  </div>
  <?php print $page_top; ?>
  <?php print $page; ?>
  <?php print $page_bottom; ?>
</body>
<script type="text/javascript">
jQuery(window).scroll(function() {
if (jQuery(this).scrollTop() > 100) {
    jQuery('#zone-menu').stop().animate({ top: '-300px' });
} else {
    jQuery('#zone-menu').stop().animate({ top: '0px' });
}
});
</script>
<script src="//cdnjs.cloudflare.com/ajax/libs/jquery/2.1.3/jquery.min.js"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/gsap/1.17.0/TweenMax.min.js"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/ScrollMagic/2.0.5/ScrollMagic.js"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/ScrollMagic/2.0.5/plugins/animation.gsap.js"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/gsap/1.17.0/plugins/ScrollToPlugin.min.js"></script>
<script>
// Init controller
var controller = new ScrollMagic.Controller({
  globalSceneOptions: {
    duration: $('section').height(),
    triggerHook: .025,
    reverse: true
  }
});

var scene1 = new ScrollMagic.Scene({ triggerElement: '#intro' })
								.setClassToggle('#intro-anchor', 'active')
								.addTo(controller);
var scene2 = new ScrollMagic.Scene({ triggerElement: '#section-1' })
								.setClassToggle('#anchor1', 'active')
								.addTo(controller);
var scene3 = new ScrollMagic.Scene({ triggerElement: '#section-2' })
								.setClassToggle('#anchor2', 'active')
								.addTo(controller);
var scene4 = new ScrollMagic.Scene({ triggerElement: '#section-3' })
								.setClassToggle('#anchor3', 'active')
								.addTo(controller);

// Change behaviour of controller
// to animate scroll instead of jump
controller.scrollTo(function(target) {

  TweenMax.to(window, 0.5, {
    scrollTo : {
      y : target,
      autoKill : true // Allow scroll position to change outside itself
    },
    ease : Cubic.easeInOut
  });

});


//  Bind scroll to anchor links
$(document).on("click", "a[href^=#]", function(e) {
  var id = $(this).attr("href");

  if($(id).length > 0) {
    e.preventDefault();

    // trigger scroll
    controller.scrollTo(id);

    // If supported by the browser we can also update the URL
    if (window.history && window.history.pushState) {
      history.pushState("", document.title, id);
    }
  }

});
</script>
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