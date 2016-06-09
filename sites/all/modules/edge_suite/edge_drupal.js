/**
 * @file
 * Functionality to bridge Edge Animate and Drupal.
 *
 */

// Global Edge variable
// The custom object pathPrefix.libs and pathPrefix.comps should be set by Drupal
// - pathPrefix.libs: contains the path to the general lib folder
// - pathPrefix.comps: an array keyed by the edge composition id, which contains
// the project specific path
window.AdobeEdge = window.AdobeEdge || {};

/**
 * Alters the DOM for each symbol within a composition.
 *
 * Gets injected in the main _edge.js file during the composition build phase.
 * @param compId
 *   Edge composition id
 * @param symbols
 * @param fonts
 * @param resources
 * @param opts
 * @param registerCompositionDefn
 */
AdobeEdge.alterRegisterCompositionDefn = function (compId, symbols, fonts, resources, opts, registerCompositionDefn) {
  // Check if one of the know patterns for the stage id can be found.
  var stage_name = "";
  var states = "";
  // Legacy support for not-minified files.
  if (symbols.stage.states != undefined) {
    states = symbols.stage.states;
  }
  else {
    if (symbols.stage.s != undefined) {
      states = symbols.stage.s;
    }
  }

  if (states["Base State"]["${_Stage}"]) {
    stage_name = '_Stage';
  }
  else {
    // Legacy support.
    if (states["Base State"]["${_stage}"]) {
      stage_name = '_stage';
    }
  }

  if (stage_name != "") {
    // Get the original definition for the stage.
    var stage_src = states["Base State"]["${" + stage_name + "}"];
    delete states["Base State"]["${" + stage_name + "}"];

    // Inject the stage definition for all instances of a composition.
    var stages = jQuery("." + compId);
    for (var i = 0; i < stages.length; i++) {
      var stage_id = jQuery(stages[i]).attr('id');
      states["Base State"]["${_" + stage_id + "}"] = stage_src;
    }
  }

  // Alter DOMs in all symbols.
  for (var key in symbols) {
    var dom = null;
    // Legacy support for not-minified version.
    if (symbols[key].content != undefined && symbols[key].content.dom != undefined) {
      dom = symbols[key].content.dom;
    }
    // Minified version.
    else {
      if (symbols[key].cn != undefined && symbols[key].cn.dom != undefined) {
        dom = symbols[key].cn.dom;
      }
    }
    if (dom != null) {
      AdobeEdge.alterDomPaths(dom, compId);
    }
  }

  // CSS-Font support.
  var project_path = AdobeEdge.pathPrefix.comps[compId];
  for (var font_key in fonts) {
    var font = fonts[font_key];
    fonts[font_key] = font.replace(/href="([a-z0-9_-]*.css)"/g, 'href="' + project_path + '\/$1"');
  }

  // Legacy opts parameter.
  if (registerCompositionDefn.length === 4){
    registerCompositionDefn(compId, symbols, fonts, resources);
  }
  else {
    registerCompositionDefn(compId, symbols, fonts, resources, opts);
  }

}

/**
 * Alters the given DOM (e.g. preContent, dlDontent or a symbol DOM)
 *
 * All media/asset files get a project specific path prefix. The function call
 * gets directly injected into the _edgePreload.js file and will be called
 * from alterSymbols() in the main _edge.js file.
 * @param dom
 *   The edge dom
 * @param compId
 *   Edge composition id
 */
AdobeEdge.alterDomPaths = function (dom, compId) {
  // Iterate over the (flat) edge dom which contains all the assets.
  for (var key in dom) {
    // Asset files are (for now) all included through the fill property.
    var fillProp = null;

    // Not minified version.
    if (dom[key].hasOwnProperty('fill')) {
      fillProp = dom[key].fill;
    }

    // Minified version of fill.
    if (dom[key].hasOwnProperty('f')) {
      fillProp = dom[key].f;
    }

    // If the asset has one of the allowed file extensions, then add the project
    // specific prefix path (taken from the custom Drupal object in AdobeEdge).
    if (fillProp != null && fillProp.length > 1 && fillProp[1] != null && typeof fillProp[1] == 'string' && fillProp[1].match(/\.(js|png|jpg|svg|gif)$/)) {
      fillProp[1] = AdobeEdge.pathPrefix.comps[compId] + '/' + fillProp[1];
    }

    // Handle audio files.
    if (dom[key].hasOwnProperty('t') && dom[key].hasOwnProperty('sr') && dom[key].t === 'audio') {
      var audio_sources = dom[key].sr;
      for (var a = 0; a < audio_sources.length; a++) {
        if(audio_sources[a].match(/\.(mp3)$/)) {
          audio_sources[a] = AdobeEdge.pathPrefix.comps[compId] + '/' + audio_sources[a];
        }
      }
    }

    // Check nested containers which are structured as a DOM as well
    if (dom[key].hasOwnProperty('c')) {
      AdobeEdge.alterDomPaths(dom[key].c, compId);
    }
  }
};

