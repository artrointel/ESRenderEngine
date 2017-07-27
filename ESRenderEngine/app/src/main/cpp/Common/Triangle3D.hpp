//
// Created by we.kim on 2017-07-20.
//

#ifndef ESRENDERENGINE_TRIANGLE3D_H
#define ESRENDERENGINE_TRIANGLE3D_H

#include "../RenderBase.hpp"


class Triangle3D
{
public:

    static const int Stride = 3+4+3;
    static const int ByteStride = sizeof(real)*Stride;

    static const int Size = 3*Stride;
    static const int ByteSize = sizeof(real)*Size;
    // Declare Position, Color, Textures as inner union Vertex3
    union Vertex3 {
        Vertex3()
        {
            for(int i = 0; i < 30; i++)
                data[i] = 0.0f;
        }
        real data[3*Stride];
        struct Attrib
        {
            struct Position {
                real x;
                real y;
                real z;
            } pos;
            struct Color {
                real r;
                real g;
                real b;
                real a;
            } color;
            struct TextureCoord {
                real x;
                real y;
                real z;
            } texcoord;
        } vertex[3];
    };
public:
    Vertex3 *attrib; // CPU data
    Vertex3 *mappedAttrib; // pointer of GPU buffer
public:
    explicit Triangle3D()
    {
        attrib = new Vertex3;
    }
    virtual ~Triangle3D() { deleteAttrib(); }

    void mapAttrib(GLfloat *ptrMap)
    {
        mappedAttrib = (Vertex3 *)ptrMap;
        deleteAttrib();
    }

    void showMem(bool mapped = false)
    {
        Vertex3 *target = attrib;
        if(mapped) target=mappedAttrib;
        if(!target)
        {
            ALOGE("Triangle3D::showMem nullptr access");
            return;
        }

        for(int i = 0; i < 3; i++)
        {
            ALOGD("Vertex %d Data", i);
            ALOGD("Pos : %f %f %f",
                  target->vertex[i].pos.x,
                  target->vertex[i].pos.y,
                  target->vertex[i].pos.z);
            ALOGD("Color : %f %f %f %f",
                  target->vertex[i].color.r,
                  target->vertex[i].color.g,
                  target->vertex[i].color.b,
                  target->vertex[i].color.a);
            ALOGD("Texture : %f %f %f",
                  target->vertex[i].texcoord.x,
                  target->vertex[i].texcoord.y,
                  target->vertex[i].texcoord.z);
        }
    }
protected:
    void deleteAttrib()
    {
        if(attrib != nullptr)
        {
            delete attrib;
            attrib = nullptr;
        }
    }
};

#endif //ESRENDERENGINE_VERTEX_H