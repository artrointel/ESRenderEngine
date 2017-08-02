//
// Created by we.kim on 2017-07-27.
//

#ifndef _MATRIX_
#define _MATRIX_

#include "matrix.hpp"


/* Matrix4 */

Matrix4::Matrix4() // identity matrix
{
    data[0] = data[5] = data[10] = 1;

    data[1] = data[2] = data[3] = data[4] = data[6] =
    data[7] = data[8] = data[9] = data[11] = 0;
}

real Matrix4::getDeterminant() const
{
    return - data[8]*data[5]*data[2] +
           data[4]*data[9]*data[2] +
           data[8]*data[1]*data[6] -
           data[0]*data[9]*data[6] -
           data[4]*data[1]*data[10] +
           data[0]*data[5]*data[10];
}

void Matrix4::setInverse(const Matrix4 &mat)
{
    real invDetM = getDeterminant();
    if(Util::IsEqual(invDetM, 0)) return;

    invDetM = static_cast<real>(1.0f) / invDetM;

    data[0] = (-mat.data[9]*mat.data[6]+mat.data[5]*mat.data[10])*invDetM;
    data[4] = (mat.data[8]*mat.data[6]-mat.data[4]*mat.data[10])*invDetM;
    data[8] = (-mat.data[8]*mat.data[5]+mat.data[4]*mat.data[9])*invDetM;

    data[1] = (mat.data[9]*mat.data[2]-mat.data[1]*mat.data[10])*invDetM;
    data[5] = (-mat.data[8]*mat.data[2]+mat.data[0]*mat.data[10])*invDetM;
    data[9] = (mat.data[8]*mat.data[1]-mat.data[0]*mat.data[9])*invDetM;

    data[2] = (-mat.data[5]*mat.data[2]+mat.data[1]*mat.data[6])*invDetM;
    data[6] = (mat.data[4]*mat.data[2]-mat.data[0]*mat.data[6])*invDetM;
    data[10] = (-mat.data[4]*mat.data[1]+mat.data[0]*mat.data[5])*invDetM;

    data[3] = (mat.data[9]*mat.data[6]*mat.data[3]
               -mat.data[5]*mat.data[10]*mat.data[3]
               -mat.data[9]*mat.data[2]*mat.data[7]
               +mat.data[1]*mat.data[10]*mat.data[7]
               +mat.data[5]*mat.data[2]*mat.data[11]
               -mat.data[1]*mat.data[6]*mat.data[11])*invDetM;
    data[7] = (-mat.data[8]*mat.data[6]*mat.data[3]
               +mat.data[4]*mat.data[10]*mat.data[3]
               +mat.data[8]*mat.data[2]*mat.data[7]
               -mat.data[0]*mat.data[10]*mat.data[7]
               -mat.data[4]*mat.data[2]*mat.data[11]
               +mat.data[0]*mat.data[6]*mat.data[11])*invDetM;
    data[11] =(mat.data[8]*mat.data[5]*mat.data[3]
               -mat.data[4]*mat.data[9]*mat.data[3]
               -mat.data[8]*mat.data[1]*mat.data[7]
               +mat.data[0]*mat.data[9]*mat.data[7]
               +mat.data[4]*mat.data[1]*mat.data[11]
               -mat.data[0]*mat.data[5]*mat.data[11])*invDetM;
}

Matrix4 Matrix4::inverse() const
{
    Matrix4 result;
    result.setInverse(*this);
    return result;
}

void Matrix4::invert()
{
    setInverse(*this);
}

Vector3 Matrix4::transform(const Vector3 &vector) const
{
    return (*this) * vector;
}

Vector3 Matrix4::transformInverse(const Vector3 &vector) const
{
    Vector3 tmp = vector;
    tmp.x -= data[3];
    tmp.y -= data[7];
    tmp.z -= data[11];

    return Vector3(
            tmp.x * data[0] +
            tmp.y * data[4] +
            tmp.z * data[8],

            tmp.x * data[1] +
            tmp.y * data[5] +
            tmp.z * data[9],

            tmp.x * data[2] +
            tmp.y * data[6] +
            tmp.z * data[10]
    );
}

Vector3 Matrix4::transformDirection(const Vector3 &vector) const
{
    return Vector3(
            vector.x * data[0] +
            vector.y * data[1] +
            vector.z * data[2],

            vector.x * data[4] +
            vector.y * data[5] +
            vector.z * data[6],

            vector.x * data[8] +
            vector.y * data[9] +
            vector.z * data[10]
    );
}

Vector3 Matrix4::transformInverseDirection(const Vector3 &vector) const
{
    return Vector3(
            vector.x * data[0] +
            vector.y * data[4] +
            vector.z * data[8],

            vector.x * data[1] +
            vector.y * data[5] +
            vector.z * data[9],

            vector.x * data[2] +
            vector.y * data[6] +
            vector.z * data[10]
    );
}

void Matrix4::scale(real dx, real dy, real dz)
{
    data[0] += dx;
    data[5] += dy;
    data[10] += dz;
}

void Matrix4::translate(real dx, real dy, real dz)
{
    data[3] += dx;
    data[7] += dy;
    data[11] += dz;
}

Vector3 Matrix4::getAxisVector(int i) const // get the column value
{
    return Vector3(data[i], data[i+4], data[i+8]);
}

#endif
