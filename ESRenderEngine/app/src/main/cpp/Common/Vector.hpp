//
// Created by we.kim on 2017-07-27.
//

#ifndef _VECTOR_H
#define _VECTOR_H

#include <math.h>
#include "Util.hpp"
#include "common.hpp"

class Vector3
{
public:
    real x;
    real y;
    real z;
private:
    // Option. Vector3 struct has an additional real data value for better performance.
    // So this pad data does not used any algorithm.
    real pad;

public:
    explicit Vector3() : x(0), y(0), z(0) { }
    explicit Vector3(const real x, const real y, const real z) : x(x), y(y), z(z) { }


public:
    inline void invert()
    {
        x = -x;
        y = -y;
        z = -z;
    }

    // Warning : 'NaN' can be occured when return the result of sqrtf.
    // So you should handle this case when you use this if you have to do.
    inline real magnitude()
    {
        return sqrtf(x*x + y*y + z*z);
    }

    // This costs less than magnitude().
    // So, We suggest to use this method for your any algorithm instead of 'a.magnitude()*a.magnitude()'.
    inline real squareMagnitude()
    {
        return (x*x + y*y + z*z);
    }

    inline void normalize()
    {
        real scalar = magnitude();
        if(scalar > 0)
            (*this) *= static_cast<real>(1.0)/scalar;
    }

    // Set to zero vector.
    inline void clear()
    {
        x = y = z = 0;
    }

public: /* Vector operations */
    inline void addScaledVector(const Vector3& v, real scale)
    {
        x += v.x * scale;
        y += v.y * scale;
        z += v.z * scale;
    }

    inline Vector3 componentProduct(const Vector3& v) const
    {
        return Vector3(x * v.x, y * v.y, z * v.z);
    }

    inline void updateComponentProduct(const Vector3& v)
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
    }

    inline real scalarProduct(const Vector3& v) const
    {
        return x*v.x+y*v.y+z*v.z;
    }

    inline Vector3 vectorProduct(const Vector3& v)
    {
        return Vector3(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
    }

public: /* [scalar *,/ vecter] operator overloading */
    inline void operator*=(const real value)
    {
        x *= value;
        y *= value;
        z *= value;
    }

    inline Vector3 operator*(const real value) const
    {
        return Vector3(x*value, y*value, z*value);
    }

    // Warning; put a non zero value
    inline void operator/=(const real value)
    {
        x /= value;
        y /= value;
        z /= value;
    }

    // Warning; put a non zero value
    inline Vector3 operator/(const real value) const
    {
        return Vector3(x/value, y/value, z/value);
    }

public: /* [vector +,- vector] operator overloading */
    inline void operator+=(const Vector3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
    }

    inline Vector3 operator+(const Vector3& v)
    {
        return Vector3(x+v.x, y+v.y, z+v.z);
    }

    inline void operator-=(const Vector3& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
    }

    inline Vector3 operator-(const Vector3& v)
    {
        return Vector3(x-v.x, y-v.y, z-v.z);
    }

public: /* [inner/outer vector product] operator overloading */
    inline real operator*(const Vector3& v) const // *= is defined by 'product *= scalar' but not here.
    {
        return x*v.x+y*v.y+z*v.z;
    }

    inline Vector3 operator%=(const Vector3& v)
    {
        (*this) = vectorProduct(v);
    }

    inline Vector3 operator%(const Vector3& v)
    {
        return Vector3(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
    }
public:
    inline real operator[](int i) const
    {
        if(i == 0) return x;
        if(i == 1) return y;
        return z;
    }

    inline real &operator[](int i)
    {
        if(i == 0) return x;
        if(i == 1) return y;
        return z;
    }

public: /* Utilities */

    // Make orthogonal and normalize at the same time.
    static inline void makeOrthonormalBasis(Vector3& a, Vector3& b, Vector3& c)
    {
        a.normalize();
        c = a % b;

        if(Util::IsEqual(c.magnitude(), 0.f)) return; // a,b is parallel
        c.normalize();
        b = a % c;
    }

    static inline real Distance(const Vector3& a, const Vector3& b)
    {
        return sqrtf(SqrDistance(a,b));
    }

    // Returns Squared-Distance value for better performance
    static inline real SqrDistance(const Vector3& a, const Vector3& b)
    {
        return fabs(a.x-b.x)*fabs(a.x-b.x) + fabs(a.y-b.y)*fabs(a.y-b.y) + fabs(a.z-b.z)*fabs(a.z-b.z);
    }
};

// This nullVect will be used to check null vector.
static const Vector3 nullVect;

// This originVect will be used to initialize any vector.
static const Vector3 originVect;

namespace Util
{
    static inline bool IsEqual(Vector3 &v1, Vector3 &v2, real epsilon = EPSILON)
    {
        if(fabs(v1.x - v2.x) < epsilon)
            if(fabs(v1.y - v2.y) < epsilon)
                if(fabs(v1.z - v2.z) < epsilon)
                    return true;
        return false;
    }
}

#endif
