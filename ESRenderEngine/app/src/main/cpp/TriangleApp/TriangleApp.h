//
// Created by we.kim on 2017-06-19.
//

#ifndef GLES3JNI_TRIANGLEAPP_H
#define GLES3JNI_TRIANGLEAPP_H

#include "../RenderEngine.h"
#include "../Common/Util.hpp"

class TriangleApp : public RenderBase<TriangleApp>
{
public:/* Shader */
    GLfloat vertices[9];
    char const *VERTEX_SHADER;
    char const *FRAGMENT_SHADER;
public:
    explicit TriangleApp();
    virtual ~TriangleApp();

    virtual bool init();
    virtual void render();
private:
    void draw();
};


#endif //GLES3JNI_TRIANGLEAPP_H
