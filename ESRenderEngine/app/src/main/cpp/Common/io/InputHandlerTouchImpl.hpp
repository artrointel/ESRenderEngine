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
#include "../quaternion.h"

static const char* TAG = "InputHandlerTouchImpl";

// Find cameraMatrix from vshader and connect it.
class InputHandlerTouchImpl : public InputHandler
{
    MAKE_SINGLETON(InputHandlerTouchImpl)
public:
    // TODO: make ubo and use the matrix by map/unmap buffer.
#define CAM_MATRIX "camMatrix"
    GLuint INDX_ATTR_CAM;
    GLuint refProgram;
    GLfloat coeff;
protected:
    Quaternion orientation;
    Matrix4 matrices[3]; // S,R,T
    GLfloat transformMatrix[16]; // transform & scaler matrix.
    int prev_x;
    int prev_y;
    real prev_sum = -1;
    int prev_touch_count;
    bool active;

    InputHandlerTouchImpl()
            : INDX_ATTR_CAM(-1), refProgram(-1),
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
        Matrix4 identity;
        Matrix4::Identity().fillGLArray(transformMatrix);
        glUniformMatrix4fv(INDX_ATTR_CAM, 1, GL_FALSE, transformMatrix);

        ALOGD("%s, Initialized.", TAG);
        active = true;
    }

    virtual bool onTouchEvent(int action, int* id, int* x, int* y, int count)
    {
        if(!active) return false;

        /* Remove dirty input */
        // Initialize touch data
        if( action == AMOTION_EVENT_ACTION_UP || action > AMOTION_EVENT_ACTION_MOVE)
        {
            // Reset prev touch count to 0 when fingers are removed from the screen panel.
            if(action == AMOTION_EVENT_ACTION_UP)
                prev_touch_count = 0;
            // Remove prev data
            prev_x = -1;
            prev_y = -1;
            prev_sum = -1;
            return false;
        }
        // Remove dirty input by detach fingers from the screen panel.
        if( count < prev_touch_count)
            return false;
        prev_touch_count = count;

        // TRS
        if(count == 1)
        {
            if(action == AMOTION_EVENT_ACTION_MOVE)
            {
                if(prev_x == -1)
                {
                    prev_x = x[0];
                    prev_y = y[0];
                }
                // Set (-x,y,0) because of camera uses inverse of x translation
                matrices[0].translate(-(GLfloat)coeff*(prev_x-x[0]), (GLfloat)coeff*(prev_y-y[0]), 0);
                prev_x = x[0];
                prev_y = y[0];
            }
        }

        if(count == 2)
        {
            if(action == AMOTION_EVENT_ACTION_MOVE)
            {
                GLfloat mx = (x[0]+x[1])/2;
                GLfloat my = (y[0]+y[1])/2;

                if(prev_x == -1)
                {
                    prev_x = mx;
                    prev_y = my;
                }

                Vector3 jesture_direction(mx-prev_x, -(my-prev_y),0); // inverse of y value.
                if(!Util::IsZero(jesture_direction.squareMagnitude()))
                {
                    Vector3 z_dir(0,0,1);
                    Vector3 normal = z_dir % jesture_direction;
                    GLfloat angle = coeff * normal.squareMagnitude();

                    orientation.rotateByAngleAxis(angle, normal);
                    GetTransformMatrix(matrices[1], Vector3(0,0,0), orientation);

                    prev_x = mx;
                    prev_y = my;
                }
            }
        }

        // Scale
        if(count == 3)
        {
            if(action == AMOTION_EVENT_ACTION_MOVE)
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
                real sum = coeff*coeff*(d1+d2+d3);
                if(prev_sum == -1) prev_sum = sum;

                real scaler = (prev_sum - sum);
                matrices[2].scale(-scaler, -scaler, -scaler);
                prev_sum = sum;
            }
        }

        // Initialize transform
        if(count == 4)
        {
            for(int i = 0; i < 3; i++)
                matrices[i] = Matrix4::Identity();
            orientation.initialize();
        }
        Matrix4 trans = matrices[0] * matrices[1] * matrices[2];
        trans.fillGLArray(transformMatrix);
        glUniformMatrix4fv(INDX_ATTR_CAM, 1, GL_FALSE, transformMatrix);
        return false;
    }

    static inline void GetTransformMatrix(Matrix4 &transformMatrix,
                                          const Vector3 &position,
                                          const Quaternion &orientation)
    {
        transformMatrix.data[0] = 1-2*orientation.j*orientation.j -
                                  2*orientation.k*orientation.k;
        transformMatrix.data[1] = 2*orientation.i*orientation.j -
                                  2*orientation.r*orientation.k;
        transformMatrix.data[2] = 2*orientation.i*orientation.k +
                                  2*orientation.r*orientation.j;
        transformMatrix.data[3] = position.x;

        transformMatrix.data[4] = 2*orientation.i*orientation.j +
                                  2*orientation.r*orientation.k;
        transformMatrix.data[5] = 1-2*orientation.i*orientation.i -
                                  2*orientation.k*orientation.k;
        transformMatrix.data[6] = 2*orientation.j*orientation.k -
                                  2*orientation.r*orientation.i;
        transformMatrix.data[7] = position.y;

        transformMatrix.data[8] = 2*orientation.i*orientation.k -
                                  2*orientation.r*orientation.j;
        transformMatrix.data[9] = 2*orientation.j*orientation.k +
                                  2*orientation.r*orientation.i;
        transformMatrix.data[10] = 1-2*orientation.i*orientation.i -
                                   2*orientation.j*orientation.j;
        transformMatrix.data[11] = position.z;
    }
};

#endif _INPUT_MANAGER_H