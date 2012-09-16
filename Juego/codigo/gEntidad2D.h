#ifndef __G_ENTIDAD2D_H__
#define __G_ENTIDAD2D_H__

#include <list>

using namespace std;

namespace Graficos
{

class Entidad2D
{
private:
    static list <Entidad2D *> entidades ;

protected:    
    int x, y, z;
    bool activo ;
    
public:
    static void PintarTodos();
    static void EliminarTodos();
    
    Entidad2D(int X, int Y, int Z);
    virtual ~Entidad2D();
    int GetX();
    int GetY();
    int GetZ();
    void SetPosicion(int x, int y, int z);
    bool IsActivo();
    void SetActivo(bool Activo);
    bool operator >= (const Entidad2D& entidad);
    virtual void Pintar();   
};

}

#endif
