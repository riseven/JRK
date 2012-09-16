#ifndef __I_UNIDAD_AGRESIVA_H__
#define __I_UNIDAD_AGRESIVA_H__

#include "iUnidadMultiSeleccionable.h"

namespace Interfaz
{
    
class UnidadAgresiva : public UnidadMultiSeleccionable
{
private:
    static bool atacar ;
    static bool modoAgresivo ; //1
    static bool modoDefensivo ; //2
    static bool modoMantenerTerreno ; //3
    static bool modoPasivo ; //4
    
    int modoActual ;
public:
    UnidadAgresiva(int numJugador) ;
    virtual ~UnidadAgresiva() ; 
    void Seleccionar();
    void Actualizar(); 
    virtual void UnidadRespuestaInteligenteUnidad(UnidadSeleccionable *unidad);
    virtual void UnidadRespuestaEstrictaTerreno(float x, float z);
    virtual void UnidadRespuestaEstrictaUnidad(UnidadSeleccionable *unidad);
    
    static void PintarBotones();
    static void ActualizarUnidadesAgresivas();
    static void Inicializar();
    static void Reset();
};    
    

}

#endif
