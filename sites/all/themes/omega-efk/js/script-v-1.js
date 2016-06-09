jQuery(function($){

var isMobile = false; //initiate as false
// device detection
if(/(android|bb\d+|meego).+mobile|avantgo|bada\/|blackberry|blazer|compal|elaine|fennec|hiptop|iemobile|ip(hone|od)|ipad|iris|kindle|Android|Silk|lge |maemo|midp|mmp|netfront|opera m(ob|in)i|palm( os)?|phone|p(ixi|re)\/|plucker|pocket|psp|series(4|6)0|symbian|treo|up\.(browser|link)|vodafone|wap|windows (ce|phone)|xda|xiino/i.test(navigator.userAgent) 
    || /1207|6310|6590|3gso|4thp|50[1-6]i|770s|802s|a wa|abac|ac(er|oo|s\-)|ai(ko|rn)|al(av|ca|co)|amoi|an(ex|ny|yw)|aptu|ar(ch|go)|as(te|us)|attw|au(di|\-m|r |s )|avan|be(ck|ll|nq)|bi(lb|rd)|bl(ac|az)|br(e|v)w|bumb|bw\-(n|u)|c55\/|capi|ccwa|cdm\-|cell|chtm|cldc|cmd\-|co(mp|nd)|craw|da(it|ll|ng)|dbte|dc\-s|devi|dica|dmob|do(c|p)o|ds(12|\-d)|el(49|ai)|em(l2|ul)|er(ic|k0)|esl8|ez([4-7]0|os|wa|ze)|fetc|fly(\-|_)|g1 u|g560|gene|gf\-5|g\-mo|go(\.w|od)|gr(ad|un)|haie|hcit|hd\-(m|p|t)|hei\-|hi(pt|ta)|hp( i|ip)|hs\-c|ht(c(\-| |_|a|g|p|s|t)|tp)|hu(aw|tc)|i\-(20|go|ma)|i230|iac( |\-|\/)|ibro|idea|ig01|ikom|im1k|inno|ipaq|iris|ja(t|v)a|jbro|jemu|jigs|kddi|keji|kgt( |\/)|klon|kpt |kwc\-|kyo(c|k)|le(no|xi)|lg( g|\/(k|l|u)|50|54|\-[a-w])|libw|lynx|m1\-w|m3ga|m50\/|ma(te|ui|xo)|mc(01|21|ca)|m\-cr|me(rc|ri)|mi(o8|oa|ts)|mmef|mo(01|02|bi|de|do|t(\-| |o|v)|zz)|mt(50|p1|v )|mwbp|mywa|n10[0-2]|n20[2-3]|n30(0|2)|n50(0|2|5)|n7(0(0|1)|10)|ne((c|m)\-|on|tf|wf|wg|wt)|nok(6|i)|nzph|o2im|op(ti|wv)|oran|owg1|p800|pan(a|d|t)|pdxg|pg(13|\-([1-8]|c))|phil|pire|pl(ay|uc)|pn\-2|po(ck|rt|se)|prox|psio|pt\-g|qa\-a|qc(07|12|21|32|60|\-[2-7]|i\-)|qtek|r380|r600|raks|rim9|ro(ve|zo)|s55\/|sa(ge|ma|mm|ms|ny|va)|sc(01|h\-|oo|p\-)|sdk\/|se(c(\-|0|1)|47|mc|nd|ri)|sgh\-|shar|sie(\-|m)|sk\-0|sl(45|id)|sm(al|ar|b3|it|t5)|so(ft|ny)|sp(01|h\-|v\-|v )|sy(01|mb)|t2(18|50)|t6(00|10|18)|ta(gt|lk)|tcl\-|tdg\-|tel(i|m)|tim\-|t\-mo|to(pl|sh)|ts(70|m\-|m3|m5)|tx\-9|up(\.b|g1|si)|utst|v400|v750|veri|vi(rg|te)|vk(40|5[0-3]|\-v)|vm40|voda|vulc|vx(52|53|60|61|70|80|81|83|85|98)|w3c(\-| )|webc|whit|wi(g |nc|nw)|wmlb|wonu|x700|yas\-|your|zeto|zte\-/i.test(navigator.userAgent.substr(0,4))) isMobile = true;

//alert($('.not-front #region-case-study-one').height());
$(document).ready(function () {
	if($("#block-views-news-views-list-block, #block-views-careers-list-block").length > 0){							
	    size_li = $(".item-list ul li.views-row").size();
		if(4 >= size_li){
				$('#loadMore').hide();	
			}
	    x=4;
	    $('.item-list ul li.views-row:lt('+x+')').show();
	    $('#loadMore').click(function () {
	        x= (x+4 <= size_li) ? x+4 : size_li;
	        $('.item-list ul li.views-row:lt('+x+')').show();
			if(x == size_li){
				$('#loadMore').hide();	
			}
	    });
		
	}



	if(is_touch_device() && isMobile){ 
		$('body').addClass('hasTouch'); 
		$('.not-front.node-type-case-study #region-case-study-one video, .not-front.node-type-page #region-case-study-one video, .view-front-movie-billboard video').remove();
	}

	//checkMobileImage();

	});	

	function checkMobileImage(){

		if($('.mobile-image.landscape img').length > 0 && $('.mobile-image.portrait img').length < 1){
			var mobImg = $('.mobile-image.landscape img').attr('src');
			$('#zone-branding').css({'background' : "transparent url("+mobImg+") no-repeat scroll center top / cover "});
			$('#zone-branding').css('height', $(window).height());
			//$('#block-views-basic-page-block-12 .views-field-field-mobile-text-headline').hide();
			//$('#block-views-basic-page-block-12 .views-field-field-logo').show();
		}
		if($(window).width() <= 1024 && $(window).width() > 736 && $('.mobile-image.tablet img').length > 0){
			var mobImg = $('.mobile-image.tablet img').attr('src');
			$('#zone-branding').css({'background' : "transparent url("+mobImg+") no-repeat scroll center top / cover "});	
			$('#zone-branding').css('height', $(window).height());
			//$('#block-views-basic-page-block-12 .views-field-field-mobile-text-headline').hide();
			//$('#block-views-basic-page-block-12 .views-field-field-logo').show();

		}else if($(window).width() <= 736 && $(window).width() > 414 && $('.mobile-image.landscape img').length > 0){
			var mobImg = $('.mobile-image.landscape img').attr('src');
			$('#zone-branding').css({'background' : "transparent url("+mobImg+") no-repeat scroll center top / cover "});
			$('#zone-branding').css('height', $(window).height());
			//$('#block-views-basic-page-block-12 .views-field-field-mobile-text-headline').hide();
			//$('#block-views-basic-page-block-12 .views-field-field-logo').show();

		}else if($(window).width() <= 414 && $('.mobile-image.portrait img').length > 0){
			var mobImg = $('.mobile-image.portrait img').attr('src');
			$('#zone-branding').css({'background' : "transparent url("+mobImg+") no-repeat scroll center top / cover "});							 
			$('#zone-branding').css('height', $(window).height());
			//$('#block-views-basic-page-block-12 .views-field-field-mobile-text-headline').show();
			//$('#block-views-basic-page-block-12 .views-field-field-logo').hide();
			
		}

	}


	if($(window).width() <= 736){
		$('#zone-branding').css('height', $(window).height());
	}

	$(window).resize(function(){
		//checkMobileImage();		
		if($(window).width() <= 736){
		$('#zone-branding').css('height', $(window).height());
	}			  
});	





$(window).scroll(function () { 

   $('.not-front .movie-body-section, .front .flow-wrapper > .center-wrapper, .not-front #block-views-work-hero-block .views-field-title').css({
      'top' : -($(this).scrollTop()/3)+"px"
   });
   
   $('.not-front #zone-branding-wrapper #block-views-basic-page-block-12').css({
	   'margin-top' : -($(this).scrollTop()/3)+"px"
   });

   if($("html,body").scrollTop() > 10){ $( '#cboxClose' ) .fadeOut('slow');}else{ $( '#cboxClose' ) .fadeIn('slow');}


	//$('#myIframe').contents().scrollTop()



});

$(document).delegate('#cboxLoadedContent', 'scroll', function(){
	
	if($("html,body").scrollTop() > 10){ $( '#cboxClose' ) .fadeOut('slow');}else{ $( '#cboxClose' ) .fadeIn('slow');}
});


        
        divFade = $(".black-overlay");
        
        var toggleHeader = function (noAnimate) {
    
            var threshold  = 100,
                fadeLength = $('.not-front #region-case-study-one, .front #zone-branding').height(),
                opacity,
                scrollTop  = $(document).scrollTop();
				
				if($(window).width() < 737){
					
					fadeLength  = $(window).height();
				}
            
            if (scrollTop < threshold) {
                opacity = 0;
            } else if (scrollTop > threshold + fadeLength) {
                opacity = 1;
            } else {
                if (noAnimate) {
                    opacity = 1;
                } else {
                    opacity = (scrollTop - threshold) / fadeLength;
                }
            }
            //if(!is_touch_device() && !isMobile){
            	divFade.css("opacity", opacity);
        	//}
            
        };
    
        toggleHeader(true);
        $(window).scroll(function () {toggleHeader();});
    


//setTimeout(function(){ $('.not-front #zone-case-study, #zone-branding-wrapper, #big-video-wrap').css('background-color' , '#000000'); }, 1000);


window.setInterval(checkHeaderHeight, 1500);
function checkHeaderHeight(){
	if($('.not-front #region-case-study-one, .not-front #zone-branding-wrapper, .not-front #big-video-wrap').length > 0){
		var headerMargin = $('.not-front #region-case-study-one').height();
		//alert(headerMargin);
		//$('.debug').html(headerMargin);
		
		$('.not-front:not(.node-type-case-study) #region-case-study-two, .node-type-work:not(.node-type-case-study)  #region-case-study-three').css('margin-top', headerMargin );

		if($(window).width() < 737 && is_touch_device() && isMobile){
			$('.node-type-case-study.not-front .scroll-content').css('margin-top', $(window).height() );
			$('.not-front.node-type-case-study #region-case-study-two,	.node-type-work #region-case-study-three').css('margin-top', 0 );

		}else if($(window).width() < 737 && !is_touch_device() && !isMobile){
			$('.node-type-case-study.not-front .scroll-content').css('margin-top', $(window).height() );
		}else{	
			$('.node-type-case-study.not-front .scroll-content').css('margin-top', headerMargin);
		}
		
		//if(!is_touch_device() && !isMobile){
			$('.not-front #region-case-study-one').css('position', 'fixed' ).css('top', 0);
		//}
		if(is_touch_device() && isMobile && $(window).width() < 737){
			$('.not-front #region-case-study-one').css('display', 'none');
		}
	}
	



}

$('#block-views-careers-list-block .item-list > ul li.views-row .click-area').on('click', function() {
		$('#block-views-careers-list-block .item-list ul li.views-row').removeClass('active');	
		$('#block-views-careers-list-block .item-list ul li.views-row .click-area').show();
		$(this).parents('.views-row').addClass('active');
		$(this).hide();
		//$('.close-job', this).show();
		 scrollPoint = $(this).offset().top - 20;
		$('body,html').animate({
			scrollTop: scrollPoint
		}, 500);
});

function closeJob(){
	
}


$('#block-views-careers-list-block .close-job').click(function() {
$('#block-views-careers-list-block .item-list ul li.views-row').removeClass('active');	
$('#block-views-careers-list-block .item-list ul li.views-row .click-area').show();
});


$('#block-views-news-views-list-block .item-list ul li').on('click', function() {
	window.location.href = $(this).find('a').attr('href');																	   
});

$('#block-views-article-details-hero-block-1 h1, #block-views-article-details-hero-block-1 h2, #block-views-article-details-hero-block-1 h3, #block-views-article-details-hero-block-1 p, #block-views-article-details-hero-block-1 ul').attr('data-wow-duration', '.5s').attr('data-wow-offset', '20').attr('data-wow-delay', '.5s').addClass('wow fadeInLeft');

$('#block-views-article-details-hero-block-1 img').attr('data-wow-duration', '.8s').attr('data-wow-offset', '20').attr('data-wow-delay', '.5s').addClass('wow zoomIn');

$('#block-views-work-hero-block-4 h2, #block-views-work-hero-block-2 ul li .views-label-field-services-provided').attr('data-wow-duration', '.5s').attr('data-wow-offset', '20').attr('data-wow-delay', '.2s').addClass('wow fadeInLeft');
$('#block-views-work-hero-block-4 p').attr('data-wow-duration', '.5s').attr('data-wow-offset', '10').attr('data-wow-delay', '.4s').addClass('wow fadeInLeft');
$('#block-views-work-hero-block-2 ul li li').attr('data-wow-duration', '.5s').attr('data-wow-offset', '10').attr('data-wow-delay', '.4s').addClass('wow fadeInLeft');
$('#block-views-work-hero-block-3 li img').attr('data-wow-duration', '.6s').attr('data-wow-offset', '20').attr('data-wow-delay', '.3s').addClass('wow zoomIn');

if($('#block-webform-webform-client-form-20').length > 0){
	$('#block-webform-webform-client-form-20 .form-item').each(function(){
		$(this).append($(this).find('label'));
		$(this).addClass('input--chisato');
		$('input, textarea',this).addClass('input__field input__field--chisato');
		$('label',this).addClass('input__label input__label--chisato');
		$('label span span',this).remove();
		var labelText = $('label',this).text();
		var labelText2 = $('label',this).text();
		$('label',this).html('<span class="input__label-content input__label-content--chisato" data-content="'+labelText2+'">'+labelText+'</span>');
		
		if($('textarea',this).length > 0){
			$('.grippie',this).remove();
			$('textarea',this).after($('label',this));
		}
	});
	
	$('#block-webform-webform-client-form-20 .form-item input, #block-webform-webform-client-form-20 .form-item textarea').on('click keyup', function(){
		if($(this).val() !=''){
			$(this).parents('.form-item').addClass('input--filled');	
		}else{
			$(this).parents('.form-item').removeClass('input--filled');	
		}
	});
}
$('.menu a').on('click', function() {

    var scrollAnchor = $(this).attr('href'),
	scrollAnchor = scrollAnchor.replace('#','');
        scrollPoint = $('.view-case-study section[id="' + scrollAnchor + '"]').offset().top + 20;

    $('body,html').animate({
        scrollTop: scrollPoint
    }, 500);

    return false;

});

$(window).load(function() {
		$('.floating-case-study-menu').removeClass('affix');
});




var leftMenuHeight = $('.case-study-movie').outerHeight();

$(window).scroll(function() {
						  
    var windscroll = $(window).scrollTop();
    if (windscroll >= leftMenuHeight) {
        //$('nav').addClass('fixed');
        $('.region').each(function(i) {
			if($(this).find('section')){
				var activeLink = $(this).find('section').attr('id');
				if ($(this).position().top <= windscroll) {
					$('.menu a.active').removeClass('active');
					$('.menu li a[href="#'+activeLink+'"]').addClass('active');
				}
			}
        });

    } else {

        $('.menu a.active').removeClass('active');
        $('.menu a:first').addClass('active');
    }

}).scroll();

jQuery.fn.exists = function(){return this.length>0;}
var checkOverlap = function($element) {
	  		if($element.length) {
				var results = $element.overlaps("div.fluid, img.fluid, .left-image-wrap, .right-image-wrap img, .racquet-selector-image, .left-image, .region");
				
				 $element.removeClass('overlap');
			      clearTimeout($element.data('scrollTimer'));
			      
			      $element.data('scrollTimer', setTimeout(function() {
			          // do something
			          if (results.length > 0) {
						 // alert('yes overlap');
			          	//console.log('something is overlapping');
			          //	$element.addClass('overlap');
						
			          }
			          	          
			          //console.log("Haven't scrolled in 500ms!");
			      }, 500)); 
			      
			      
			      //hide for sure if over related proejcts
			      var footerOverlap = $element.overlaps("#region-case-study-thirteen, #section-footer");
			      if (footerOverlap.length > 0) {
			      	//console.log('something is overlapping');
			      	$element.addClass('footer-overlap');
			      } else {
			      	$element.removeClass('footer-overlap');
			      }
	    	}
	  	 }
	  	 
 $(window).scroll(function(event){
		  
	 /*check to see if side meno is overlapping images*/
	 var sidemenu = $(".floating-case-study-menu");
	 if(sidemenu.length > 0) {
	   //checkOverlap($(".close_share"));
	   checkOverlap(sidemenu);
	 }
	 /*end*/

  });
 
  $(".floating-case-study-menu .menu li a").attr("title","");
 
 //$(".floating-case-study-menu .menu li a").tooltip();
 
 $(".floating-case-study-menu .menu li a").click(function(){
		$(this).focusout();												   
});
 
 
 
 
 	/*
	*
	* Case study sidebar code
	*
	*/
	$(window).scroll(function() {
	
		if($('.floating-case-study-menu').length > 0) {
        	var yPos = $(window).scrollTop();
        	var offset = $('#region-case-study-two').offset();
        	var startPos = offset.top - 131;
        	var sideMenuWrapper = $('.floating-case-study-menu')
			//console.log(yPos);
			
			//console.log(offset.top + ' startpos ' + startPos);
			
          	if (yPos < startPos){
			  sideMenuWrapper.removeClass('affix');
			  //$('.case_sidebar').addClass('bs-sidebar');
            }
			else if((yPos > startPos)){
			//else if((yPos > 1117)){
				sideMenuWrapper.addClass('affix');
			}
		}
	});

				


// Hide Header on on scroll down

$("#zone-menu").autoHidingNavbar();


$(window).scroll(function(event){
   if($(window).scrollTop() > ($(window).height()/2) && $('#trigger-overlay').hasClass('is-closed')){
	   $("#zone-menu").addClass('bg');
   }else{
	   $("#zone-menu").removeClass('bg');
   }
});

/*
// Hide Header on on scroll down
if($("#block-views-careers-list-block .close-job").length > 0){
var closeJob = $("#block-views-careers-list-block .view-header").offset().top;


$(window).scroll(function(event){
   if($(window).scrollTop() > closeJob-100 && $("#block-views-careers-list-block .close-job").length > 0){
	   $("#block-views-careers-list-block .close-job").css({'top':'100px', 'position' : 'fixed'});
   }else{
	   $("#block-views-careers-list-block .close-job").css({'top':'0px', 'position' : 'absolute'});
   }
});


$(window).resize(function(){
	closeJob = $("#block-views-careers-list-block .view-header").offset().top;						  
});
		
}*/
$('.node-type-page .body-wrap-three .grid-3 h2 .fa-long-arrow-right').click(function(){
	$(this).hide().parents('.grid-3').find('ul').toggle('slow');	
	$(this).parents('.grid-3').find('.fa-close').css('display', 'block');
});

$('.node-type-page .body-wrap-three .grid-3 h2 .fa-close').click(function(){
	$(this).hide().parents('.grid-3').find('ul').toggle('slow');	
	$(this).parents('.grid-3').find('.fa-long-arrow-right').toggle();										
});

$('#block-views-careers-list-block .item-list ul li.views-row').each(function(){
	//$(this).prepend('<span class="close-job fa fa-close"></span>');																		  
	var title 		= $('.views-field-title h1',this).find('a').attr('id');
	var	emailLink   = $('.email-button', this).attr('href');
					  $('.email-button', this).attr('href', emailLink + ' ' +  title);
					  $('.copy-button', this).attr('data-href', window.location + '#' + title);
});

$('.copy-button').click(function(){
	var person = prompt("Copy to clipboard: Cmd + C", $(this).attr('data-href'));
	return false;
});

$('.email-button').click(function(){
	//var person = prompt("Copy to clipboard: Cmd + C", $(this).attr('data-href'));
	//window.location.href = $(this).attr('href')
	return false;
});


$('.close-job').click(function(){
	//$(this).parents('.views-row').removeClass('active');
});



//alert('width: ' + $(window).outerWidth() + ' height: ' + $(window).outerHeight());

var imgSrc = $('.page-node-84 #block-views-work-hero-block-3 ul li li:nth-child(1)').find('img').attr('src');
$('.page-node-84 #block-views-work-hero-block-3 ul li li:nth-child(1)').find('img').remove();
$('.page-node-84 #block-views-work-hero-block-3 ul li li:nth-child(1)').append('<div class="video-section wow zoomIn"  data-wow-duration=".5s" data-wow-offset="10" data-wow-delay=".3s"><video id="video1"  controls="true" poster="/sites/default/files/trans-poster.png" style="background: url('+imgSrc+') 50% 50% / cover no-repeat ;"><source src="/sites/default/files/The-Daily-Trip-SD.mp4" type="video/mp4"></video><img src="'+imgSrc+'" class="poster"><img src="/sites/default/files/play-icon.png" class="play-icon" style="display:block;" onclick="playvideo(\'video1\')"><img src="/sites/default/files/paus-icon.png" class="paus-icon"  onclick="pausevideo(\'video1\')"></div>');		

var imgSrc = $('.page-node-96 #block-views-work-hero-block-3 ul li li:nth-child(5)').find('img').attr('src');
$('.page-node-96 #block-views-work-hero-block-3 ul li li:nth-child(5)').find('img').remove();
$('.page-node-96 #block-views-work-hero-block-3 ul li li:nth-child(5)').append('<div class="video-section wow zoomIn"  data-wow-duration=".5s" data-wow-offset="10" data-wow-delay=".3s"><video id="video1"  controls="true" poster="/sites/default/files/trans-poster.png" style="background: url('+imgSrc+') 50% 50% / cover no-repeat ;"><source src="/sites/default/files/St-John-SD.mp4" type="video/mp4"></video><img src="'+imgSrc+'" class="poster"><img src="/sites/default/files/play-icon.png" class="play-icon" style="display:block;" onclick="playvideo(\'video1\')"><img src="/sites/default/files/paus-icon.png" class="paus-icon"  onclick="pausevideo(\'video1\')"></div>');		


var imgSrc = $('.page-node-101 #block-views-work-hero-block-3 ul li li:nth-child(1)').find('img').attr('src');
$('.page-node-101 #block-views-work-hero-block-3 ul li li:nth-child(1)').find('img').remove();
$('.page-node-101 #block-views-work-hero-block-3 ul li li:nth-child(1)').append('<div class="video-section wow zoomIn"  data-wow-duration=".5s" data-wow-offset="10" data-wow-delay=".3s"><video id="video1"  controls="true" poster="/sites/default/files/trans-poster.png" style="background: url('+imgSrc+') 50% 50% / cover no-repeat ;"><source src="/sites/default/files/Immaculata-University-Undergraduate-HD.mp4" type="video/mp4"></video><img src="'+imgSrc+'" class="poster"><img src="/sites/default/files/play-icon.png" class="play-icon" style="display:block;" onclick="playvideo(\'video1\')"><img src="/sites/default/files/paus-icon.png" class="paus-icon"  onclick="pausevideo(\'video1\')"></div>');		


var imgSrc = $('.page-node-101 #block-views-work-hero-block-3 ul li li:nth-child(2)').find('img').attr('src');
$('.page-node-101 #block-views-work-hero-block-3 ul li li:nth-child(2)').find('img').remove();
$('.page-node-101 #block-views-work-hero-block-3 ul li li:nth-child(2)').append('<div class="video-section wow zoomIn"  data-wow-duration=".5s" data-wow-offset="10" data-wow-delay=".3s"><video id="video2"  controls="true" poster="/sites/default/files/trans-poster.png" style="background: url('+imgSrc+') 50% 50% / cover no-repeat ;"><source src="/sites/default/files/Immaculata-University-Graduate-HD.mp4" type="video/mp4"></video><img src="'+imgSrc+'" class="poster"><img src="/sites/default/files/play-icon.png" class="play-icon" style="display:block;" onclick="playvideo(\'video2\')"><img src="/sites/default/files/paus-icon.png" class="paus-icon"  onclick="pausevideo(\'video2\')"></div>');		


var imgSrc = $('.page-node-101 #block-views-work-hero-block-3 ul li li:nth-child(3)').find('img').attr('src');
$('.page-node-101 #block-views-work-hero-block-3 ul li li:nth-child(3)').find('img').remove();
$('.page-node-101 #block-views-work-hero-block-3 ul li li:nth-child(3)').append('<div class="video-section wow zoomIn"  data-wow-duration=".5s" data-wow-offset="10" data-wow-delay=".3s"><video id="video3"  controls="true" poster="/sites/default/files/trans-poster.png" style="background: url('+imgSrc+') 50% 50% / cover no-repeat ;"><source src="/sites/default/files/Immaculata University Adult-HD.mp4" type="video/mp4"></video><img src="'+imgSrc+'" class="poster"><img src="/sites/default/files/play-icon.png" class="play-icon" style="display:block;" onclick="playvideo(\'video3\')"><img src="/sites/default/files/paus-icon.png" class="paus-icon"  onclick="pausevideo(\'video3\')"></div>');	

/* Rita's work page */
var imgSrc = $('.page-node-95 #block-views-work-hero-block-3 ul li li:nth-child(9)').find('img').attr('src');
$('.page-node-95 #block-views-work-hero-block-3 ul li li:nth-child(9)').find('img').remove();
$('.page-node-95 #block-views-work-hero-block-3 ul li li:nth-child(9)').append('<div class="video-section wow zoomIn"  data-wow-duration=".5s" data-wow-offset="10" data-wow-delay=".3s"><video id="video1"  controls="true" poster="/sites/default/files/trans-poster.png" style="background: url('+imgSrc+') 50% 50% / cover no-repeat ;"><source src="/sites/default/files/JilLati 15sec-SD.mp4" type="video/mp4"></video><img src="'+imgSrc+'" class="poster"><img src="/sites/default/files/play-icon.png" class="play-icon" style="display:block;" onclick="playvideo(\'video1\')"><img src="/sites/default/files/paus-icon.png" class="paus-icon"  onclick="pausevideo(\'video1\')"></div>');		


var imgSrc = $('.page-node-95 #block-views-work-hero-block-3 ul li li:nth-child(10)').find('img').attr('src');
$('.page-node-95 #block-views-work-hero-block-3 ul li li:nth-child(10)').find('img').remove();
$('.page-node-95 #block-views-work-hero-block-3 ul li li:nth-child(10)').append('<div class="video-section wow zoomIn"  data-wow-duration=".5s" data-wow-offset="10" data-wow-delay=".3s"><video id="video2"  controls="true" poster="/sites/default/files/trans-poster.png" style="background: url('+imgSrc+') 50% 50% / cover no-repeat ;"><source src="/sites/default/files/Buster-15sec-SD.mp4" type="video/mp4"></video><img src="'+imgSrc+'" class="poster"><img src="/sites/default/files/play-icon.png" class="play-icon" style="display:block;" onclick="playvideo(\'video2\')"><img src="/sites/default/files/paus-icon.png" class="paus-icon"  onclick="pausevideo(\'video2\')"></div>');		


var imgSrc = $('.page-node-95 #block-views-work-hero-block-3 ul li li:nth-child(11)').find('img').attr('src');
$('.page-node-95 #block-views-work-hero-block-3 ul li li:nth-child(11)').find('img').remove();
$('.page-node-95 #block-views-work-hero-block-3 ul li li:nth-child(11)').append('<div class="video-section wow zoomIn"  data-wow-duration=".5s" data-wow-offset="10" data-wow-delay=".3s"><video id="video3"  controls="true" poster="/sites/default/files/trans-poster.png" style="background: url('+imgSrc+') 50% 50% / cover no-repeat ;"><source src="/sites/default/files/Roadshow-30sec-SD.mp4" type="video/mp4"></video><img src="'+imgSrc+'" class="poster"><img src="/sites/default/files/play-icon.png" class="play-icon" style="display:block;" onclick="playvideo(\'video3\')"><img src="/sites/default/files/paus-icon.png" class="paus-icon"  onclick="pausevideo(\'video3\')"></div>');

var imgSrc = $('.page-node-95 #block-views-work-hero-block-3 ul li li:nth-child(12)').find('img').attr('src');
$('.page-node-95 #block-views-work-hero-block-3 ul li li:nth-child(12)').find('img').remove();
$('.page-node-95 #block-views-work-hero-block-3 ul li li:nth-child(12)').append('<div class="video-section wow zoomIn"  data-wow-duration=".5s" data-wow-offset="10" data-wow-delay=".3s"><video id="video4"  controls="true" poster="/sites/default/files/trans-poster.png" style="background: url('+imgSrc+') 50% 50% / cover no-repeat ;"><source src="/sites/default/files/Audition.mp4" type="video/mp4"></video><img src="'+imgSrc+'" class="poster"><img src="/sites/default/files/play-icon.png" class="play-icon" style="display:block;" onclick="playvideo(\'video4\')"><img src="/sites/default/files/paus-icon.png" class="paus-icon"  onclick="pausevideo(\'video4\')"></div>');


$('.play-icon').click(function(){
$(this).toggle().parents('.video-section').find('.poster').hide();
});

$('.paus-icon').click(function(){
$(this).toggleClass('hover').parents('.video-section').find('.play-icon').toggle();
});
});	



