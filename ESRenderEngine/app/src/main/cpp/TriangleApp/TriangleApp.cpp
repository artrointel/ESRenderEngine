//
// Created by we.kim on 2017-06-19.
//
#include "android/log.h"
#include "TriangleApp.h"

#define STR(s) #s
#define TOSTR(s) STR(s)

TriangleApp::TriangleApp()
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

TriangleApp::~TriangleApp()
{

}

bool TriangleApp::init()
{
    ALOGD("TriangleApp::init called.");
    mProgram = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    if(!mProgram) return false;

    // location, arr_size, data_type, fixed-point type(normalized), stride, array pointer)
    glVertexAttribPointer(POS_ATTRIB, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    // Enable location
    glEnableVertexAttribArray(POS_ATTRIB);
    return true;
}

void TriangleApp::render()
{
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT);
    glUseProgram(mProgram);

    draw();

    checkGLError("TriangleApp::render");
}

void TriangleApp::draw()
{
    // Primitive type, first index of the array, count
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
