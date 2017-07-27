//
// Created by we.kim on 2017-07-20.
//

#include "MapFlushVertexApp.h"

bool MapFlushVertexApp::init()
{
    if(TriangleVBOApp::init())
    {
        // Generate an additional VBO for map example
        glGenBuffers(1, &vboMapId);
        glBindBuffer(GL_ARRAY_BUFFER, vboMapId);
        glBufferData(GL_ARRAY_BUFFER, Triangle3D::ByteSize, NULL, GL_DYNAMIC_DRAW);

        if(triangle.attrib)
        {
            // options : We will WRITE data to the mapped buffer and
            // gpu driver can invalidate data of the buffer.
            // It's okay because we haven't ever updated before this buffer by any valid data.
            mappedBuf = (GLfloat *)glMapBufferRange(GL_ARRAY_BUFFER, 0, Triangle3D::ByteSize,
                                                    GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
            if(mappedBuf == NULL)
            {
                ALOGE("MapFlushVertexApp::init glMapBuffer fail");
                return false;
            }
            memcpy(mappedBuf, triangle.attrib->data, Triangle3D::ByteSize);
            glUnmapBuffer(GL_ARRAY_BUFFER); // full-memory of 'mappedBuf' Flush operation from GPU side
            mappedBuf = NULL;
        }

        glGenVertexArrays(1, &vaoMapId);
        glBindVertexArray(vaoMapId);
        {
            glBindBuffer(GL_ARRAY_BUFFER, vboMapId);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                                  Triangle3D::ByteStride, 0);
            glEnableVertexAttribArray(POS_ATTRIB);
            ALOGD("Generated VAO ID is %d", vaoMapId);
        }
        ALOGD("MapFlushVertexApp Initialized");
        return true;
    }
    else return false;
}

MapFlushVertexApp::~MapFlushVertexApp() {
    glDeleteBuffers(1, &vboMapId);
    vboMapId = NULL;
}

void MapFlushVertexApp::render()
{
    // Draw previous triangle on left screen.
    glBindVertexArray(0);
    TriangleVBOApp::render();

    glBindVertexArray(vaoMapId);
    {
        // This time we made a Map with FLUSH option so user can call
        // explicit Flush operation on some discrete data for updating.
        mappedBuf = (GLfloat *)glMapBufferRange(GL_ARRAY_BUFFER, 0, Triangle3D::ByteSize,
                         GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);
        if(mappedBuf == NULL)
        {
            ALOGE("MapVertexApp::init glMapBuffer fail");
            return;
        }

        // we only update a 'x' position of vertex 0
        triangle.mapAttrib(mappedBuf);
        triangle.mappedAttrib->vertex[0].pos.x += 0.1f;
        if(triangle.mappedAttrib->vertex[0].pos.x >= 1.f)
            triangle.mappedAttrib->vertex[0].pos.x = 0.f;
        glFlushMappedBufferRange(GL_ARRAY_BUFFER, 0, sizeof(triangle.mappedAttrib->vertex[0].pos.x)); // sizeof Float
        glUnmapBuffer(GL_ARRAY_BUFFER); // Flush only x data.

        // data of the others will not be updated even if the data has really updated like this.
        // because we didn't flushed.
        // triangle.mappedAttrib->vertex[1].pos.x = -1.f;
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    checkGLError("MapFlushVertexApp::render");
}
