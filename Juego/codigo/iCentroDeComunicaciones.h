#ifndef __I_CENTRO_COMUNICACIONES_H__
#define __I_CENTRO_COMUNICACIONES_H__

#include "iUnidadUniSeleccionable.h"

#include <string>

namespace Interfaz
{
    
class CentroDeComunicaciones : public UnidadUniSeleccionable
{
private:
    static int modeloHandler ;
public:
    CentroDeComunicaciones(int numJugador) ;
    virtual ~CentroDeComunicaciones() ; 
    void Seleccionar();
    void Actualizar();
    int GetTipo(); 
    bool EsReparable(){return true;}
    
    string GetPathRecursos() { return "Unidades\\CentroDeComunicaciones\\"; }
    
    static void Inicializar() ;
    static void Cerrar();
};    

}

#endif
