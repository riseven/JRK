#include <allegro.h>
#include <alleggl.h>
#include "uVector3D.h"
#include <cmath>

namespace Util
{

Vector3D::Vector3D():
    x(0.0f), y(0.0f), z(0.0f)
{
}

Vector3D::Vector3D(float x, float y, float z):
    x(x), y(y), z(z)
{
}

Vector3D::Vector3D(const Vector3D &v)
{
    *this = v ;
}

Vector3D &
Vector3D::operator =(const Vector3D &v)
{
    if ( this == &v )
    {
        return *this ;
    }
    
    x = v.x ;
    y = v.y ;
    z = v.z ;
    return *this ;
}

Vector3D::~Vector3D()
{
    x = 0.0f ;
    y = 0.0f ;
    z = 0.0f ;
}

float
Vector3D::GetX()
{
    return x ;
}

float
Vector3D::GetY()
{
    return y ;
}

float
Vector3D::GetZ()
{
    return z ;
}

void
Vector3D::SetX(float x)
{
    Vector3D::x = x ;
}

void
Vector3D::SetY(float y)
{
    Vector3D::y = y ;
}

void
Vector3D::SetZ(float z)
{
    Vector3D::z = z ;
}

Vector3D
Vector3D::operator +(const Vector3D &v)
{
    return Vector3D(x+v.x, y+v.y, z+v.z);
}

Vector3D
Vector3D::operator -(const Vector3D &v)
{
    return Vector3D(x-v.x, y-v.y, z-v.z);
}

Vector3D
Vector3D::operator +=(const Vector3D &v)
{
    x += v.x ;
    y += v.y ;
    z += v.z ;
    return *this ;
}

Vector3D
Vector3D::MultVec(const Vector3D &v)
{
    return Vector3D( y*v.z - z*v.y ,
                     z*v.x - x*v.z ,
                     x*v.y - y*v.x );
}    

void
Vector3D::Normalizar()
{
    double dist = sqrt( x*x + y*y + z*z );
    x = x / dist ;
    y = y / dist ;
    z = z / dist ;
}

}

