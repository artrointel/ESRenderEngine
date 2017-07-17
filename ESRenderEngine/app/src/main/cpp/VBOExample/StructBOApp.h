#include "../RenderBase.hpp"
#include "../Common/Util.hpp"
#include "../Common/GenericSingleton.hpp"

#ifndef ESRENDERENGINE_STRUCTBOAPP_H
#define ESRENDERENGINE_STRUCTBOAPP_H


#define VERT_SZ_POS     3 // x, y, z
#define VERT_SZ_CLR     4 // r, g, b, a

#define VERT_IDXSIZE    3
#define VERT_ARRSIZE    (VERT_SZ_POS+VERT_SZ_CLR)*3 // Vertex Attrib array size

class StructBOApp : public RenderBase
{
    MAKE_SINGLETON(StructBOApp)
protected:
    char const *VERTEX_SHADER;
    char const *FRAGMENT_SHADER;
    GLfloat vertices[VERT_ARRSIZE]; // [x,y,z,r,g,b,a]
    GLushort indices[VERT_IDXSIZE];
    GLuint mVboId[2];

    explicit StructBOApp()
    : vertices{
        -0.5f, 0.5f, 0.0f,
         1.0f, 0.0f, 0.0f, 1.0f,

        -1.0f,-0.5f, 0.0f,
         0.0f, 1.0f, 0.0f, 1.0f,

         0.0f,-0.5f, 0.0f,
         0.0f, 0.0f, 1.0f, 1.0f
    }, indices {0,1,2},
#define POS_ATTR_LOC 0
#define CLR_ATTR_LOC 1
      VERTEX_SHADER(
              "#version 300 es\n"
              "layout(location = " TOSTR(POS_ATTR_LOC) ") in vec4 vPos;\n"
              "layout(location = " TOSTR(CLR_ATTR_LOC) ") in vec4 vColor;\n"
              "out vec4 v_color;"
              "void main() {\n"
              "   v_color = vColor;\n"
              "   gl_Position = vPos;\n"
              "}\n"),
      FRAGMENT_SHADER(
              "#version 300 es\n"
              "precision mediump float;\n"
              "in vec4 v_color;\n"
              "out vec4 fragColor;\n"
              "void main(){\n"
              "   fragColor = v_color;\n"
              "}\n")
    {

    };
    virtual ~StructBOApp() {}
public:
    virtual bool init();
    virtual void render();
private:
    void draw();
};

#endif