//
// Created by we.kim on 2017-06-19.
//

#ifndef GLES3JNI_TRIANGLEAPP_H
#define GLES3JNI_TRIANGLEAPP_H

#include "../RenderBase.hpp"
#include "../Common/Util.hpp"
#include "../Common/GenericSingleton.hpp"

class TriangleApp : public RenderBase
{
    MAKE_SINGLETON(TriangleApp)
public:/* Shader */
    GLfloat vertices[9];
    char const *VERTEX_SHADER;
    char const *FRAGMENT_SHADER;
protected:
    explicit TriangleApp()
    :
    vertices{
            0.0f,  0.5f,  0.0f,
            -0.5f, -0.5f,  0.0f,
            0.5f, -0.5f,  0.0f },

#define POS_ATTRIB 0
    VERTEX_SHADER(
            "#version 300 es\n"
            "layout(location = " TOSTR(POS_ATTRIB) ") in vec4 vPosition;\n"
            "void main() {\n"
            "    gl_Position = vPosition;\n"
            "}\n"),

    FRAGMENT_SHADER(
            "#version 300 es\n"
            "precision mediump float;\n"
            "out vec4 fragColor;\n"
            "void main() {\n"
            "    fragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
            "}\n")
    {

    }
public:
    virtual ~TriangleApp() {}

    virtual bool init();
    virtual void render();
private:
    void draw();
};


#endif //GLES3JNI_TRIANGLEAPP_H
