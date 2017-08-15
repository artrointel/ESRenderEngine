//
// Created by we.kim on 2017-08-14.
//

#ifndef ESRENDERENGINE_TEXTURE2DAPP_H
#define ESRENDERENGINE_TEXTURE2DAPP_H

#include "../RenderBase.hpp"
#include "../Common/Util.hpp"
#include "../Common/io/InputHandlerTouchImpl.hpp"
#include "../Common/Cube3D.hpp"

class Texture2DApp : public RenderBase
{
    MAKE_SINGLETON(Texture2DApp)
public:
    GLuint mAxisVaoId;
    GLuint mAxisVboId;

    GLuint mCubeVboId;
    GLuint mCubeToId;
    GLuint mCubeIboId;
    GLuint mIndices[30];
    Cube3D cube3D;
    GLubyte texture[4*3];

    char const* VERTEX_SHADER;
    char const* FRAGMENT_SHADER;

protected:
#define VPOS_ATTR_LOC 0
#define VCLR_ATTR_LOC 1
#define VTEX_ATTR_LOC 2
#define VCAM_ATTR_LOC 3
    Texture2DApp()
    : cube3D(0.2, 0.2, 0.2),
      texture{255, 0, 0,
              0, 255, 0,
              0, 0, 255,
              255, 255, 0},
      VERTEX_SHADER(
            "#version 300 es\n"
            "layout(location = " TOSTR(VPOS_ATTR_LOC) ") in vec4 vPos;\n"
            "layout(location = " TOSTR(VCLR_ATTR_LOC) ") in vec4 vColor;\n"
            "layout(location = " TOSTR(VTEX_ATTR_LOC) ") in vec2 vTexCoord;\n"
            "layout(location = " TOSTR(VCAM_ATTR_LOC) ") uniform mat4 " CAM_MATRIX ";\n"
            "out vec2 v_texCoord;\n"
            "out vec4 v_color;\n"
            "void main() {\n"
            "   gl_Position = " CAM_MATRIX "* vPos;\n"
            "   v_texCoord = vTexCoord;\n"
            "   v_color = vColor;"
            "}\n"),
      FRAGMENT_SHADER(
              "#version 300 es\n"
              "precision mediump float;\n"
              "in vec4 v_color;\n"
              "in vec2 v_texCoord;\n"
              "out vec4 fragColor;\n"
              "void main() {\n"
              "     fragColor = v_color;\n"
              "}\n")
    {

    }
    virtual ~Texture2DApp() {}
public:
    virtual bool init();
    virtual void render();
private:
    void draw();
};


#endif //ESRENDERENGINE_TEXTURE2DAPP_H
