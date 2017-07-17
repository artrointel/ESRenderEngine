//
// Created by we.kim on 2017-06-14.
//


#ifndef _RENDERBASE_CPP
#define _RENDERBASE_CPP

#include "RenderBase.hpp"

bool RenderBase::checkGLError(const char *funcName)
{
    GLint err = glGetError();
    if(err != GL_NO_ERROR)
    {
        ALOGE("RenderBase::checkGLError, GL error after %s() : 0x%08x\n", funcName, err);
        return true;
    }
    return false;
}

GLuint RenderBase::createShader(GLenum shaderType, const char *src)
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

GLuint RenderBase::createProgram(char const* vtxSrc, char const* fragSrc)
{
    ALOGD("RenderBase::createProgram called.");
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
        ALOGE("RenderBase::createProgram, Could not link program.");
        GLint length = 0;
        glGetProgramiv(pgm, GL_INFO_LOG_LENGTH, &length);
        if(length)
        {
            GLchar *log = new GLchar[length];
            if(log)
            {
                glGetProgramInfoLog(pgm, length, NULL, log);
                ALOGE("RenderBase::createProgram, Could not link program\n > %s\n", log);
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

#endif