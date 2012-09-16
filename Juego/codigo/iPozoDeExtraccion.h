#ifndef __I_POZO_EXTRACCION_H__
#define __I_POZO_EXTRACCION_H__

#include "iUnidadUniSeleccionable.h"
#include <string>

namespace Interfaz
{
    
class PozoDeExtraccion : public UnidadUniSeleccionable
{
private:
    static int textura ;
    static int modeloHandler[8] ;
public:
    PozoDeExtraccion(int numJugador) ;
    virtual ~PozoDeExtraccion() ; 
    void Seleccionar();
    void Actualizar();
    int GetTipo(); 
    bool EsReparable(){return true;}
    
    string GetPathRecursos() { return "Unidades\\PozoDeExtraccion\\"; }
    
    
    static void Inicializar() ;
    static void Cerrar();
    
    static int GetModeloHandler(int numJugador){return modeloHandler[numJugador];}
};    

}

#endif
