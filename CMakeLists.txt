cmake_minimum_required(VERSION 2.6)
project(mayaEXROIIOReader)
set(PLUGIN_NAME exrOIIOReader)
set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_BUILD_TYPE RELEASE)

# Maya SDK
set(MAYA_ROOT "/usr/autodesk/maya2016")
set(MAYA_INCLUDE ${MAYA_ROOT}/include)
set(MAYA_LIB ${MAYA_ROOT}/lib)

# OpenImageIO
#set(OIIO_INCLUDE "/usr/local/include")
#set(OIIO_LIB "/usr/local/lib64")
set(OIIO_INCLUDE "/home/davidc/bin/oiio/oiio-Release-1.2.2/dist/linux64/include")
set(OIIO_LIB "/home/davidc/bin/oiio/oiio-Release-1.2.2/build/linux64/libOpenImageIO")

# Utilities
set(UTILS_INCLUDE "/home/davidc/dev/cppTests/utilities")

set(SOURCE_FILES
        include/exrOIIOReader.h
        src/exrOIIOReader.cpp
        src/exrOIIOReaderMain.cpp)

include_directories(
        include
        ${UTILS_INCLUDE}
        ${MAYA_INCLUDE}
        $(OIIO_INCLUDE)
)

link_directories(
        ${LIBRARY_OUTPUT_PATH}
        ${MAYA_LIB}
        ${OIIO_LIB}
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