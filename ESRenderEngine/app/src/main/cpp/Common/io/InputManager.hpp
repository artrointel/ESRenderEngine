//
// Created by we.kim on 2017-07-25.
//
#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H

#include <jni.h>
#include <android/log.h>

#include "InputHandler.hpp"
#include "../GenericSingleton.hpp"
#include "../common.hpp"

class InputManager : public InputHandler
{
    MAKE_SINGLETON(InputManager)
protected:
    InputManager()
    {

    }
public:
    virtual bool onTouchEvent(int action, int* id, int* x, int* y, int count)
    {

    }
};

#endif _INPUT_MANAGER_H