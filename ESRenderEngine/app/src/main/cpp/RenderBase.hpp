//
// Created by we.kim on 2017-06-14.
//

#ifndef GLES3JNI_RENDERBASE_H
#define GLES3JNI_RENDERBASE_H

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

template<class T>
class RenderBase {
private:
    static T *mInstance;
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
    static T* GetInstance()
    {
        if(mInstance == NULL)
            mInstance = new T(); // Will Fix it with CRTP pattern
        return mInstance;
    }
    virtual bool init() { return false; }
    virtual void render() {/* do some operations */};
    virtual void destroy()
    {
        if(mEGLContext != eglGetCurrentContext())
            return;
        glDeleteProgram(mProgram);

        if(mInstance != NULL)
        {
            delete mInstance;
            mInstance = NULL;
        }
    }
    void resize(int width, int height) { glViewport(0, 0, mWidth=width, mHeight=height); } // this will be fixed
protected:
    static bool checkGLError(const char* funcName);
    static GLuint createShader(GLenum shaderType, const char* src);
    static GLuint createProgram(char const* vtxSrc, char const* fragSrc);
};
template<class T>
T* RenderBase<T>::mInstance = NULL;

template<class T>
bool RenderBase<T>::checkGLError(const char *funcName)
{
    GLint err = glGetError();
    if(err != GL_NO_ERROR)
    {
        ALOGE("RenderBase<>::checkGLError, GL error after %s() : 0x%08x\n", funcName, err);
        return true;
    }
    return false;
}

template<class T>
GLuint RenderBase<T>::createShader(GLenum shaderType, const char *src)
{
    // 1. Make a handler of shader.
    GLuint shader_handle = glCreateShader(shaderType);
    if(!shader_handle)
    {
        checkGLError("glCreateShader");
        return 0;
    }
    // 2. Hard-Copy Shader codes for compile.
    glShaderSource(shader_handle, 1, &src, NULL);

    // 3. Then Try Compile it.
    glCompileShader(shader_handle);

    GLint compiled = GL_FALSE;
    glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &compiled);

    if(!compiled)
    {
        GLint length = 0;
        glGetShaderiv(shader_handle, GL_INFO_LOG_LENGTH, &length);
        if(length)
        {
            GLchar *log = new GLchar[length];
            if(log)
            {
                glGetShaderInfoLog(shader_handle, length, NULL, log);
                ALOGE("Could not compile %s shader\n > %s\n",
                      shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment", log);
                delete[] log;
            }
        }
        glDeleteShader(shader_handle);
        return 0;
    }
    return shader_handle;
}

template<class T>
GLuint RenderBase<T>::createProgram(char const* vtxSrc, char const* fragSrc)
{
    ALOGD("RenderBase<>::createProgram called.");
    GLuint vShader = 0;
    GLuint fShader = 0;
    GLuint pgm = 0;
    GLint linked = GL_FALSE;

    // 1. Create Shaders and Program.
    vShader = createShader(GL_VERTEX_SHADER, vtxSrc);
    if(!vShader) return pgm;

    fShader = createShader(GL_FRAGMENT_SHADER, fragSrc);
    if(!fShader) return pgm;

    pgm = glCreateProgram();
    if(!pgm)
    {
        checkGLError("glCreateProgram");
        return 0;
    }
    // 2. Attach Shader to a program and link it.
    glAttachShader(pgm, vShader);
    glAttachShader(pgm, fShader);

    glLinkProgram(pgm);
    glGetProgramiv(pgm, GL_LINK_STATUS, &linked);
    if(!linked)
    {
        ALOGE("RenderBase<>::createProgram, Could not link program.");
        GLint length = 0;
        glGetProgramiv(pgm, GL_INFO_LOG_LENGTH, &length);
        if(length)
        {
            GLchar *log = new GLchar[length];
            if(log)
            {
                glGetProgramInfoLog(pgm, length, NULL, log);
                ALOGE("RenderBase<>::createProgram, Could not link program\n > %s\n", log);
                delete[] log;
            }
        }
        glDeleteProgram(pgm);
        pgm = 0;
    }
    glDeleteShader(vShader);
    glDeleteShader(fShader);
    return pgm;
}


#endif //GLES3JNI_RENDERBASE_H
