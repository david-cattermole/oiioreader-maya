/*
 * Reads an OpenEXR file (or any other image file type) using OpenImageIO.
 */

#include <exrOIIOReader.h>


// Constructor
EXROIIOImageFile::EXROIIOImageFile() :
        fImageBuffer(),
        fSpec(),
        fChannels(0),
        fWidth(0),
        fHeight(0) {
}


// Destructor
EXROIIOImageFile::~EXROIIOImageFile() {
    fImageBuffer.clear();
}


// Create a new class.
void *EXROIIOImageFile::creator() {
    return new EXROIIOImageFile();
}


// Open the header of the given file path and extract the file info.
MStatus EXROIIOImageFile::open(MString pathname, MImageFileInfo *info) {
    std::string input_path(pathname.asChar());

    // TODO: Use an ImageCache to back the EXR reads. Then we can
    // hopefully speed up EXR playback in Maya.
    OIIO::ImageCache *cache = NULL;
    fImageBuffer.reset(input_path, cache);

    // // Threads (we can force single thread, if needed?)
    // fImageBuffer.threads(1);

    // Read File Spec (file header)
    int subimage = 0;
    int mipmap = 0;
    bool ok = fImageBuffer.init_spec(input_path, 0, 0);
    if (!ok) {
        std::cerr << "OIIO EXR Reader: Load file specification failed: "
                  << fImageBuffer.geterror() << std::endl;
        return MS::kFailure;
    }
    fSpec = fImageBuffer.spec();

    if (info) {
        fWidth = fSpec.width;
        fHeight = fSpec.height;
        fChannels = std::min(fSpec.nchannels, 4);
        bool hasAlpha = fSpec.alpha_channel != -1;

        info->width((unsigned int) fWidth);
        info->height((unsigned int) fHeight);
        info->channels((unsigned int) fChannels);
        info->hasAlpha(hasAlpha);

        // Multi-layer images are not supported, yet.
        info->numberOfImages(1);

        // Mip maps not handled for now
        info->hasMipMaps(false);

        info->imageType(MImageFileInfo::kImageTypeColor);
        info->hardwareType(MImageFileInfo::kHwTexture2D);
        info->pixelType(MImage::kFloat);
    }
    return MS::kSuccess;
}


// Clears the image buffer (which should close the file handle).
MStatus EXROIIOImageFile::close() {
    fImageBuffer.clear();
    return MS::kSuccess;
}


// Loads the file data originally opened in 'open' function into the given Maya Image class.
MStatus EXROIIOImageFile::load(MImage &image, unsigned int imageNumber) {
    MStatus status = MS::kFailure;

    // Force to use one thread, because OIIO has a bug if we use more than
    // one thread, OIIO will segfault.
    OIIO::attribute("threads", 1);

    if (fImageBuffer.has_error())
        return status;

    image.create((unsigned int) fWidth,
                 (unsigned int) fHeight,
                 (unsigned int) fChannels,
                 MImage::kFloat);
    float *pixels = image.floatPixels();
    if (pixels == NULL)
        return status;

    const int xbegin = 0;
    const int xend = fWidth;
    const int ybegin = 0;
    const int yend = fHeight;
    const int zbegin = 0;
    const int zend = 0;
    const int chbegin = 0;
    const int chend = fChannels;

    // Read the Image
    bool ok = fImageBuffer.read();
    if (!ok) {
        std::cerr << "OIIO EXR Reader: Read error: "
                  << fImageBuffer.geterror() << std::endl;
        return status;
    }

    // Read the pixels into the pre-allocated Maya memory.
    ok = fImageBuffer.get_pixel_channels(
            xbegin, xend,
            ybegin, yend,
            zbegin, zend,
            chbegin, chend,
            OIIO::TypeDesc::FLOAT,
            pixels,
            OIIO::AutoStride,
            OIIO::AutoStride,
            OIIO::AutoStride);
    if (!ok) {
        std::cerr << "OIIO EXR Reader: Read pixels error: "
                  << fImageBuffer.geterror() << std::endl;
        return status;
    }
    // fImageBuffer.clear();
    status = MS::kSuccess;
    return status;
}


