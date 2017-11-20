/*
 * Reads an OpenEXR file (or any other image file type) using OpenImageIO.
 */

#include <maya/MPxImageFile.h>
#include <maya/MImageFileInfo.h>
#include <maya/MImage.h>

#include <oiioReader.h>

// Constructor
OIIOImageFile::OIIOImageFile() :
        fImageBuffer(),
        fSpec(),
        fChannels(0),
        fWidth(0),
        fHeight(0) {
    OIIO::attribute("max_memory_MB", float(CACHE_MEM));
    OIIO::attribute("threads", THREAD_NUM);
}


// Destructor
OIIOImageFile::~OIIOImageFile() {
    fImageBuffer.clear();
}


// Create a new class.
void *OIIOImageFile::creator() {
    return new OIIOImageFile();
}


// Open the header of the given file path and extract the file info.
MStatus OIIOImageFile::open(MString pathname, MImageFileInfo *info) {
    std::string input_path(pathname.asChar());
//    std::cerr << "OIIO ImageReader: Open file path: " << input_path << std::endl;

    OIIO::ImageCache *cache = OIIO::ImageCache::create(true);
    cache->attribute("max_memory_MB", CACHE_MEM);
    cache->attribute("threads", THREAD_NUM);
    cache->attribute("forcefloat", 1);

    fImageBuffer.reset(input_path, cache);

    // Read File Spec (file header)
    int subimage = 0;
    int mipmap = 0;
    bool ok = fImageBuffer.init_spec(input_path, subimage, mipmap);
    if (!ok) {
        std::cerr << "OIIO ImageReader: Load file specification failed: "
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

        info->hardwareType(MImageFileInfo::kHwTexture2D);
        info->pixelType(MImage::kFloat);
    }
    std::cerr << cache->getstats(5) << std::endl;
    return MS::kSuccess;
}


// Clears the image buffer (which should close the file handle).
MStatus OIIOImageFile::close() {
//    std::cerr << "OIIO ImageReader: Close file path: " << fImageBuffer.name() << std::endl;
    return MS::kSuccess;
}


// Loads the file data originally opened in 'open' function into the given Maya Image class.
MStatus OIIOImageFile::load(MImage &image, unsigned int imageNumber) {
    MStatus status = MS::kFailure;
//    std::cerr << "OIIO ImageReader: Load file path: "
//              << fImageBuffer.name()
//              << " img num:"
//              << imageNumber
//              << std::endl;

    if (fImageBuffer.has_error()) {
        std::cerr << "Maya OIIO Reader: Image Buffer has an error." << std::endl;
        return status;
    }

    image.create((unsigned int) fWidth,
                 (unsigned int) fHeight,
                 (unsigned int) fChannels,
                 MImage::kFloat);
    float *pixels = image.floatPixels();
    if (pixels == NULL) {
        std::cerr << "Maya OIIO Reader: No pixels from Maya." << std::endl;
        return status;
    }

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
        std::cerr << "Maya OIIO Reader: Read error: "
                  << fImageBuffer.geterror() << std::endl;
        return status;
    } else {
        std::cerr << "OIIO ImageReader: Reading File Path: "
                  << fImageBuffer.name() << std::endl;
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
        std::cerr << "Maya OIIO Reader: Read pixels error: "
                  << fImageBuffer.geterror() << std::endl;
        return status;
    }

    status = MS::kSuccess;
    return status;
}
