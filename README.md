# OpenImageIO Image Reader for Maya
Image Reader using OpenImageIO (OIIO) library plug-in for Autodesk Maya.

Only supports OpenEXR image format currently. This plug-in does not support image format writing.

This plug-in was written with the aim to speed up EXR file loading inside Maya, especially for image sequences on the Maya Image Plane. 

OpenImageIO image loading is multi-threaded, which speeds up image loading. Unfortunately the multi-threaded nature of this image reader has been disabled (see Limitations and Known Bugs). 

In future this image reader is intended to cache image files in memory using OIIO's ImageCache class. This caching is intended to provide improved performance, as Maya's EXR reading and caching is often very slow.

This plug-in loads OpenEXR files, however the same plug-in with minor changes can be used to load any image format that OpenImageIO supports. The ability to load perform image operations on file read is also an subject for future research.

## Features
- Reads OpenEXR files using OpenImageIO

## Usage
- Compile plugin using CMake.
- Copy library (.so file) into plugin path (MAYA_PLUGIN_PATH)
  - For example, create the following directory and copy the .so file into it: 
    - /home/$USER/maya/2016/plug-ins/
- Open Maya
- Unload Maya EXR loader "OpenEXRLoader" and load "exrOIIOReader", using MEL or Python.
  - MEL:
    - unloadPlugin "OpenEXRLoader";
    - loadPlugin "exrOIIOReader";
  - Python:
    - maya.cmds.unloadPlugin("OpenEXRLoader")
    - maya.cmds.loadPlugin("exrOIIOReader")
- Load EXR images as needed normal, Maya will use the loaded plugin to read the image files. 

Tip: Inside Maya, you can turn on "Use Interactive Sequence Caching" on a "File" texture node to allow image sequence caching.

## Building and Install

### Dependencies

- C++ compiler with support for C++11
- CMake 2.6+
- OpenImageIO 1.2+ (https://sites.google.com/site/openimageio/home)
- Autodesk Maya 2016+ 
  - Older versions could work by have not been tested

### Build

_To be written._

### Install

_To be written._

## Limitations and Known Bugs 

- On Linux build there is a symbol clash between boost inside Maya and OpenImageIO, this causes any parallel computation to crash Maya immediately. Because of this bug the reader has been forced to use a single thread (disable boost) for all image computation inside OpenImageIO.  