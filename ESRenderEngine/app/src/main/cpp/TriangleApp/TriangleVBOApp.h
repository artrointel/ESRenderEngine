//
// Created by we.kim on 2017-07-04.
//

#ifndef ESRENDERENGINE_TRIANGLEAPPVBO_H
#define ESRENDERENGINE_TRIANGLEAPPVBO_H

#include "../RenderEngine.h"

class TriangleVBOApp : public RenderBase<TriangleVBOApp>
{
public:
    GLfloat vertices[9];
    GLuint indices[3];
    GLuint mVboId;

    char const *VERTEX_SHADER;
    char const *FRAGMENT_SHADER;
public:
    explicit TriangleVBOApp();
    virtual ~TriangleVBOApp();
    virtual bool init();
    virtual void render();
private:
    void draw();
};
#endif //ESRENDERENGINE_TRIANGLEAPPVBO_H