jQuery(function($) {
	$.fn.dropdown = function(opts) {
		// default configuration
		var config = $.extend({}, { 
            fadeInTime: 800,
            fadeOutTime: 800,
            interval: 5600
        }, opts);
		// main function
		function init(obj) {
            var dNewsticker = obj;
            var dFrame = dNewsticker.find('.js-frame');
            var dItem = dFrame.find('.js-item');
            var dCurrent;
            var stop = false;

            dItem.eq(0).addClass('current');
            dItem.eq(0).show();
            
            var move = setInterval(function(){
                if(!stop){
                    dCurrent = dFrame.find('.current');
                    dCurrent.fadeOut(config.fadeOutTime, function(){
                        if(dCurrent.next().length !== 0){
                            dCurrent.removeClass('current');
                            dCurrent.next().addClass('current');
                            dCurrent.next().fadeIn(config.fadeInTime);
                        }
                        else{
                            dCurrent.removeClass('current');
                            dItem.eq(0).addClass('current');
                            dItem.eq(0).fadeIn(config.fadeInTime);
                        }
                    });
                }
            }, config.interval);
            
            dNewsticker.on('mouseover mouseout', function(e){
                if(e.type == 'mouseover'){
                    stop = true;
                }
                else{
                    stop = false;
                }
            });
        }
		// initialize every element
		this.each(function() {
			init($(this));
		});
		return this;
	};
	// start
	$(function() {
		$('.js-newsticker').dropdown();
	});
});


		

 // This code helps to shuffle the li ...
