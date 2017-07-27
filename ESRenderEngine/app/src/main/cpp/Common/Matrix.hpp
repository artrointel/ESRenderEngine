//
// Created by we.kim on 2017-07-27.
//

#ifndef _MATRIX_H
#define _MATRIX_H

#include "Vector.hpp"

class Matrix4 // 3 by 4
{
public:
    real data[12];
public:
    explicit Matrix4();
    real getDeterminant() const;
    void setInverse(const Matrix4 &mat);
    Matrix4 inverse() const;
    void invert();

    // Vector multiplication of transform
    Vector3 transform(const Vector3 &vector) const;
    Vector3 transformInverse(const Vector3 &vector) const;

    // Vector multiplication only direction elements
    Vector3 transformDirection(const Vector3 &vector) const;
    Vector3 transformInverseDirection(const Vector3 &vector) const;

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
