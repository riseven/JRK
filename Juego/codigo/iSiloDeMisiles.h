#ifndef __I_SILO_MISILES_H__
#define __I_SILO_MISILES_H__

#include "iUnidadUniSeleccionable.h"
#include <string>

namespace Interfaz
{
    
class SiloDeMisiles : public UnidadUniSeleccionable
{
private:
    static int modeloHandler ;
public:
    SiloDeMisiles(int numJugador) ;
    virtual ~SiloDeMisiles() ; 
    void Seleccionar();
    void Actualizar();
    int GetTipo(); 
    bool EsReparable(){return true;}
    
    string GetPathRecursos() { return "Unidades\\SiloDeMisiles\\"; }
    
    static void Inicializar() ;
    static void Cerrar();
};    

}

#endif
