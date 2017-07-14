//
// Created by we.kim on 2017-07-12.
//

#include "StructBOApp.h"
#include<stdio.h>

bool StructBOApp::init()
{
    ALOGD("StructBOApp::init");

    mProgram = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    if(!mProgram) return false;

    glGenBuffers(2, mVboId);
    glBindBuffer(GL_ARRAY_BUFFER, mVboId[0]); // Bind forever
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboId[1]); // Bind forever
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    ALOGD("Generated VBO ID is %d, %d", mVboId[0], mVboId[1]);
    ALOGD("StructBOApp::init finished");
    return true;
}

void StructBOApp::draw()
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

    // last parameter of 'indices' set to 0 which is offset of Buffer Object 'GL_ELEMENT_ARRAY_BUFFER'
    glDrawElements(GL_TRIANGLES, VERT_IDXSIZE, GL_UNSIGNED_SHORT, 0);
    // glDrawArrays(GL_TRIANGLES, 0, 3); is okay, but
    // we have sent indices to GPU memory (Buffer Object) and we are just using it!
}

void StructBOApp::render()
{
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT);
    glUseProgram(mProgram);

    draw();

    checkGLError("StructBOApp::render end");
    // disable and unbind them.
}

