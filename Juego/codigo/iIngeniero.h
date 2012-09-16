#ifndef __I_INGENIERO_H__
#define __I_INGENIERO_H__

#include "iUnidadMultiSeleccionable.h"
#include <string>

namespace Interfaz
{
    
class Ingeniero : public UnidadMultiSeleccionable
{
private:
    static int textura ;
    static int modeloHandler[8] ;
    
    static bool construirPulsado ;
    static bool repararPulsado ;
    
    static bool construirCuartelGeneral ;
    static bool construirBarracones ;
    static bool construirPozo ;

    static bool cancelar ;
    
    static int objetoEdificio ;
    
    static bool situandoEdificio ;
    static int tipoEdificio ;
    
    static int sonidosMover[3] ;
public:
    Ingeniero(int numJugador) ;
    virtual ~Ingeniero() ; 
    void Seleccionar();
    void Actualizar();
    int GetTipo(); 
    bool EsAgresiva(){return false;}
    
    string GetPathRecursos() { return "Unidades\\Ingeniero\\"; }
    void PintarBotonesUnidad();
    
    void UnidadRespuestaInteligenteUnidad(UnidadSeleccionable *unidad);
    void UnidadRespuestaEstrictaTerreno(float x, float z);
    void UnidadRespuestaEstrictaUnidad(UnidadSeleccionable *unidad);
    
    static void Inicializar() ;
    static void Reset();
    static void ActualizarIngeniero();
    static void SituarEdificio(float x, float z);
};    

}

#endif
