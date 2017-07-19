//
// Created by we.kim on 2017-07-04.
//

#ifndef ESRENDERENGINE_TRIANGLEAPPVBO_H
#define ESRENDERENGINE_TRIANGLEAPPVBO_H

#include "../RenderBase.hpp"
#include "../Common/Util.hpp"
#include "../Common/GenericSingleton.hpp"

class TriangleVBOApp : public RenderBase
{
    MAKE_SINGLETON(TriangleVBOApp)
public:
    GLfloat vertices[9];
    GLuint mVboId;

    char const *VERTEX_SHADER;
    char const *FRAGMENT_SHADER;
protected:
    explicit TriangleVBOApp()
    : vertices{
        -0.5f, 0.5f, 0.0f,
        -1.0f,-0.5f, 0.0f,
        0.0f,-0.5f, 0.0f,
},
#define POS_ATTRIB 0
      VERTEX_SHADER(
              "#version 300 es\n"
              "layout(location = " TOSTR(POS_ATTRIB) ") in vec4 vPos;\n"
              "void main() {\n"
              "   gl_Position = vPos;\n"
              "}\n"),
      FRAGMENT_SHADER(
              "#version 300 es\n"
              "precision mediump float;\n"
              "out vec4 fragColor;\n"
              "void main(){\n"
              "   fragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
              "}\n")
    {}
    virtual ~TriangleVBOApp() {}
public:
    virtual bool init();
    virtual void render();
private:
    void draw();
};
#endif //ESRENDERENGINE_TRIANGLEAPPVBO_H
