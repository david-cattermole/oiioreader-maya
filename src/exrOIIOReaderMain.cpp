/*
 * Reads an OpenEXR file (or any other image file type) using OpenImageIO.
 */

#include <exrOIIOReader.h>

#include <maya/MFnPlugin.h>
#include <maya/MIOStream.h>

// Name of the Plugin
MString kImagePluginName("OIIO OpenEXR");


// Plugin load function
MStatus initializePlugin(MObject obj) {
    MFnPlugin plugin(obj, PLUGIN_COMPANY, "1.0", "Any");

    // File extensions supported
    MStringArray extensions;
    extensions.append("exr");

    CHECK_MSTATUS(plugin.registerImageFile(
            kImagePluginName,
            EXROIIOImageFile::creator,
            extensions));

    // TODO: Force unload 'OpenEXRLoader' and set auto-load off.

    return MS::kSuccess;
}


// Plugin unload function
MStatus uninitializePlugin(MObject obj) {
    MFnPlugin plugin(obj);
    CHECK_MSTATUS(plugin.deregisterImageFile(kImagePluginName));

    return MS::kSuccess;
}

