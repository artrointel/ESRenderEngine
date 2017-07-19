//
// Created by we.kim on 2017-07-14.
//

#ifndef ESRENDERENGINE_VAOAPP_H
#define ESRENDERENGINE_VAOAPP_H

#include "../VBOExample/StructBOApp.h"

class VAOApp : public StructBOApp
{
    MAKE_SINGLETON(VAOApp)
protected:
    explicit VAOApp()
    : localVertices{
            0.5f,  0.5f, 0.0f,
            0.0f, -0.5f, 0.0f,
            1.0f, -0.5f, 0.0f
    }
    {}
    virtual ~VAOApp() {}

public:
    GLuint newVboId;
    GLuint vaoId;
    GLfloat localVertices[9];
public:
    virtual bool init(); // override
    virtual void render(); // override
};

#endif //ESRENDERENGINE_VAOAPP_H
