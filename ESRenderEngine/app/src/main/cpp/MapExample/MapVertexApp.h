//
// Created by we.kim on 2017-07-18.
//

#ifndef ESRENDERENGINE_MAPVERTEXAPP_H
#define ESRENDERENGINE_MAPVERTEXAPP_H

#include "../Common/GenericSingleton.hpp"
#include "../TriangleApp/TriangleVBOApp.h"

/*
 * Using GPU-CPU Shared Memory Example by Map/Unmap buffers API
 */

// TriangleVBOApp was using single VBO from CPU side and copied CPU Memory once on initialization.
// So the previous example couldn't update vertex data after glBufferData() call which was to copy memory.
class MapVertexApp : public TriangleVBOApp
{
    MAKE_SINGLETON(MapVertexApp)
private:
    GLuint vaoMapId;
    GLuint vboMapId; // Map example vbo
    GLfloat mapVertices[9];
    GLfloat* mappedBuf;
protected:
    explicit MapVertexApp()
    {
        memcpy(mapVertices, vertices, sizeof(vertices));
        // to draw right screen
        mapVertices[0] += 1.0f;
        mapVertices[3] += 1.0f;
        mapVertices[6] += 1.0f;
    }
public:
    virtual ~MapVertexApp() {glUnmapBuffer(GL_ARRAY_BUFFER);}

    virtual bool init();
    virtual void render();
};

#endif //ESRENDERENGINE_MAPVERTEXAPP_H
