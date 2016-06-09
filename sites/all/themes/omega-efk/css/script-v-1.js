jQuery(function($){
				
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





checkMobileImage();

});	

function checkMobileImage(){
	
if($('.mobile-image.landscape img').length > 0){
	var mobImg = $('.mobile-image.landscape img').attr('src');
	$('#zone-branding').css({'background' : "transparent url("+mobImg+") no-repeat scroll center top / cover "});
	$('#zone-branding').css('height', $(window).height());
	$('#block-views-basic-page-block-12 .views-field-field-mobile-text-headline').hide();
	$('#block-views-basic-page-block-12 .views-field-field-logo').show();
}
if($(window).width() <= 1024 && $(window).width() > 736 && $('.mobile-image.tablet img').length > 0){
	var mobImg = $('.mobile-image.tablet img').attr('src');
	$('#zone-branding').css({'background' : "transparent url("+mobImg+") no-repeat scroll center top / cover "});	
	$('#zone-branding').css('height', $(window).height());
	$('#block-views-basic-page-block-12 .views-field-field-mobile-text-headline').hide();
	$('#block-views-basic-page-block-12 .views-field-field-logo').show();

}else if($(window).width() <= 736 && $(window).width() > 414 && $('.mobile-image.landscape img').length > 0){
	var mobImg = $('.mobile-image.landscape img').attr('src');
	$('#zone-branding').css({'background' : "transparent url("+mobImg+") no-repeat scroll center top / cover "});
	$('#zone-branding').css('height', $(window).height());
	$('#block-views-basic-page-block-12 .views-field-field-mobile-text-headline').hide();
	$('#block-views-basic-page-block-12 .views-field-field-logo').show();

}else if($(window).width() <= 414 && $('.mobile-image.portrait img').length > 0){
	var mobImg = $('.mobile-image.portrait img').attr('src');
	$('#zone-branding').css({'background' : "transparent url("+mobImg+") no-repeat scroll center top / cover "});							 
	$('#zone-branding').css('height', $(window).height());
	$('#block-views-basic-page-block-12 .views-field-field-mobile-text-headline').show();
	$('#block-views-basic-page-block-12 .views-field-field-logo').hide();

}



if($(window).width() <= 1024 && $(window).width() > 768 && $('.page-node-21 .body-wrap.expertise').length > 0){
	var expertiseHeight = $('.page-node-21 .body-wrap.expertise .grid-6 img').height();
	$('.page-node-21 .body-wrap.expertise .grid-6').each(function(){
		if($(this).height() > expertiseHeight){
			expertiseHeight = $(this).height();	
		}
		$(this).parents('.expertise').find('.grid-6').css('height', expertiseHeight);
	});
}else{
	$('.page-node-21 .body-wrap.expertise .grid-6').css('height', 'auto');
}

if($('.not-front #region-case-study-one, .not-front #zone-branding-wrapper, .not-front #big-video-wrap').length > 0){
	var headerMargin = $('.not-front #region-case-study-one').height();
	//alert(headerMargin);
	$('.debug').html(headerMargin);
}


//alert($('.mobile-image.landscape img').attr('src'));
targetHeight = target.outerHeight();
targetHeight2 = $(window).height();
targetHeight3 = target3.outerHeight();
targetHeight4 = target4.outerHeight();

//alert(targetHeight);
//alert(targetHeight2);
}


$(window).resize(function(){
	checkMobileImage();					  
});	

var target = $('#block-views-basic-page-block, #block-views-work-hero-block');
if(target.length > 0){
var targetHeight = target.outerHeight();

$(window).scroll(function(e){
    var scrollPercent = (targetHeight - window.scrollY) / targetHeight;
	
    if(scrollPercent >= 0){
        target.css('opacity', scrollPercent);
    }
});

}



var target2 = $('#zone-branding');
if(target2.length > 0){
var targetHeight2 = $(window).height();

$(window).scroll(function(e){
    var scrollPercent2 = (targetHeight2 - window.scrollY) / targetHeight2;
	
    if(scrollPercent2 >= 0){
        $('#zone-branding, #big-video-vid').css('opacity', scrollPercent2);
    }
});

}

var target3 = $('#block-views-work-hero-block .views-field-title h1');
if(target3.length > 0){
var targetHeight3 = target3.outerHeight();

$(window).scroll(function(e){
    var scrollPercent3 = (targetHeight3 - window.scrollY) / targetHeight3;
	
    if(scrollPercent3 >= 0){
        target3.css('opacity', scrollPercent3);
    }
});

}

var target4 = $('.case-study-movie .movie-body-section .logo, .case-study-movie .movie-body-section .logo,  #zone-branding-wrapper .movie-body-section .logo');
if(target4.length > 0){
var targetHeight4 = target4.outerHeight();

$(window).scroll(function(e){
    var scrollPercent4 = (targetHeight4 - window.scrollY) / targetHeight4;
	
    if(scrollPercent4 >= 0){
        target4.css('opacity', scrollPercent4);
    }
});

}



$('#block-views-careers-list-block .item-list > ul li.views-row').on('click', function() {
	if($(this).hasClass('active')){
		return false;
	}else{
		$('#block-views-careers-list-block .item-list ul li.views-row').removeClass('active');		
		$(this).addClass('active');
		$('#block-views-careers-list-block .close-job').show();
		 scrollPoint = $(this).offset().top - 20;
		$('body,html').animate({
			scrollTop: scrollPoint
		}, 500);
	}
});