(function($) {
       $.fn.shuffle = function() {
         var elements = this.get()
         var copy = [].concat(elements)
         var shuffled = []
         var placeholders = []
         // Shuffle the element array
         while (copy.length) {
           var rand = Math.floor(Math.random() * copy.length)
           var element = copy.splice(rand,1)[0]
           shuffled.push(element)
         }

         // replace all elements with a plcaceholder
         for (var i = 0; i < elements.length; i++) {
           var placeholder = document.createTextNode('')
           findAndReplace(elements[i], placeholder)
           placeholders.push(placeholder)
         }

         // replace the placeholders with the shuffled elements
         for (var i = 0; i < elements.length; i++) {
           findAndReplace(placeholders[i], shuffled[i])
         }

         return $(shuffled)
       }

       function findAndReplace(find, replace) {
         find.parentNode.replaceChild(replace, find)
       }

       })(jQuery);

       // I am displying the 6 elements currently rest elements are hide.

       function listsort(){
       jQuery('.listify_widget_recent_listings ul.job_listings').each(function(index){
         jQuery(this).find('li').shuffle();
         jQuery(this).find('li').each(function(index){
           jQuery(this).show();
           if(index>=6){
             jQuery(this).hide();
           }
         });
       });
       }
       // first time call to function ...
       listsort();
       // calling the function after the 15seconds.. 
       window.setInterval(function(){
         listsort();
         /// call your function here 5 seconds.
       }, 15000);    

//if(jQuery('body').hasClass('page-node-18')){	
window.setInterval(check_colorbox, 500);
//}

function check_colorbox(){
	if(jQuery('#colorbox').hasClass('cboxSlideshow_off') && jQuery('#colorbox').length > 0 && jQuery('body').hasClass('page-node-18')){
		//if(!jQuery('body').addClass('noflow'){
			jQuery('body').addClass('noflow');
		//}
	}else{
		//if(jQuery('body').addClass('noflow'){
			jQuery('body').removeClass('noflow');
		//}
	}
}



function playvideo(x) { 
var myVideo = document.getElementById(x); 
        myVideo.play();   
} 

function pausevideo(x) { 
var myVideo = document.getElementById(x); 
        myVideo.pause();   
} 

function is_touch_device() {
 return (('ontouchstart' in window)
      || (navigator.MaxTouchPoints > 0)
      || (navigator.msMaxTouchPoints > 0));
}


		


