//
// Created by we.kim on 2017-06-14.
//

#ifndef _RENDERBASE_H
#define _RENDERBASE_H

#include <android/log.h>
#include <math.h>
#include <string>

#if DYNAMIC_ES3
#include "gl3stub.h"
#else
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#endif

#define DEBUG 1

#define LOG_TAG "RenderEngine"
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#if DEBUG
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#else
#define ALOGV(...)
#endif


class RenderBase {
public:
    const EGLContext mEGLContext;
    int mProgram;
    int mWidth;
    int mHeight;

    explicit RenderBase ()
    : mWidth(0), mHeight(0), mProgram(0),
      mEGLContext(eglGetCurrentContext())
    {

    }

    virtual ~RenderBase() {
        destroy();
    }
    virtual bool init() { return false; }
    virtual void render() {/* do some operations */};
    virtual void destroy()
    {
        if(mEGLContext != eglGetCurrentContext())
            return;
        glDeleteProgram(mProgram);
    }
    void resize(int width, int height) { glViewport(0, 0, mWidth=width, mHeight=height); } // this will be fixed
protected:
    static bool checkGLError(const char* funcName);
    static GLuint createShader(GLenum shaderType, const char* src);
    static GLuint createProgram(char const* vtxSrc, char const* fragSrc);
};


#endif //GLES3JNI_RENDERBASE_H
