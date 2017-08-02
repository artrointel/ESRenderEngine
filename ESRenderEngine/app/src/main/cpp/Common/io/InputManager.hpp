//
// Created by we.kim on 2017-07-25.
// https://www.khronos.org/opengl/wiki/GLSL_:_common_mistakes
// http://github.prideout.net/modern-opengl-prezo/

#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H

#include <jni.h>
#include <android/log.h>
#include <string>

#include "InputHandler.hpp"
#include "../GenericSingleton.hpp"
#include "../common.hpp"
#include "../Matrix.hpp"
static const char* TAG = "InputManager";
// Find cameraMatrix from vshader and connect it.
class InputManager : public InputHandler
{
    MAKE_SINGLETON(InputManager)
public:
#define CAM_MATRIX "camMatrix"
    GLuint INDX_ATTR_CAM;
    GLuint refProgram;
    GLfloat coeff;
protected:
    //static const char* TAG;
    GLuint uboId;
    Matrix4 camMatrix; // note that it has only 12 elements.
    GLfloat glMatrix[16];
    int prev_x;
    int prev_y;
    bool active;

    InputManager()
            : INDX_ATTR_CAM(-1), uboId(-1), refProgram(-1),
              coeff(0.001), prev_x(-1), prev_y(-1), active(false)
    {

    }
public:
    void init(GLuint pgm)
    {
        refProgram = pgm;
        // to get the Index of any attribute, it was already enabled. or returns -1
        if(refProgram != -1)
        {
            glUseProgram(refProgram);
            INDX_ATTR_CAM = glGetUniformLocation(refProgram, CAM_MATRIX);
        }
        else
        {
            ALOGE("%s, refProgram is not set.",TAG);
            return;
        }
        if(INDX_ATTR_CAM == -1)
        {
            // Check below;
            // 1. program is not valid
            // 2. there is no cameraMatrix in vertex shader
            // 3. cameraMatrix not active
            ALOGW("%s, Couldn't find uniform mat4 camMatrix on program %d.", TAG, refProgram);
            return;
        }
        Matrix4::SetIdentity(camMatrix.data, 12); // Identity
        camMatrix.fillGLArray(glMatrix);

        glGenBuffers(1, &uboId);
        glBindBuffer(GL_UNIFORM_BUFFER, uboId);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glMatrix), glMatrix, GL_DYNAMIC_DRAW);

        GLuint bindingPointIdx = 1;
        glBindBufferBase(GL_UNIFORM_BUFFER, bindingPointIdx, uboId);

        // glUniformMatrix4fv(INDX_ATTR_CAM, 1, GL_FALSE, glMatrix);
        ALOGD("%s, Initialized.", TAG);
        active = true;
    }

    virtual bool onTouchEvent(int action, int* id, int* x, int* y, int count)
    {
        if(!active) return false;

        glBindBuffer(GL_UNIFORM_BUFFER, uboId);
        // buffer Map and update matrix
        GLfloat *pbuffer = (GLfloat *) glMapBufferRange(GL_UNIFORM_BUFFER, 0, sizeof(glMatrix), GL_MAP_WRITE_BIT);
        if(pbuffer == NULL)
        {
            ALOGW("%s, mapped buffer pbuffer is null. ignoring event.", TAG);
            return false;
        }

        // Translation
        if(count == 1 && action == AMOTION_EVENT_ACTION_MOVE)
        {
            if(prev_x == -1)
            {
                prev_x = x[0]; prev_y = y[0];
            }
            Matrix4::Translate(pbuffer, coeff*(prev_x-x[0]), coeff*(prev_y-y[0]), 0);
            prev_x = x[0];
            prev_y = y[0];
        }

        // Scale
        if(count == 3)
        {
            static real prev_sum = -1;
            if(action == AMOTION_EVENT_ACTION_DOWN)
            {
                prev_sum = -1;
            }
            else if(action == AMOTION_EVENT_ACTION_MOVE)
            {
                Vector3 a[3];
                for(int i = 0 ; i < 3; i++)
                {
                    a[i].x = x[i];
                    a[i].y = y[i];
                    a[i].z = 0;
                }
                real d1 = Vector3::SqrDistance(a[0], a[1]);
                real d2 = Vector3::SqrDistance(a[0], a[2]);
                real d3 = Vector3::SqrDistance(a[1], a[2]);
                real sum = d1+d2+d3;

                if(prev_sum == -1) prev_sum = sum;

                real scaler = coeff*(prev_sum - sum);
                Matrix4::Scale(pbuffer, scaler, scaler, 0);
                prev_sum = sum;
            }
        }
        glUnmapBuffer(GL_UNIFORM_BUFFER);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        return false;
    }
};


#endif _INPUT_MANAGER_H