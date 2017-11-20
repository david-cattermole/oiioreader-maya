/*
 * Reads an OpenEXR file (or any other image file type) using OpenImageIO.
 */


#ifndef EXR_OIIO_READER_H
#define EXR_OIIO_READER_H

#include <maya/MPxImageFile.h>
#include <maya/MImageFileInfo.h>
#include <maya/MImage.h>
#include <maya/MStringArray.h>

// OpenImageIO
#include <OpenImageIO/ustring.h>
#include <OpenImageIO/imageio.h>
#include <OpenImageIO/imagebuf.h>
#include <OpenImageIO/imagebufalgo.h>
#include <OpenImageIO/version.h>

#if (OIIO_VERSION < 10100)
namespace OIIO = OIIO_NAMESPACE;
#endif


// Number of threads that OIIO will use. 0=all, 1=single threaded
#ifndef THREAD_NUM
#define THREAD_NUM 1
#endif


// How much memory (in megabytes) to allocate for the OIIO image cache.
#ifndef CACHE_MEM
#define CACHE_MEM 512
#endif

#ifndef READ_EXR
#define READ_EXR 1
#endif

#ifndef READ_DPX
#define READ_DPX 1
#endif

#ifndef READ_CINEON
#define READ_CINEON 1
#endif

#ifndef READ_HDR
#define READ_HDR 1
#endif

#ifndef READ_TX
#define READ_TX 1
#endif


class OIIOImageFile : public MPxImageFile {
public:
    OIIOImageFile();

    virtual ~OIIOImageFile();

    static void *creator();

    virtual MStatus open(MString pathname, MImageFileInfo *info);

    virtual MStatus close();

    virtual MStatus load(MImage &image, unsigned int idx);

protected:
    int fWidth;
    int fHeight;
    int fChannels;
    OIIO::ImageBuf fImageBuffer;
    OIIO::ImageSpec fSpec;
};

#endif // EXR_OIIO_READER_H