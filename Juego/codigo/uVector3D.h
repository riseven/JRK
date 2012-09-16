#ifndef __VECTOR_H__
#define __VECTOR_H__

namespace Util
{

class Vector3D
{
private:
    float x, y, z ;
    
public:
    Vector3D();
    Vector3D(float x, float y, float z);
    Vector3D(const Vector3D &v);
    Vector3D & operator = (const Vector3D &v);
    ~Vector3D();
    float GetX();
    float GetY();
    float GetZ();
    void SetX(float x);
    void SetY(float y);
    void SetZ(float z);
    Vector3D operator + (const Vector3D &v);
    Vector3D operator - (const Vector3D &v);
    Vector3D operator += (const Vector3D &v);
    Vector3D MultVec (const Vector3D &v);
    void Normalizar();
};

}

#endif 
    
