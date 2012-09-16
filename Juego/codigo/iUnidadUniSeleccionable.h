#ifndef __I_UNIDAD_UNI_SELECCIONABLE_H__
#define __I_UNIDAD_UNI_SELECCIONABLE_H__

#include "iUnidadSeleccionable.h"
#include <string>

namespace Interfaz
{
    
class UnidadUniSeleccionable : public UnidadSeleccionable    
{
private:
    static UnidadUniSeleccionable *unidadSeleccionada ;

protected:
    

public:
    static void Inicializar();
    static void Cerrar();
    static void Reset();
    static void ActualizarUnidadesUniSeleccionables() ;
    static void DeseleccionarTodo();
    static void SeleccionarUnidad(UnidadUniSeleccionable *unidad);
    
    UnidadUniSeleccionable(int numJugador);
    virtual ~UnidadUniSeleccionable();
    
    virtual void Actualizar();
    virtual void Seleccionar();
    virtual void HaSidoDeseleccionada();
    virtual bool EstaSeleccionada();
};  
    
}

#endif
