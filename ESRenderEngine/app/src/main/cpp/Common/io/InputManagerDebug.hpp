//
// Created by we.kim on 2017-07-26.
//


#ifndef _INPUT_MANAGERDEBUG_H
#define _INPUT_MANAGERDEBUG_H

#include <jni.h>
#include <android/log.h>

#include "InputHandler.hpp"
#include "../GenericSingleton.hpp"
#include "../common.hpp"

class InputManagerDebug : public InputHandler
{
    MAKE_SINGLETON(InputManagerDebug)
protected:
    InputManagerDebug()
    {

    }
public:
    virtual bool onTouchEvent(int action, int* id, int* x, int* y, int count)
    {
        ALOGD("TOUCH COUNT %d, action %d", count, action);
        for(int i = 0; i < count; i++)
        {
            ALOGD("ID %d X %d Y %d", id[i], x[i],y[i]);
        }
        return false;
    }
};

#endif _INPUT_MANAGERDEBUG_H