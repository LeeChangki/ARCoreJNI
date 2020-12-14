# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

#set( CMAKE_VERBOSE_MAKEFILE on )
#set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI} )
#set( ANDROID_STL c++_shared )
set( CPP_FILES
        native-lib.cpp
        helloAR/hello_ar_application.cc
        helloAR/background_renderer.cc
        helloAR/point_cloud_renderer.cc
        helloAR/augmented_image_renderer.cc
        helloAR/augmented_face_renderer.cc
        helloAR/face_obj_renderer.cc
        helloAR/obj_renderer.cc
        helloAR/plane_renderer.cc
        helloAR/texture.cc
        helloAR/util.cc)

# Import the ARCore (Google Play Services for AR) library.
add_library(arcore SHARED IMPORTED)
set_target_properties(arcore PROPERTIES IMPORTED_LOCATION
        "${ARCORE_LIBPATH}/${ANDROID_ABI}/libarcore_sdk_c.so"
        INTERFACE_INCLUDE_DIRECTORIES "${ARCORE_INCLUDE}"
        )

# Import the glm header file from the NDK.
add_library( glm INTERFACE )
set_target_properties( glm PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${GLM_INCLUDE}")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
add_library( # Sets the name of the library.
                native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             ${CPP_FILES} )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_include_directories(native-lib PRIVATE helloAR)

target_link_libraries( # Specifies the target library.
                       native-lib

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib}

                       android jnigraphics EGL GLESv2 arcore glm)