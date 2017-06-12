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


class EXROIIOImageFile : public MPxImageFile {
public:
    EXROIIOImageFile();

    virtual ~EXROIIOImageFile();

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