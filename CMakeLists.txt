cmake_minimum_required(VERSION 2.6)
project(mayaOpenImageIOReader)
set(PLUGIN_NAME oiioImageReader)
set(CMAKE_CXX_STANDARD 11)

# Maya SDK
set(MAYA_INCLUDE_PATH "/usr/autodesk/maya2016/include" CACHE PATH "Maya include directory")
set(MAYA_LIB_PATH "/usr/autodesk/maya2016/lib" CACHE PATH "Maya library directory")

# OpenImageIO
set(OIIO_INCLUDE_PATH "/usr/local/include" CACHE PATH "OpenImageIO include directory")
set(OIIO_LIB_PATH "/usr/local/lib" CACHE PATH "OpenImageIO library directory")

# Compile Options
set(THREAD_NUM "1" CACHE STRING "Number of threads OpenImageIO will use.")
set(CACHE_MEM "512" CACHE STRING "Size of the OpenImageIO ImageCache in megabytes")
set(READ_EXR "1" CACHE STRING "Use OIIO to read EXR (.exr) files?")
set(READ_DPX "1" CACHE STRING "Use OIIO to read DPX (.dpx) files?")
set(READ_CINEON "1" CACHE STRING "Use OIIO to read Cineon (.cin) files?")
set(READ_HDR "1" CACHE STRING "Use OIIO to read Radiance HDR (.hdr) files?")
set(READ_TX "1" CACHE STRING "Use OIIO to read TX Texture (.tx) files?")


set(SOURCE_FILES
        include/oiioReader.h
        src/oiioReader.cpp
        src/pluginMain.cpp)

include_directories(
        include
        ${UTILS_INCLUDE}
        ${MAYA_INCLUDE_PATH}
        $(OIIO_INCLUDE_PATH)
)

link_directories(
        ${LIBRARY_OUTPUT_PATH}
        ${MAYA_LIB_PATH}
        ${OIIO_LIB_PATH}
)

add_library(${PLUGIN_NAME} SHARED ${SOURCE_FILES})
set_target_properties(${PLUGIN_NAME} PROPERTIES
        LINK_FLAGS "-Wl,--version-script=${CMAKE_SOURCE_DIR}/plugin.map  -Wl,--strip-all")

target_link_libraries(${PLUGIN_NAME}
        OpenMaya
        Foundation
        OpenImageIO
        z
        m
        )
set_target_properties(${PLUGIN_NAME} PROPERTIES
        PREFIX "" # no 'lib' prefix to .so files
        )