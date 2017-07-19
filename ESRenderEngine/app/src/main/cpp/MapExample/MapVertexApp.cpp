//
// Created by we.kim on 2017-07-18.
//

#include "MapVertexApp.h"

bool MapVertexApp::init()
{

    if(TriangleVBOApp::init())
    {
        glGenVertexArrays(1, &vaoMapId);
        ALOGD("Generated VAO ID is %d", vaoMapId);

        // Generate an additional VBO for map example
        glGenBuffers(1, &vboMapId);
        glBindBuffer(GL_ARRAY_BUFFER, vboMapId);
        glBufferData(GL_ARRAY_BUFFER, sizeof(mapVertices), NULL, GL_STATIC_DRAW); // Just notify the size of array to gpu

        // options : We will WRITE data to the mapped buffer
        mappedBuf = (GLfloat *)glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(mapVertices),
                                                         GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
        if(mappedBuf == NULL)
        {
            ALOGE("MapVertexApp::init glMapBuffer fail");
            return false;
        }
        memcpy(mappedBuf, mapVertices, sizeof(mapVertices));
        glUnmapBuffer(GL_ARRAY_BUFFER);
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
        // mapped memory will be updated with this codes and
        // There is no copy operation about it.
        mappedBuf[0] += 0.1f;
        if(mappedBuf[0] >= 1.f) mappedBuf[0] = 0.f;

        glBindBuffer(GL_ARRAY_BUFFER, vboMapId);
        glEnableVertexAttribArray(POS_ATTRIB);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                              0, 0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    checkGLError("MapVertexApp::render");
}
