# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.


# set targetPlatform, will be passed in from gradle when this sample is completed
# openGL Supportability
# platform         status
#   (0 12)           ES2/ES3 not supported
#   [12, 18)         ES2 only, for ES3, app do dynamic load
#   [18, 24)         ES2 & ES3
#   [24, infinite)   ES2 & ES3 & Vulkan


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -fno-exceptions -Wall")

if (${ANDROID_PLATFORM_LEVEL} LESS 18)
  message(FATAL_ERROR "OpenGL 3 is not supported before API level 18 (currently using ${ANDROID_PLATFORM_LEVEL}).")
  return()
else ()
  set(OPENGL_LIB GLESv3)
endif (${ANDROID_PLATFORM_LEVEL} LESS 11)



# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library(
            # Sets the name of the library. # Sets the library as a shared library.
            RenderEngineJNIGlue SHARED

            # Provides a relative path to your source file(s).
            gl3stub_wrapper.c
            RenderEngineJNIGlue.cpp
            RenderEngine.h
            RenderBase.cpp
            Common/Util.hpp
            TriangleApp/TriangleApp.cpp
            TriangleApp/TriangleVBOApp.cpp
            VBOExample/StructBOApp.cpp
            VAOExample/VAOApp.cpp
            )



# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries(
                      # Specifies the target library.
                      RenderEngineJNIGlue

                      # Links the target library to the log library
                      # included in the NDK.
                      ${OPENGL_LIB}
                      android
                      EGL
                      log
                      m)
