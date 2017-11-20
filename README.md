# OpenImageIO Image Reader for Maya
Image Reader using OpenImageIO (OIIO) library plug-in for Autodesk Maya.

This plug-in was written with the aim to speed up EXR file loading inside Maya, especially for image sequences on the Maya Image Plane. 

This image reader is intended to cache image files in memory using OIIO's ImageCache class. This caching is intended to provide improved performance, as Maya's EXR reading and caching is often very slow.

This plug-in loads OpenEXR, DPX, Cineon, HDR and TX files, however the same plug-in with minor changes can be used to load any image format that OpenImageIO supports. The ability to load perform image operations on file read is also an subject for future research.

This plug-in does not support image format writing.

## Features

The project goals should be considered unattainable. There appears to be a bug in Maya (2016 and 2017) where the 'MPxImageFile' class does not truly allow overriding or control over image file format loading, in Viewport 2 (One Graphics System). This bug has stopped the below features from being possible without some internal Maya API insight or workaround. Anybody reading this with knowledge about the subject would be welcome to provide information about this.

These features were the original goals, but due to the Maya bug above, they are seemingly unattainable. 

- Performance speed up with the help of multi-threaded OpenImageIO reading.
- Transparent integration with Maya.
- Reads image files using OpenImageIO. 
- Supports native OpenImageIO colour space conversion.

### File Formats

OpenImageIO supports a lot of file formats natively. Without any configuration not all file formats are used by the plug-in. However more formats can be added fairly easily.

Supported formats:
 - Open EXR (.exr)
 - Cineon (.cin)
 - DPX (.dpx)
 - Radiance HDR (.hdr)
 - Texture TX (.tx)

## Usage
After installation (see 'Build and Install'), once you've loaded the plug-in the plug-in will be transparently used to load all registered image file formats in Maya. 

```python
import maya.cmds
maya.cmds.unloadPlugin("OpenEXRLoader")
maya.cmds.loadPlugin("oiioImageReader")
```

or via MEL:
```text
unloadPlugin "OpenEXRLoader";
loadPlugin "exrOIIOReader";
```

## Building and Install

### Dependencies

- C++ compiler ([GCC](https://gcc.gnu.org/), Clang, VC++, etc)
- [CMake 2.6+](https://cmake.org/)
- [OpenImageIO 1.2+](https://sites.google.com/site/openimageio/home) 
- [Autodesk Maya 2016+](https://www.autodesk.com.au/products/maya/overview) (Older versions may work by have not been tested)

### Build and Install
  
Run the following in a Bash-like shell:

#### Build OpenImageIO

OpenImageIO can be a little trickly to build; especially on Microsoft Windows.

These build instructions are for CentOS 6.x (Linux) on a x86-64 machine, and we install into '/usr/local', change below as needed for your installation.

Our build will use OpenImageIO 1.2.2 as found on the [github website](https://github.com/OpenImageIO/oiio/releases/tag/Release-1.2.2). For extended build instructions, please see [this website](https://sites.google.com/site/openimageio/checking-out-and-building-openimageio) or the 'INSTALL' file that comes with OpenImageIO.
 
Run the below commands to build and install OpenImageIO. As usual, ensure you understand what each command is doing before running them (especially 'su').

```commandline
$ tar -xf oiio-Release-1.2.2.tar.gz
$ cd oiio-Release-1.2.2
$ make USE_PYTHON=0 USE_OPENGL=0 USE_QT=0 -j 4  # example flags, choose your flags however you wish
$ cd dist/linux64
$ su
... enter password ...
$ cp -R -t /usr/local/ bin 
$ cp -R -t /usr/local/ lib
$ cp -R -t /usr/local/ include
$ mkdir -p /usr/local/share/doc/OpenImageIO-1.2.2/
$ cp -R -t /usr/local/share/doc/OpenImageIO-1.2.2/ doc/*
```

Inside your '~/.bashrc' file, place the following lines to ensure environment variables are setup correctly. 
```bash
# OpenImageIO
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
export IMAGEIO_LIBRARY_PATH=/usr/local/lib
```

#### Build plugin

To compile the Maya plugin 'oiioImageReader', run the commands.

```commandline
$ cd <project root>
$ mkdir build
$ cd build
$ cmake \
  -DMAYA_INCLUDE_PATH=/path/to/maya/include \
  -DMAYA_LIB_PATH=/path/to/maya/lib \
  -DOIIO_LIB_PATH=/usr/local/lib \
  -DOIIO_INCLUDE_PATH=/usr/local/include \ 
  -DTHREAD_NUM=1 \
  -DCACHE_MEM=512 \
  -DREAD_EXR=1 \
  -DREAD_DPX=1 \
  -DREAD_CINEON=1 \
  -DREAD_HDR=1 \
  -DREAD_TX=1 \
  ..
$ make -j 4
```

#### Install plugin

Now lets install into our home directory maya 'plug-ins' directory.

```commandline
$ mkdir ~/maya/<maya version>/plug-ins
$ cp oiioImageReader.so ~/maya/<maya version>/plug-ins
```
## Notes

This plug-in only handles image reading. OpenImageIO cannot be used for image writing with this plug-in and you are limited by Maya's native file saving mechanisms.

This plug-in conflicts with bundled Maya plugins which also load various file formats. You may need to unload various plug-ins
 
OpenImageIO image loading is multi-threaded, which speeds up image loading. Unfortunately the multi-threaded nature of this image reader has been disabled (see Limitations and Known Bugs). 

Inside Maya, you can turn on "Use Interactive Sequence Caching" on a "File" texture node to allow image sequence caching.

## Limitations and Known Bugs

- On Linux build there is a symbol clash between boost inside Maya and OpenImageIO, this causes any parallel computation to crash Maya immediately. Because of this bug the reader has been forced to use a single thread (disable boost) for all image computation inside OpenImageIO.  