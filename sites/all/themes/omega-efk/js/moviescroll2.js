        jQuery(document).ready(function($) {
            if($('body').hasClass('front')){
                // Get media - with autoplay disabled (audio or video)
                var media = $('video').not("[autoplay='autoplay']");
                var tolerancePixel = 200;

                function checkMedia(){
                    // Get current browser top and bottom
                    var scrollTop = $(window).scrollTop() + tolerancePixel;
                    var scrollBottom = $(window).scrollTop() + $(window).height() - tolerancePixel;

                    //if ($(window).scrollTop() > $(window).height() - 10) {
                    media.each(function(index, el) {
                        var yTopMedia = $(this).offset().top;
                        var yBottomMedia = $(this).height() + yTopMedia;

                        if(scrollTop < yBottomMedia && scrollBottom > yTopMedia){
                            $(this).get(0).play();
                            $(this).bind('ended', function(){     
                                this.play();  
                            });
                        } else {
                            $(this).get(0).pause();
                        }
                    });

                    //}
                }
                $(document).on('scroll', checkMedia);
            }
        });
        
        