#ifndef __I_CUARTEL_GENERAL_H__
#define __I_CUARTEL_GENERAL_H__

#include "iUnidadUniSeleccionable.h"
#include "iApi.h"

#include <string>

namespace Interfaz
{
  
class CuartelGeneral : public UnidadUniSeleccionable
{
private:
    bool construirIngeniero ;
    bool desarrollarMotivacion ;
    bool desarrollarMotivacionMasDistancia ;
    int porcentajeConstruido ;
    
    void MostrarBotonesTecnologia();
    
    //----------
    static int textura ;
    static int modeloHandler[8] ;
public:
    CuartelGeneral(int numJugador) ;
    virtual ~CuartelGeneral() ; 
    
    void SetPorcentajeConstruido(int porcentaje);
    
    void Seleccionar();
    void Actualizar();
    int GetTipo(); 
    bool EsReparable(){return true;}
    
    string GetPathRecursos() { return "Unidades\\CuartelGeneral\\"; }
    
    
    static void Inicializar() ;
    static void Cerrar();
    static int GetModeloHandler(int numJugador){return modeloHandler[numJugador];}
};

}

#endif
