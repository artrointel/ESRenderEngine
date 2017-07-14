//
// Created by we.kim on 2017-07-04.
//
#include<string>
#include "TriangleVBOApp.h"

TriangleVBOApp::TriangleVBOApp()
: vertices{
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
},
#define POS_ATTRIB 0
  indices{ 0,1,2 },
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
{

}

TriangleVBOApp::~TriangleVBOApp()
{

}

bool TriangleVBOApp::init()
{
    ALOGD("TriangleVBOApp::init");

    mProgram = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    if(!mProgram) return false;

    // Generate a buffers from GPU side and get the id of it.
    glGenBuffers(1, &mVboId);
    glBindBuffer(GL_ARRAY_BUFFER, mVboId);

    // GPU buffer size is initialized but there is no data yet.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    ALOGD("TriangleVBOApp::finished");
    return true;
}

void TriangleVBOApp::draw()
{
    // After using buffer object codes, GL notices that the buffer object 'GL_ARRAY_BUFFER'

    // so we use 0 as a location of Vertex (layout(location=0 in the v-shader))
    glEnableVertexAttribArray(0);

    // (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer)
    // index as a location, size of an element, pointer(which is an offset which is based on the Buffer Object)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          0, 0);
    // stride = 0 means tightly packed, so it's meaning '3*sizeof(GLfloat)' as default in this case.

    // There is no drawing code with 'vertices' which is on CPU memory.
    if(vertices[0] > 0.5f) vertices[0] = 0.0f;
    vertices[0] += 0.1f;
    // so this experimental code never be used for drawing.
    // hence, you can remove the CPU memory after init()

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void TriangleVBOApp::render()
{
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT);
    glUseProgram(mProgram);

    draw();

    checkGLError("TriangleVBOApp::render end");
}

