/*
 * Reads an OpenEXR file (or any other image file type) using OpenImageIO.
 */


#include <maya/MFnPlugin.h>
#include <maya/MIOStream.h>

#include <oiioReader.h>

// Name of the Plugin
MString kImagePluginName("OIIO Image Reader");


// Plugin load function
MStatus initializePlugin(MObject obj) {
    MFnPlugin plugin(obj, PLUGIN_COMPANY, "1.0", "Any");

    // File extensions supported
    MStringArray extensions;

#if READ_EXR == 1
    extensions.append("exr");
#endif

#if READ_DPX == 1
    extensions.append("dpx");
#endif

#if READ_CINEON == 1
    extensions.append("cin");
#endif

#if READ_HDR == 1
    extensions.append("hdr");
#endif

#if READ_TX == 1
    extensions.append("tx");
#endif

    CHECK_MSTATUS(plugin.registerImageFile(
            kImagePluginName,
            OIIOImageFile::creator,
            extensions));

    // TODO: Force unload 'OpenEXRLoader' and set auto-load off.
    // MEL: 'unloadPlugin "OpenEXRLoader";'
    // PY: 'maya.cmds.unloadPlugin("OpenEXRLoader")'

    return MS::kSuccess;
}


// Plugin unload function
MStatus uninitializePlugin(MObject obj) {
    MFnPlugin plugin(obj);
    CHECK_MSTATUS(plugin.deregisterImageFile(kImagePluginName));

    return MS::kSuccess;
}

