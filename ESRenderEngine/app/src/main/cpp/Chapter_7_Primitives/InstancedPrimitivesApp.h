//
// Created by we.kim on 2017-07-24.
//

#ifndef ESRENDERENGINE_INSTANCEDPRIMITIVES_H
#define ESRENDERENGINE_INSTANCEDPRIMITIVES_H

// Make Two triangles with 'a draw call'.
// but the each of instances can have some different attributes, ex. position in most cases

#include "../RenderBase.hpp"
#include "../Common/Util.hpp"
#include "../Common/GenericSingleton.hpp"
#include "../Common/Matrix.hpp"

class InstancedPrimitivesApp : public RenderBase
{
    MAKE_SINGLETON(InstancedPrimitivesApp)
private:
    static const int NUM_INSTANCES = 2; // The number of Instance of Triangle object.
    char const *VERTEX_SHADER;
    char const *FRAGMENT_SHADER;

    GLuint vboPosId;
    GLuint vboColorId;
    GLuint vboMatId;
    GLuint vboIndexId;

    GLfloat vertices[9];
    GLfloat colors[12];
    Matrix4 transformMatrix;
    GLushort indices[3];

protected:
    explicit InstancedPrimitivesApp()
    : vertices{
            -0.5f, 0.5f, 0.0f,
            -1.0f,-0.5f, 0.0f,
             0.0f,-0.5f, 0.0f
    }, colors{
            1.0f, 0.2f, 0.2f, 1.0f,
            0.2f, 1.0f, 0.2f, 1.0f,
            0.2f, 0.2f, 1.0f, 1.0f
    }, indices { // 012, (211, 212, 222, 223, 334), 435
            0, 1, 2
    },
#define VPOS_ATTR_LOC 0
#define VCLR_ATTR_LOC 1
#define VMAT_ATTR_LOC 2
#define VCAM_ATTR_LOC 6
        VERTEX_SHADER(
            "#version 300 es\n"
            "layout(location = " TOSTR(VPOS_ATTR_LOC) ") in vec4 vPos;\n"
            "layout(location = " TOSTR(VCLR_ATTR_LOC) ") in vec4 vColor;\n"
            "layout(location = " TOSTR(VMAT_ATTR_LOC) ") in mat4 transformMatrix;\n"
            "layout(location = " TOSTR(VCAM_ATTR_LOC) ") uniform mat4 camMatrix;\n"
            "out vec4 v_color;\n"
            "void main() {\n"
            "   gl_Position = transformMatrix * vPos;\n"
            "   v_color = camMatrix * vColor;\n"
            "}\n"),
        FRAGMENT_SHADER(
            "#version 300 es\n"
            "precision mediump float;\n"
            "in vec4 v_color;\n"
            "out vec4 fragColor;\n"
            "void main() {\n"
            "   fragColor = v_color;\n"
            "}\n"
        )
    {

    }
public:
    virtual ~InstancedPrimitivesApp() {}

    virtual bool init();
    virtual void render();
};

#endif //ESRENDERENGINE_INSTANCEDPRIMITIVES_H