#ifndef __I_BARRACONES_H__
#define __I_BARRACONES_H__

#include "iUnidadUniSeleccionable.h"
#include <string>

namespace Interfaz
{
    
class Barracones : public UnidadUniSeleccionable
{
private:
    bool construirSoldado ;
    bool desarrollarArmaduraSoldado ;
    bool desarrollarVelocidadDisparoSoldado ;
    
    int porcentajeConstruido ;
    
    void MostrarBotonesTecnologia();    
    
    //------------
    static int textura ;
    static int modeloHandler[8] ;
public:
    Barracones(int numJugador);
    virtual ~Barracones(); 
    void Seleccionar();
    void Actualizar();
    int GetTipo(); 
    bool EsReparable(){return true;}
    void SetPorcentajeConstruido(int porcentaje);
    
    string GetPathRecursos() { return "Unidades\\Barracones\\"; }
    
    static void Inicializar() ;
    static void Cerrar();
    static int GetModeloHandler(int numJugador){return modeloHandler[numJugador];}    
};    

}

#endif
