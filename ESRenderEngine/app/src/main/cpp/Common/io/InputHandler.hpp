//
// Created by we.kim on 2017-07-25.
//

#ifndef ESRENDERENGINE_INPUTHANDLER_H
#define ESRENDERENGINE_INPUTHANDLER_H

#include <android/input.h>
#include <jni.h>

#define interface class

interface InputHandler
{
public:
    virtual bool onTouchEvent(int action, int* id, int* x, int* y, int count) = 0;
};

#endif //ESRENDERENGINE_INPUTHANDLER_H
