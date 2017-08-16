//
// Created by we.kim on 2017-07-27.
//

#ifndef _MATRIX_H
#define _MATRIX_H

#include <string.h>
#include "Vector.hpp"

class Matrix4 // 3 by 4
{
public:
    real data[12];
public:
    explicit Matrix4();
    explicit Matrix4(
            real a, real b, real c,
            real d, real e, real f,
            real g, real h, real i)
    {
        data[0] = a; data[1] = b; data[2] = c; data[3] = 0;
        data[4] = d; data[5] = e; data[6] = f; data[7] = 0;
        data[8] = g; data[9] = h; data[10] = i; data[11] = 0;
    }
    real getDeterminant() const;
    void setInverse(const Matrix4 &mat);
    Matrix4 inverse() const;
    void invert();
    void setTranslation(real x, real y, real z);
    void setScale(real x, real y, real z);

    // Vector multiplication of transform
    Vector3 transform(const Vector3 &vector) const;
    Vector3 transformInverse(const Vector3 &vector) const;

    // Vector multiplication only direction elements
    Vector3 transformDirection(const Vector3 &vector) const;
    Vector3 transformInverseDirection(const Vector3 &vector) const;

    // Simple Operations for Translation, Scale
    void scale(real dx, real dy, real dz);
    void translate(real dx, real dy, real dz);

public: /* Utilities */
    static inline Vector3 worldToLocal(const Vector3 &world, const Matrix4 &transform)
    { return transform.transformInverse(world); }
    static inline Vector3 localToWorld(const Vector3 &local, const Matrix4 &transform)
    { return transform.transform(local); }
    static inline Vector3 worldToLocalDirn(const Vector3 &world, const Matrix4 &transform)
    { return transform.transformInverseDirection(world); }
    static inline Vector3 localToWorldDirn(const Vector3 &local, const Matrix4 &transform)
    { return transform.transformDirection(local); }
    inline void fillGLArray(float array[16]) const
    {
        array[0] = (float)data[0];
        array[1] = (float)data[4];
        array[2] = (float)data[8];
        array[3] = (float)0;

        array[4] = (float)data[1];
        array[5] = (float)data[5];
        array[6] = (float)data[9];
        array[7] = (float)0;

        array[8] = (float)data[2];
        array[9] = (float)data[6];
        array[10] = (float)data[10];
        array[11] = (float)0;

        array[12] = (float)data[3];
        array[13] = (float)data[7];
        array[14] = (float)data[11];
        array[15] = (float)1;
    }
    inline void setFromGLArray(float array[16])
    {
        data[0] = array[0];
        data[4] = array[1];
        data[8] = array[2];

        data[1] = array[4];
        data[5] = array[5];
        data[9] = array[6];

        data[2] = array[8];
        data[6] = array[9];
        data[10] = array[10];

        data[3] = array[12];
        data[7] = array[13];
        data[11] = array[14];
    }

public: // GLES 3.x buffer Interfaces
    // transpose = false if gl buffer array which is column-major matrix
    static inline void Scale(real *glBuffer, real dx, real dy, real dz)
    {
        glBuffer[0] += dx;
        glBuffer[5] += dy;
        glBuffer[10]+= dz;
    }

    static inline void Translate(real *glBuffer, real dx, real dy, real dz, bool gl_transpose = GL_FALSE)
    {
        if(gl_transpose)
        {
            glBuffer[3] += dx;
            glBuffer[7] += dy;
            glBuffer[11] += dz;
            return;
        }
        glBuffer[12] += dx;
        glBuffer[13] += dy;
        glBuffer[14] += dz;
    }

