//
// Created by we.kim on 2017-07-14.
//

#ifndef ESRENDERENGINE_VAOAPP_H
#define ESRENDERENGINE_VAOAPP_H

#include "../VBOExample/StructBOApp.h"

class VAOApp : public StructBOApp
{
public:
    GLuint newVboId;
    GLuint vaoId;
public:
    virtual ~VAOApp() {}
    virtual bool init(); // override
    virtual void render(); // override
};

#endif //ESRENDERENGINE_VAOAPP_H
