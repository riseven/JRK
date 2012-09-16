#ifndef __I_TORRE_VIGILANCIA_H__
#define __I_TORRE_VIGILANCIA_H__

#include "iUnidadUniSeleccionable.h"
#include <string>

namespace Interfaz
{
    
class TorreDeVigilancia : public UnidadUniSeleccionable
{
private:
    static int modeloHandler ;
public:
    TorreDeVigilancia(int numJugador) ;
    virtual ~TorreDeVigilancia() ; 
    void Seleccionar();
    void Actualizar();
    int GetTipo(); 
    bool EsReparable(){return true;}
    
    string GetPathRecursos() { return "Unidades\\TorreDeVigilancia\\"; }
    
    static void Inicializar() ;
    static void Cerrar();
};    

}

#endif
