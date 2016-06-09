------------------------
Edge Compositions
------------------------
 by Timm Jansen (timm.jansen@gmail.com) (@timmjansen)
 ("ti2m", http://drupal.org/user/73587)


Notes
--------------
 For detailed information and resources check
 - the project page (http://drupal.org/project/edge_suite)
 - Edge Docks (http://edgedocks.com/edge-suite)


Description
-----------
 This module enables you to upload, manage, configure and integrate Adobe Edge
 Animate compositons in Drupal. Edge Suite is cloesly coupled with the
 edgecommons library for Edge Animate, which is needed to use some of the more
 advanced features (e.g. data injection).


Install
-----------
 Install as any other contributed module.
 - Download Edge Suite
 - Unpack it
 - Upload it to e.g. sites/all/modules
 - Do the same for the module Entity API (called 'entity')
 - Enable the module in Drupal
 - Go to permissions and assign 'Administer Edge Suite' to the
   appropriate roles. See 'Security'.
 - Install and enable 'colorbox', 'token', 'entity_token' and 'views' for
   additional functionality.
 - Take a look at edgecommons.org


Security
-----------
 This module comes with certain security implications: A user with the given
 'Administer Edge Suite compositions' role can upload archives/OAM files to your
 server. These archives will get unpacked and all images, gifs and Javascript
 files will be copied to a separate project folder. The archive itself will be
 copied to a special source folder within the 'private' system folder, which is
 not publicly accessible.
 This is being done so the EA composition can be rebuild from source in case
 of corruption or an update of this module.
Allowing users to upload edge compositions enables them to deploy random HTML
and Javascript files on your site. This is basically like granting a user the
'Full HTML Text format' permission, which makes you vulnarable to cross-site
scripting (XSS). Conclusion: ONLY ALLOW TRUSTED USERS TO UPLOAD OAMS!


Basic Usage
-----------
 - Create a composition with Edge Animate
 - Publish it with option "Animate Deployment Package", an OAM file will be
   generated
 - Upload the OAM file under:
   "Structure->Edge Suite" (admin/structure/edge_suite)
 - The composition is now being exposed through a block and you can place it on
   the page

   For more advanced usage on fields check the project page


What it does
------------
 The module takes care of adjusting all the paths and uses shared
 libraries for all the compositions. It provides block, field and
 views support. For a full feature list check the project page.


Issues & Features
----------
In case you are running into problems or have specific feature request,
please post them at http://drupal.org/project/issues/edge_suite
