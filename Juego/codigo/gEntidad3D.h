#ifndef __G_ENTIDAD3D_H__
#define __G_ENTIDAD3D_H__

#include <allegro.h>
#include <alleggl.h>
#include <list>


using namespace std;

namespace Graficos
{

class Entidad3D
{
private:
    static list <Entidad3D *> entidades ;

protected:    
    float x, y, z;
    float radio;
    float angY, angXZ;
    bool activo ;
    
public:
    Entidad3D(float X, float Y, float Z, float angY=0.0, float angXZ=0.0);
    virtual ~Entidad3D();
    float GetX();
    float GetY();
    float GetZ();
    float GetAnguloY();
    float GetAnguloXZ();
    void SetPosicion(float x, float y, float z);
    void SetAngulo(float angY, float angXZ);
    bool IsActivo();
    void SetActivo(bool Activo);
    virtual void Pintar(); 
    virtual void PintarBoundingBox(){};  
    virtual bool EsSiempreVisible()=0;
    virtual bool EsTransparente()=0;
    virtual int GetCallList(){return -1;};
    void SetRadio(float radio);
    float GetRadio();
    
    static void PintarTodos();
    static void PintarTodosEn(float x0, float y0, float x1, float y1);
    static void EliminarTodos();
    static void ResetClippingRegs();
    static void PintarTodosBoundingBox();    
    static void PintarTodosClipping();
};

}

#endif