    static inline Matrix4 GetRotationMatrix(real angle, real x, real y, real z)
    {
        angle = 3.141592f*angle/180.f;
        Vector3 axis(x,y,z);
        Matrix4 rotMat(cosf(angle) + axis.x*axis.x*(1-cosf(angle)),
                       axis.x*axis.y*(1-cosf(angle)) - axis.z*sinf(angle),
                       axis.x*axis.z*(1-cosf(angle)) + axis.y*sinf(angle),

                       axis.y*axis.x*(1-cosf(angle)) + axis.z*sinf(angle),
                       cosf(angle) + axis.y*axis.y*(1-cosf(angle)),
                       axis.y*axis.z*(1-cosf(angle)) - axis.x*sinf(angle),

                       axis.z*axis.x*(1-cosf(angle)) - axis.y*sinf(angle),
                       axis.z*axis.y*(1-cosf(angle)) + axis.x*sinf(angle),
                       cosf(angle) + axis.z*axis.z*(1-cosf(angle))
        );
        return rotMat;
    }

    static inline void Rotate(real *glBuffer, real angle, real x, real y, real z)
    {
        angle = 3.141592f*angle/180.f;
        Vector3 axis(x,y,z);
        axis.normalize();
        Matrix4 glMatrix;
        glMatrix.setFromGLArray(glBuffer);
        glMatrix = GetRotationMatrix(angle, x, y, z) * glMatrix;
        glMatrix.fillGLArray(glBuffer);
    }

    // Both GL buffer and Matrix4
    static inline void SetIdentity(real *buffer, real sz)
    {
        if(buffer != nullptr)
        {
            for(int i = 0 ; i < sz; i++) buffer[i] = 0;
            buffer[0] = 1;
            buffer[5] = 1;
            buffer[10] = 1;
            if(sz >= 15)
                buffer[15] = 1;
        }
    }

public: /* get/set */
    //void setOrientationAndPos(const Quaternion &q, const Vector3 &pos);
    Vector3 getAxisVector(int axis) const;
public:
    inline Vector3 operator*(const Vector3 &vector) const
    {
        return Vector3(
                vector.x * data[0] +
                vector.y * data[1] +
                vector.z * data[2] +
                data[3],

                vector.x * data[4] +
                vector.y * data[5] +
                vector.z * data[6] +
                data[7],

                vector.x * data[8] +
                vector.y * data[9] +
                vector.z * data[10] +
                data[11]
        );
    }

    inline Matrix4 operator*(const Matrix4 &mat) const
    {
        Matrix4 result;
        result.data[0] = data[0] * mat.data[0] +
                         data[1] * mat.data[4] +
                         data[2] * mat.data[7];
        result.data[1] = data[0] * mat.data[1] +
                         data[1] * mat.data[5] +
                         data[2] * mat.data[9];
        result.data[2] = data[0] * mat.data[2] +
                         data[1] * mat.data[6] +
                         data[2] * mat.data[10];
        result.data[3] = data[0] * mat.data[3] +
                         data[1] * mat.data[7] +
                         data[2] * mat.data[11] +
                         data[3];

        result.data[4] = data[4] * mat.data[0] +
                         data[5] * mat.data[4] +
                         data[6] * mat.data[7];
        result.data[5] = data[4] * mat.data[1] +
                         data[5] * mat.data[5] +
                         data[6] * mat.data[9];
        result.data[6] = data[4] * mat.data[2] +
                         data[5] * mat.data[6] +
                         data[6] * mat.data[10];
        result.data[7] = data[4] * mat.data[3] +
                         data[5] * mat.data[7] +
                         data[6] * mat.data[11] +
                         data[7];

        result.data[8] = data[8] * mat.data[0] +
                         data[9] * mat.data[4] +
                         data[10] * mat.data[7];
        result.data[9] = data[8] * mat.data[1] +
                         data[9] * mat.data[5] +
                         data[10] * mat.data[9];
        result.data[10] =data[8] * mat.data[2] +
                         data[9] * mat.data[6] +
                         data[10] * mat.data[10];
        result.data[11] =data[8] * mat.data[3] +
                         data[9] * mat.data[7] +
                         data[10] * mat.data[11] +
                         data[11];

        return result;
    }
};


#endif
