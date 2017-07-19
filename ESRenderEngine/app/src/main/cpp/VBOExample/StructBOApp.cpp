//
// Created by we.kim on 2017-07-12.
//

#include "StructBOApp.h"

bool StructBOApp::init()
{
    ALOGD("StructBOApp::init");

    mProgram = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    if(!mProgram) return false;

    glGenBuffers(2, mVboId);
    ALOGD("Generated VBO ID is %d, %d", mVboId[0], mVboId[1]);
    ALOGD("StructBOApp::init finished");

    glBindBuffer(GL_ARRAY_BUFFER, mVboId[0]); // Bind forever
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    {
        GLuint stride = (VERT_SZ_POS + VERT_SZ_CLR) * sizeof(GL_FLOAT);
        GLuint offset = 0;

        // Vertex POS
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, VERT_SZ_POS, GL_FLOAT, GL_FALSE,
                              stride, (void *)offset);

        offset += VERT_SZ_POS * sizeof(GLfloat);
        // Vertex COLOR
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, VERT_SZ_CLR, GL_FLOAT, GL_FALSE,
                              stride, (void *)offset);
    }   // add a bracket with no reason.

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboId[1]); // Bind forever
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    {
        // You'll be noticed that Indices hasn't been set because the indices are not Vertex's attribute.
    }

    return true;
}

void StructBOApp::render()
{
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT);
    glUseProgram(mProgram);

    // same as previous VBO example, this code doesn't be used to update the screen
    // until the init function is called. (which calls glBufferData to upload this data)
    vertices[0] += 0.1;
    if(vertices[0] >= 1.f) vertices[0] = 0.f;

    // last parameter of 'indices' set to 0 which is offset of Buffer Object 'GL_ELEMENT_ARRAY_BUFFER'
    glDrawElements(GL_TRIANGLES, VERT_IDXSIZE, GL_UNSIGNED_SHORT, 0);
    // glDrawArrays(GL_TRIANGLES, 0, 3); is okay, but
    // we have sent indices to GPU memory (Buffer Object) and we are just using it!

    checkGLError("StructBOApp::render end");
    // disable and unbind them.
}

