#ifndef __I_UNIDAD_H__
#define __I_UNIDAD_H__

#include <allegro.h>
#include <alleggl.h>

#include <list>

using namespace std;

namespace Interfaz
{

class UnidadSeleccionable ;    

class Unidad
{
private:
    static bool algunaUnidadSeleccionada ;
    static list <Unidad *> unidades ;
    
    float x, y, z ; 
    float angulo ;
    
    
    
protected:
    int handlerUnidad ;
    int objetoHandler ;

    
    static void SetAlgunaUnidadSeleccionada(bool algunaUnidadSeleccionada);
    
    
public:

    // ------------------------------------------------------------------------
    
    static void Inicializar();
    static void Cerrar();
    static void Reset();
    static void ActualizarUnidades();
    static void DestruirUnidades();
    
    Unidad();
    virtual ~Unidad();
    
    int GetHandlerUnidad(){return handlerUnidad;}
    void SetHandlerUnidad(int handlerUnidad);
    virtual void SetPos(float x, float y, float z);
    void GetPos(float &, float &, float &);
    void SetAngulo(float angulo);
    float GetAngulo(){return angulo;}
    virtual int GetTipo()=0;
    virtual int GetNumJugador(){return -1;}

    
    
    
    
    virtual void SetPorcentajeConstruido(int porcentaje){}
};

}

#include "iUnidadSeleccionable.h"

#endif
