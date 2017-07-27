//
// Created by we.kim on 2017-07-04.
//
#include<string>
#include "TriangleVBOApp.h"

bool TriangleVBOApp::init()
{
    ALOGD("TriangleVBOApp::init");

    mProgram = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    if(!mProgram)
    {
        ALOGD("createProgram Failed!");
        return false;
    }


    // Generate a buffers from GPU side and get the id of it.
    glGenBuffers(1, &mVboId);
    // After using buffer object codes, GL notices that the buffer object 'GL_ARRAY_BUFFER'
    glBindBuffer(GL_ARRAY_BUFFER, mVboId);

    // GPU buffer size is initialized but there is no data yet.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer)
    // index as a location, size of an element, pointer(which is an offset which is based on the Buffer Object)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          0, 0);
    // stride = 0 means tightly packed, so it's meaning '3*sizeof(GLfloat)' as default in this case.

    // we use 0 as a location of Vertex (layout(location=0 in the v-shader))
    glEnableVertexAttribArray(0);
    checkGLError("TriangleVBOApp::init");
    return true;
}

void TriangleVBOApp::draw()
{
    // We did not upload this cpu-data to Buffer Object by calling glBufferData().
    vertices[0] += 0.1;
    if(vertices[0] >= 1.f) vertices[0] = 0.f;
    // so this experimental code never be used while drawing.
    // (or, you can see the updated-screen after calling glBufferData(vertices) )
    // hence, you can remove the CPU memory after upload

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

