//
// Created by we.kim on 2017-08-14.
//

#ifndef ESRENDERENGINE_CUBE3D_H
#define ESRENDERENGINE_CUBE3D_H

// To make this general cube, we suppose that doesn't use Instanced drawing method.

#include "common.hpp"

class Cube3D {
public:
    static const int Stride = 3+4+3;
    static const int ByteStride = sizeof(real)*Stride;
    static const int Size = 8 * Stride;
    static const int ByteSize = sizeof(real)*Size;

    static const int ByteOffsetPos = sizeof(real)*3;
    static const int ByteOffsetColor= sizeof(real)*4;

    // half length of
    real length; // x-axis
    real height; // y-axis
    real width ; // z-axis

    union Vertex3 {
        struct Attrib
        {
            // Center of Cube3D
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
                Color(real _r, real _g, real _b, real _a)
                        : r(_r), g(_g), b(_b), a(_a) {}
            } color;
            struct TextureCoord {
                real x;
                real y;
                real z;
            } texture;
        } vertex[8];
        real data[8*Stride];

        Vertex3() {
            for (int i = 0; i < Size; i++)
                data[i] = 0.0f;
        }
    };
    Vertex3 *attrib;

public:
    explicit Cube3D(real half_length, real half_height, real half_width)
    {
        length = half_length;
        height = half_height;
        width = half_width;
        attrib = new Vertex3;
        setVertexPosition();
    }

    void setDefaultColor()
    {
        attrib->vertex[0].color = Vertex3::Attrib::Color(1,0,0, 0.7);
        attrib->vertex[1].color = Vertex3::Attrib::Color(0,1,0, 0.7);
        attrib->vertex[2].color = Vertex3::Attrib::Color(0,0,1, 0.7);
        attrib->vertex[3].color = Vertex3::Attrib::Color(1,1,0, 0.7);
        attrib->vertex[4].color = Vertex3::Attrib::Color(0,1,1, 0.7);
        attrib->vertex[5].color = Vertex3::Attrib::Color(1,0,1, 0.7);
        attrib->vertex[6].color = Vertex3::Attrib::Color(0,0,0, 0.7);
        attrib->vertex[7].color = Vertex3::Attrib::Color(1,1,1, 0.7); // 7, 0, 1
    }

    void setVertexPosition()
    {
        attrib->vertex[0].pos.x = length;
        attrib->vertex[0].pos.y = height;
        attrib->vertex[0].pos.z = width;

        attrib->vertex[1].pos.x = -length;
        attrib->vertex[1].pos.y = height;
        attrib->vertex[1].pos.z = width;

        attrib->vertex[2].pos.x = -length;
        attrib->vertex[2].pos.y = -height;
        attrib->vertex[2].pos.z = width;

        attrib->vertex[3].pos.x = length;
        attrib->vertex[3].pos.y = -height;
        attrib->vertex[3].pos.z = width;

        attrib->vertex[4].pos.x = length;
        attrib->vertex[4].pos.y = -height;
        attrib->vertex[4].pos.z = -width;

        attrib->vertex[5].pos.x = length;
        attrib->vertex[5].pos.y = height;
        attrib->vertex[5].pos.z = -width;

        attrib->vertex[6].pos.x = -length;
        attrib->vertex[6].pos.y = height;
        attrib->vertex[6].pos.z = -width;

        attrib->vertex[7].pos.x = -length;
        attrib->vertex[7].pos.y = -height;
        attrib->vertex[7].pos.z = -width;
    }

    static void GetIndices(GLenum mode, GLuint* const out_indices)
    {
        if(mode == GL_TRIANGLE_FAN)
        {
            if(out_indices == nullptr)
            {
                ALOGE("Cube3D::getIndices in memory error");
                return;
            }

            int fan[69] = {0, 1, 2, 2, 2, 0,
                           0, 2, 3, 3, 3, 0,
                           0, 3, 4, 4, 4, 0,
                           0, 4, 5, 5, 5, 0,
                           0, 5, 6, 6, 6, 0,
                           0, 6, 1, 1, 1, 7,
                           7, 3, 2, 2, 2, 7,
                           7, 4, 3, 3, 3, 7,
                           7, 5, 4, 4, 4, 7,
                           7, 6, 5, 5, 5, 7,
                           7, 2, 1, 1, 1, 7,
                           7, 1, 6
            };
            for(int i = 0; i < 69; i++)
                out_indices[i] = fan[i];
        }
    }


};


#endif //ESRENDERENGINE_CUBE3D_H
