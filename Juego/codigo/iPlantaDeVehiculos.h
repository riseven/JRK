#ifndef __I_PLANTA_VEHICULOS_H__
#define __I_PLANTA_VEHICULOS_H__

#include "iUnidadUniSeleccionable.h"
#include <string>

namespace Interfaz
{
    
class PlantaDeVehiculos : public UnidadUniSeleccionable
{
private:
    static int modeloHandler ;
public:
    PlantaDeVehiculos(int numJugador) ;
    virtual ~PlantaDeVehiculos() ; 
    void Seleccionar();
    void Actualizar();
    int GetTipo(); 
    bool EsReparable(){return true;}
    
    string GetPathRecursos() { return "Unidades\\PlantaDeVehiculos\\"; }
    
    static void Inicializar() ;
    static void Cerrar();
};    

}

#endif
