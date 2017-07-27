//
// Created by we.kim on 2017-06-14.
//

#ifndef _RENDERBASE_H
#define _RENDERBASE_H

#include <math.h>
#include <string>

#if DYNAMIC_ES3
#include "gl3stub.h"
#else
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <android/input.h>

#include "Common/common.hpp"
#include "Common/io/InputManager.hpp"

#endif

class RenderBase {
public:
    const EGLContext mEGLContext;
    int mProgram;
    int mWidth;
    int mHeight;

    InputHandler *io;

    explicit RenderBase ()
    : mWidth(0), mHeight(0), mProgram(0),
      mEGLContext(eglGetCurrentContext()),
      io(nullptr)
    {
        io = InputManager::GetInstance();
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
    void onTouchEvent(int action, int* id, int* x, int* y, int count) { if(io) io->onTouchEvent(action, id, x, y, count); };
    void resize(int width, int height) { glViewport(0, 0, mWidth=width, mHeight=height); } // this will be fixed

protected:
    static bool checkGLError(const char* funcName);
    static GLuint createShader(GLenum shaderType, const char* src);
    static GLuint createProgram(char const* vtxSrc, char const* fragSrc);
};


#endif //GLES3JNI_RENDERBASE_H
