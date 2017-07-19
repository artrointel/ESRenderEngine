//
// Created by we.kim on 2017-07-14.
//

#include "VAOApp.h"

bool VAOApp::init()
{
    if(StructBOApp::init())
    {   // Previous example of StructBOApp was actually using an VAO '0' as default.

        // Make New Vertex buffer for position
        glGenBuffers(1, &newVboId);
        ALOGD("Generated New VBO ID is %d", newVboId);
        glBindBuffer(GL_ARRAY_BUFFER, newVboId);
        glBufferData(GL_ARRAY_BUFFER, sizeof(localVertices), &localVertices, GL_STATIC_DRAW);

        // Now we'll make new VAO and will use both of them for rendering.
        glGenVertexArrays(1, &vaoId);
        ALOGD("Generated VAO ID is %d", vaoId);

        // From now on, VAO '1' will be used for Vertex Attrib settings as below.
        glBindVertexArray(vaoId);
        {
            glBindBuffer(GL_ARRAY_BUFFER, newVboId); // binding with no reason
            glVertexAttribPointer(0, VERT_SZ_POS, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);

            // To Use the previous color data, we bind again the vbo and call VertexAttribPointer() again.
            GLuint stride = (VERT_SZ_POS + VERT_SZ_CLR) * sizeof(GL_FLOAT);
            GLuint color_offset = VERT_SZ_POS * sizeof(GLfloat);
            glBindBuffer(GL_ARRAY_BUFFER, mVboId[0]);
            glVertexAttribPointer(1, VERT_SZ_CLR, GL_FLOAT, GL_FALSE, stride, (void *)color_offset);
            glEnableVertexAttribArray(1);

            // Using same indices
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboId[1]);
        } // add a bracket with no reason

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

    // Now the VAO '1' has no any setting(bind, pointer.. etc) codes on this draw call. quite simple!
    glBindVertexArray(1);
    glDrawElements(GL_TRIANGLES, VERT_IDXSIZE, GL_UNSIGNED_SHORT, 0);
    checkGLError("StructBOApp::render end");
}