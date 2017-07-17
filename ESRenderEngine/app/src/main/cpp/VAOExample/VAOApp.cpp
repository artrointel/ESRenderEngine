//
// Created by we.kim on 2017-07-14.
//

#include "VAOApp.h"

bool VAOApp::init()
{
    if(StructBOApp::init())
    {   // Previous example of StructBOApp was actually using an VAO '0' as default.

        // Now we'll make VAO '1' and will use both of them for rendering.

        glGenVertexArrays(1, &vaoId);
        ALOGD("Generated VAO ID is %d", vaoId);

        glGenBuffers(1, &newVboId);
        glBindBuffer(GL_ARRAY_BUFFER, newVboId);
        glBufferData(GL_ARRAY_BUFFER, sizeof(localVertices), &localVertices, GL_STATIC_DRAW);
        ALOGD("Generated New VBO ID is %d", newVboId);

        return true;
    }
    else return false;
}

void VAOApp::render()
{
    ALOGD("render start");
    // Draw Left Triangle with Default VAO '0' and
    glBindVertexArray(0);
    StructBOApp::render();

    // Draw Right Triangle with VAO '1'
    glBindVertexArray(vaoId); // From now on, VAO '1' will be used for Vertex Attrib settings as below.
    {
        // Using Different New Vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, newVboId);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, VERT_SZ_POS, GL_FLOAT, GL_FALSE, 0, 0);

        // Using same colors
        GLuint stride = (VERT_SZ_POS + VERT_SZ_CLR) * sizeof(GL_FLOAT);
        GLuint color_offset = VERT_SZ_POS * sizeof(GLfloat);
        glBindBuffer(GL_ARRAY_BUFFER, mVboId[0]);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, VERT_SZ_CLR, GL_FLOAT, GL_FALSE, stride, (void *)color_offset);

        // Using same indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboId[1]); // Bind forever
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    } // add new bracket with no reason

    glDrawElements(GL_TRIANGLES, VERT_IDXSIZE, GL_UNSIGNED_SHORT, 0);
    checkGLError("StructBOApp::render end");
}