$('#block-views-careers-list-block .close-job').click(function() {
	$('#block-views-careers-list-block .item-list ul li.views-row').removeClass('active');
	$(this).hide();
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
   if($(window).scrollTop() > 120 && $('#trigger-overlay').hasClass('is-closed')){
	   $("#zone-menu").addClass('bg');
   }else{
	   $("#zone-menu").removeClass('bg');
   }
});


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
		
}
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



/*
jQuery(function($) {
				//custom animations to use
				//in the transitions
				var animations		= ['right','left','top','bottom','rightFade','leftFade','topFade','bottomFade'];
				var total_anim		= animations.length;
				//just change this to one of your choice
				var easeType		= 'swing';
				//the speed of each transition
				var animSpeed		= 450;
				//caching
				var $hs_container	= $('#hs_container');
				var $hs_areas		= $hs_container.find('.hs_area');
				
				//first preload all images
                $hs_images          = $hs_container.find('img');
                var total_images    = $hs_images.length;
                var cnt             = 0;
                $hs_images.each(function(){
                    var $this = $(this);
                    $('<img/>').load(function(){
                        ++cnt;
                        if(cnt == total_images){
							$hs_areas.each(function(){
								var $area 		= $(this);
								//when the mouse enters the area we animate the current
								//image (random animation from array animations),
								//so that the next one gets visible.
								//"over" is a flag indicating if we can animate 
								//an area or not (we don't want 2 animations 
								//at the same time for each area)
								$area.data('over',true).bind('mouseenter',function(){
									if($area.data('over')){
										$area.data('over',false);
										//how many images in this area?
										var total		= $area.children().length;
										//visible image
										var $current 	= $area.find('img:visible');
										//index of visible image
										var idx_current = $current.index();
										//the next image that's going to be displayed.
										//either the next one, or the first one if the current is the last
										var $next		= (idx_current == total-1) ? $area.children(':first') : $current.next();
										//show next one (not yet visible)
										$next.show();
										//get a random animation
										var anim		= animations[Math.floor(Math.random()*total_anim)];
										switch(anim){
											//current slides out from the right
											case 'right':
												$current.animate({
													'left':$current.width()+'px'
												},
												animSpeed,
												easeType,
												function(){
													$current.hide().css({
														'z-index'	: '1',
														'left'		: '0px'
													});
													$next.css('z-index','9999');
													$area.data('over',true);
												});
												break;
											//current slides out from the left
											case 'left':
												$current.animate({
													'left':-$current.width()+'px'
												},
												animSpeed,
												easeType,
												function(){
													$current.hide().css({
														'z-index'	: '1',
														'left'		: '0px'
													});
													$next.css('z-index','9999');
													$area.data('over',true);
												});
												break;
											//current slides out from the top	
											case 'top':
												$current.animate({
													'top':-$current.height()+'px'
												},
												animSpeed,
												easeType,
												function(){
													$current.hide().css({
														'z-index'	: '1',
														'top'		: '0px'
													});
													$next.css('z-index','9999');
													$area.data('over',true);
												});
												break;
											//current slides out from the bottom	
											case 'bottom':
												$current.animate({
													'top':$current.height()+'px'
												},
												animSpeed,
												easeType,
												function(){
													$current.hide().css({
														'z-index'	: '1',
														'top'		: '0px'
													});
													$next.css('z-index','9999');
													$area.data('over',true);
												});
												break;
											//current slides out from the right	and fades out
											case 'rightFade':
												$current.animate({
													'left':$current.width()+'px',
													'opacity':'0'
												},
												animSpeed,
												easeType,
												function(){
													$current.hide().css({
														'z-index'	: '1',
														'left'		: '0px',
														'opacity'	: '1'
													});
													$next.css('z-index','9999');
													$area.data('over',true);
												});
												break;
											//current slides out from the left and fades out	
											case 'leftFade':
												$current.animate({
													'left':-$current.width()+'px','opacity':'0'
												},
												animSpeed,
												easeType,
												function(){
													$current.hide().css({
														'z-index'	: '1',
														'left'		: '0px',
														'opacity'	: '1'
													});
													$next.css('z-index','9999');
													$area.data('over',true);
												});
												break;
											//current slides out from the top and fades out	
											case 'topFade':
												$current.animate({
													'top':-$current.height()+'px',
													'opacity':'0'
												},
												animSpeed,
												easeType,
												function(){
													$current.hide().css({
														'z-index'	: '1',
														'top'		: '0px',
														'opacity'	: '1'
													});
													$next.css('z-index','9999');
													$area.data('over',true);
												});
												break;
											//current slides out from the bottom and fades out	
											case 'bottomFade':
												$current.animate({
													'top':$current.height()+'px',
													'opacity':'0'
												},
												animSpeed,
												easeType,
												function(){
													$current.hide().css({
														'z-index'	: '1',
														'top'		: '0px',
														'opacity'	: '1'
													});
													$next.css('z-index','9999');
													$area.data('over',true);
												});
												break;		
											default:
												$current.animate({
													'left':-$current.width()+'px'
												},
												animSpeed,
												easeType,
												function(){
													$current.hide().css({
														'z-index'	: '1',
														'left'		: '0px'
													});
													$next.css('z-index','9999');
													$area.data('over',true);
												});
												break;
										}	
									}
								});
							});
							
							//when clicking the hs_container all areas get slided
							//(just for fun...you would probably want to enter the site
							//or something similar)
							$hs_container.bind('click',function(){
								$hs_areas.trigger('mouseenter');
							});
						}
					}).attr('src',$this.attr('src'));
				});			
				

            });
	*/