/**
 * Alter the preloader resources.
 *
 * During the build process (edge_composition builder) the normal loadResource
 * function in the preloader.js file gets overwritten with this custom function
 * call. It allows to first alter the paths to the resource that should be
 * preloaded before the original loadResource function, which now gets called
 * after the modifications are done.
 * @param compId
 *   Edge composition id
 * @param aLoader
 *   Preloader array, which contains the objects that have to be preloaded
 * @param doDelayLoad
 *   Original doDelayLoad variable
 * @param loadResources
 *   Original loader function
 */
AdobeEdge.alterPreloadPaths = function (compId, aLoader, doDelayLoad, loadResources) {
  // Iterate over loader objects.
  for (var key in aLoader) {
    var obj = aLoader[key];
    // Check the properties of the object for JS file names.
    for (var prop in obj) {
      if (typeof(obj[prop]) == 'string') {
        if (!obj[prop].match(/^(?:https?:)?\/\//) && obj[prop].substr(obj[prop].length - 3) === ".js") {
          // If the file is a general edge library, add the lib path prefix.
          if (obj[prop].substr(0, "edge_includes/".length) === "edge_includes/") {
            // Todo: Alternative jQuery path, not in use yet.
            if (AdobeEdge.edgesuiteJqueryPathAlternative && obj[prop].match(/edge_includes\/jquery-\d(\.\d)*\.min\.js/g)) {
              obj[prop] = AdobeEdge.edgesuiteJqueryPathAlternative;
            }
            else {
              obj[prop] = AdobeEdge.pathPrefix.libs + "/" + obj[prop];
            }
          }
          // Otherwise add the project specific path prefix.
          else {
            obj[prop] = AdobeEdge.pathPrefix.comps[compId] + "/" + obj[prop];
          }
        }
      }
    }
  }

  // Check if other jQuery version exist prior to the preloader run, if
  // otherjQuery is set to true and noConflict mode is enabled then the old
  // jQuery version will be restored after the preloader has finished and
  // has passed its own jQuery version through global variable 'jQuery' to the
  // Edge runtime.

  if (typeof jQuery != 'undefined') {
    if (!AdobeEdge.otherjQuery) {
      AdobeEdge.otherjQuery = true;
      AdobeEdge.edgesuiteLogger('Other jQuery include exists: Version ' + jQuery().jquery);
    }
  }
  else {
    AdobeEdge.edgesuiteLogger('No other jQuery exists yet.');
  }

  // Call the original loader with the modified aLoader object.
  loadResources(aLoader, doDelayLoad);
}

// jQuery noConflict mode: Specific jQuery Edge Animate version.
window.jQueryEdge = window.jQueryEdge || null;

/**
 * Alter composition launch.
 *
 * This call is injected during the build process, it enables Edge Suite to
 * perform certain processing steps before a composition is being loaded. For
 * now only jQuery noConflict mode is being handled.
 */
AdobeEdge.alterOkToLaunchComposition = function (comp) {
  // Check if other jQuery version existed, if noConflict mode is enabled and
  // if it got already reverted.
  if (AdobeEdge.otherjQuery && AdobeEdge.edgesuiteNoConflict && jQueryEdge == null) {
    // Revert old 'pre' Edge jQuery version and save Edge jQuery to a separate
    // variable, which probably won't be used.
    AdobeEdge.edgesuiteLogger("Init 'edgejQuery' with: Version " + jQuery().jquery);
    jQueryEdge = jQuery.noConflict(true);
    AdobeEdge.edgesuiteLogger("Reset 'jQuery' to: Version " + jQuery().jquery);
  }

  // Fire composition launch
  AdobeEdge.okToLaunchComposition(comp);
}

/**
 * Edge Suite JavaScript Logger which gets enabled when Drupal variable
 * 'edge_suite_debug_mode' is set to true.
 *
 * @param msg
 *   Log message
 * @param level
 *   Log level
 */
AdobeEdge.edgesuiteLogger = function (msg, level) {
  if (!level) {
    level = 'Info';
  }
  if (AdobeEdge.edgesuiteDebug) {
    console.log('[ DEBUG | EdgeSuite | ' + level + ']: ' + msg);
  }
}