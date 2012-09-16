#ifndef __I_LABORATORIO_H__
#define __I_LABORATORIO_H__

#include "iUnidadUniSeleccionable.h"
#include <string>

namespace Interfaz
{
    
class Laboratorio : public UnidadUniSeleccionable
{
private:
    static int modeloHandler ;
public:
    Laboratorio(int numJugador) ;
    virtual ~Laboratorio() ; 
    void Seleccionar();
    void Actualizar();
    int GetTipo(); 
    bool EsReparable(){return true;}
    
    string GetPathRecursos() { return "Unidades\\Laboratorio\\"; }
    
    static void Inicializar() ;
    static void Cerrar();
};    

}

#endif
