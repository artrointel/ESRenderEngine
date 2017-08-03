//
// Created by we.kim on 2017-07-20.
//

#ifndef ESRENDERENGINE_MAPFLUSHVERTEXAPP_H
#define ESRENDERENGINE_MAPFLUSHVERTEXAPP_H

#include <string.h>
#include "../Common/GenericSingleton.hpp"
#include "../Common/Triangle3D.hpp"
#include "../TriangleApp/TriangleVBOApp.h"
#include "../Common/io/InputHandlerTouchImpl.hpp"

class MapFlushVertexApp : public TriangleVBOApp
{
    MAKE_SINGLETON(MapFlushVertexApp)
private:
    GLuint vaoMapId;
    GLuint vboMapId; // Map example vbo

    Triangle3D triangle;
    GLfloat* mappedBuf;
protected:
    explicit MapFlushVertexApp()
    {
        // vertex attribute of triangle is much bigger than 'vertices'.
        memcpy(&triangle.attrib->vertex[0].pos, vertices, sizeof(GLfloat)*3);
        memcpy(&triangle.attrib->vertex[1].pos, &vertices[3], sizeof(GLfloat)*3);
        memcpy(&triangle.attrib->vertex[2].pos, &vertices[6], sizeof(GLfloat)*3);
        // to draw right screen
        triangle.attrib->vertex[0].pos.x += 1.0f;
        triangle.attrib->vertex[1].pos.x += 1.0f;
        triangle.attrib->vertex[2].pos.x += 1.0f;

        // and we're supposed to change x position of vertex 0 on rendering, as we have done
    }
public:
    virtual ~MapFlushVertexApp();

    virtual bool init();
    virtual void render();
};

#endif //ESRENDERENGINE_MAPFLUSHVERTEXAPP_H
