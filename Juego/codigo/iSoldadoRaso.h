#ifndef __I_SOLDADORASO_H__
#define __I_SOLDADORASO_H__

#include "iUnidadUniSeleccionable.h"

namespace Interfaz
{
    
class SoldadoRaso : public UnidadUniSeleccionable
{
private:
    int modeloHandler ;
    bool mover ;
    bool detener ;
public:
    SoldadoRaso() ;
    virtual ~SoldadoRaso() ; 
    void Seleccionar();
    void Actualizar(); 
    int GetTipo(); 
};    
    

}

#endif
