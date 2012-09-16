#ifndef __I_HANGAR_H__
#define __I_HANGAR_H__

#include "iUnidadUniSeleccionable.h"
#include <string>

namespace Interfaz
{
    
class Hangar : public UnidadUniSeleccionable
{
private:
    static int modeloHandler ;
public:
    Hangar(int numJugador) ;
    virtual ~Hangar() ; 
    void Seleccionar();
    void Actualizar();
    int GetTipo(); 
    bool EsReparable(){return true;}
    
    string GetPathRecursos() { return "Unidades\\Hangar\\"; }
    
    static void Inicializar() ;
    static void Cerrar();
};    

}

#endif
