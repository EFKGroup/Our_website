(function ($) {
  Drupal.behaviors.edge_suite = {
    attach: function (context, settings) {
      console.log('das');
      $('.comp_link_view', context).click(function () {
        var instance_id = $(this).attr('data-id');
        $.ajax({
          url : settings.basePath + 'edge_suite/iframe/data/' + instance_id,
          type: 'POST',
          data : {token : true},
          success : function(data, textStatus, jqXHR){
            $.colorbox({
              href: data.url,
              iframe: true,
              innerWidth: data.width,
              innerHeight: data.height,
              scrolling: false,
              title: data.text
            });
          }
        });
        return false;
      });
    }
  };
})(jQuery);
