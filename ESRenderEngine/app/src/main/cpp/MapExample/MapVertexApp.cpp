//
// Created by we.kim on 2017-07-18.
//

#include "MapVertexApp.h"

bool MapVertexApp::init()
{
    if(TriangleVBOApp::init())
    {
        // Generate an additional VBO for map example
        glGenBuffers(1, &vboMapId);
        glBindBuffer(GL_ARRAY_BUFFER, vboMapId);
        glBufferData(GL_ARRAY_BUFFER, sizeof(mapVertices), NULL, GL_DYNAMIC_DRAW);

        // options : We will WRITE data to the mapped buffer
        mappedBuf = (GLfloat *)glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(mapVertices),
                                                         GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
        if(mappedBuf == NULL)
        {
            ALOGE("MapVertexApp::init glMapBuffer fail");
            return false;
        }
        memcpy(mappedBuf, mapVertices, sizeof(mapVertices));

        // full-memory of 'mappedBuf' Flush operation from GPU side
        // and release the pointer to inaccessible.
        glUnmapBuffer(GL_ARRAY_BUFFER);
        mappedBuf = NULL;

        glGenVertexArrays(1, &vaoMapId);
        glBindVertexArray(vaoMapId);
        {
            glBindBuffer(GL_ARRAY_BUFFER, vboMapId);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                                  0, 0);
            glEnableVertexAttribArray(POS_ATTRIB);
            ALOGD("Generated VAO ID is %d", vaoMapId);
        }
        ALOGD("MapVertexApp Initialized");
        return true;
    }
    else return false;
}

void MapVertexApp::render()
{
    // Draw previous triangle on left screen.
    glBindVertexArray(0);
    TriangleVBOApp::render();

    // draw
    glBindVertexArray(vaoMapId);
    {
        // This mapped memory(GPU-CPU shared memory) will be updated with this codes.
        // Notice that there is no 'copy' operation.
        mappedBuf = (GLfloat *)glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(mapVertices),
                                                GL_MAP_WRITE_BIT);
        mappedBuf[0] += 0.1;
        if(mappedBuf[0] >= 0.f) mappedBuf[0] = -1.f;
        glUnmapBuffer(GL_ARRAY_BUFFER);
        mappedBuf = NULL;
        // This is not the best case because only A first of mappedBuf data [0] changes but
        // GPU maybe update the entire of *mappedBuf data to draw even though
        // the other data haven't ever updated.

        // so, one of approach is that getting the only 4 bytes from the glMapBufferRange like below but
        // mappedBuf = (GLfloat *)glMapBufferRange(GL_ARRAY_BUFFER, 0, 4,
        //                                      GL_MAP_WRITE_BIT);
        // This is not for General method.
        // In most cases we should do update too many things on discrete data
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    checkGLError("MapVertexApp::render");
}
