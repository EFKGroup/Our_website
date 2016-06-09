<?php print $doctype; ?>
<html lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>"<?php print $rdf->version . $rdf->namespaces; ?>>
<head<?php print $rdf->profile; ?>>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
   <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js"></script>
  
  <?php print $styles; ?>
<?php print $scripts; ?>
              <script>
             // if(!is_touch_device()){ 
               wow = new WOW(
                    {
                      mobile:       false       // default
                    });
                 wow.init();
             // }
              </script>
  
  
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
  // Custom sorting plugin
  (function($) {
    $.fn.sorted = function(customOptions) {
      var options = {
        reversed: false,
        by: function(a) { return a.text(); }
      };
      $.extend(options, customOptions);
      $data = $(this);
      arr = $data.get();
      arr.sort(function(a, b) {
          var valA = options.by($(a));
          var valB = options.by($(b));
        if (options.reversed) {
          return (valA < valB) ? 1 : (valA > valB) ? -1 : 0;				
        } else {		
          return (valA < valB) ? -1 : (valA > valB) ? 1 : 0;	
        }
      });
      return $(arr);
    };
  })(jQuery);

  // DOMContentLoaded
  $(function() {
  
    // bind radiobuttons in the form
    var $filterType = $('#filter input[name="type"]');
    var $filterSort = $('#filter input[name="sort"]');
  
    // get the first collection
    var $applications = $('#applications');
  
    // clone applications to get a second collection
    var $data = $applications.clone();

    // attempt to call Quicksand on every form change
    $filterType.add($filterSort).change(function(e) {
      if ($($filterType+':checked').val() == 'all') {
        var $filteredData = $data.find('li');
      } else {
        var $filteredData = $data.find('li[data-type=' + $($filterType+":checked").val() + ']');
      }
    
      // if sorted by size
      if ($('#filter input[name="sort"]:checked').val() == "size") {
        var $sortedData = $filteredData.sorted({
          by: function(v) {
            return parseFloat($(v).find('span[data-type=size]').text());
          }
        });
      } else {
      // if sorted by name
        var $sortedData = $filteredData.sorted({
          by: function(v) {
            return $(v).find('strong').text().toLowerCase();
          }
        });
      }   
    
      // finally, call quicksand
      $applications.quicksand($sortedData, {
        duration: 800,
        easing: 'easeInOutQuad'
      });

    });
  
  });
</script>
<script type="text/javascript">
jQuery(window).scroll(function() {
/*if (jQuery(this).scrollTop() > 100) {
    jQuery('#zone-menu').stop().animate({ top: '-300px' });
} else {
    jQuery('#zone-menu').stop().animate({ top: '0px' });
}*/
});
</script>
<script>
var header = $(".floating-menu");
$(document).scroll(function(e) {
    if($(this).scrollTop() > $("#region-case-study-one").height()) {
        header.css({"position" : "fixed", "top" : "0"});
    } else {
        header.css({"position" : "absolute", "top" : "95px"});
    }
});
</script>

<script src="js/jquery.quicksand.js" type="text/javascript"></script> 
<script src="js/jquery.quicksand.action.js" type="text/javascript"></script>

<script>
(function(e){e.fn.sorted=function(t){var n={reversed:!1,by:function(e){return e.text()}};return e.extend(n,t),$data=e(this),arr=$data.get(),arr.sort(function(t,r){var i=n.by(e(t)),s=n.by(e(r));return n.reversed?i<s?1:i>s?-1:0:i<s?-1:i>s?1:0}),e(arr)}})(jQuery),$(function(){var e=function(e){var t={selected:!1,type:0};for(var n=0;n<e.length;n++)e[n].indexOf("selected-")==0&&(t.selected=!0),e[n].indexOf("segment-")==0&&(t.segment=e[n].split("-")[1]);return t},t=function(e){var t=e.parent().filter('[class*="selected-"]');return t.find("a").attr("data-value")},n=function(e){var t=e.parent().filter('[class*="selected-"]');return t.find("a").attr("data-value")},r={duration:800,easing:"easeInOutQuad",adjustHeight:"auto",useScaling:!0},i=$("#list"),s=i.clone(),o=$("ul.splitter ul");o.each(function(u){var a=$(this),f=a.find("a");f.bind("click",function(u){var a=$(this),l=a.parent(),c=e(l.attr("class").split(" ")),h=c.selected,p=c.segment;if(!h){f.parent().removeClass("selected-0").removeClass("selected-1").removeClass("selected-2"),l.addClass("selected-"+p);var d=t(o.eq(1).find("a")),v=n(o.eq(0).find("a"));if(v=="all")var m=s.find("li");else var m=s.find("li."+v);if(d=="size")var g=m.sorted({by:function(e){return parseFloat($(e).find("span").text())}});else var g=m.sorted({by:function(e){return $(e).find("strong").text().toLowerCase()}});i.quicksand(g,r)}u.preventDefault()})});var u=!0,a=$("#performance-toggle"),f=a.html();a.find("a").live("click",function(e){u?(r.useScaling=!1,a.html('CSS3 scaling turned off. Try the demo again. <a href="#toggle">Reverse</a>.'),u=!1):(r.useScaling=!0,a.html(f),u=!0),e.preventDefault()})});
</script>

</html>