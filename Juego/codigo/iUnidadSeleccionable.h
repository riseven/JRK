#ifndef __I_UNIDAD_SELECCIONABLE_H__
#define __I_UNIDAD_SELECCIONABLE_H__

#include <allegro.h>
#include <alleggl.h>

#include <list>
#include "iUnidad.h"

using namespace std;

namespace Interfaz
{

class UnidadSeleccionable : public Unidad
{
public:
    enum EnumImagenes
    {
        // Unidades        
        ImagenMover = 0,
        ImagenDetener,
        ImagenTomar,

        // Unidades agresivas
        ImagenAtacar,
        ImagenOfensivo,
        ImagenDefensivo,
        ImagenMantenerTerreno,
        ImagenPasivo,

        // Cuartel General
        ImagenConstruirIngeniero,
        ImagenDesarrollarMotivacion,
        ImagenDesarrollarMotivacionMasDistancia,

        // Barracones
        ImagenConstruirSoldado,
        ImagenDesarrollarArmaduraSoldado,
        ImagenDesarrollarVelocidadDisparoSoldado,
        
        // Soldado
        ImagenCorrerSoldado,
        
        // Ingeniero
        ImagenConstruirEdificio,
        ImagenRepararIngeniero,
        
        ImagenConstruirCuartelGeneral,
        ImagenConstruirBarracones,
        ImagenConstruirPozoDeExtraccion,
        
        // Especial
        ImagenCancelarAccion,
    };
    
    enum EnumAcciones
    {
        AccionNinguna = 0,
        AccionMover,
        AccionAtacar,
        AccionReparar,
        SituarEdificio,
    };
    
protected:
    static list <UnidadSeleccionable *> unidadesSeleccionables ;
    
    static bool multiSeleccion ;
    
    static int handlerImagenSeleccion ;
    static int handlerModeloSeleccion[8] ;

    static const int numImagenesBotones = 21 ;
    static int imagenesBotones[numImagenesBotones] ;
    static const int numHotKeys = 2 ;
    static bool estadoHotKey[numHotKeys] ;
    
    static int accionActual ;
        
    static list<UnidadSeleccionable *>::iterator BuscarUltimaBusqueda();

    static void BuscarSiguienteUnidad() ;
    
    int numJugador ;
    int handlerCirculoSeleccion ;
    int handlerBarraVida ;
    
    static list <UnidadSeleccionable *> unidadesApuntadas ;
    
    static UnidadSeleccionable *ultimaBusqueda ;

public:              
    static void Inicializar();
    static void Cerrar();
    static void Reset();
    static void ActualizarUnidadesSeleccionables() ;
    static void SetMultiSeleccion(bool multiSeleccion);
    static UnidadSeleccionable * BuscarUnidad(int handlerObjeto);
    static void RespuestaInteligenteTerreno(float x, float z);
    static void RespuestaInteligenteUnidad(UnidadSeleccionable *unidad);
    static void RespuestaEstrictaTerreno(float x, float z);
    static void RespuestaEstrictaUnidad(UnidadSeleccionable *unidad);
    static void ApuntarUnidad(UnidadSeleccionable *unidad);
    static void DesapuntarUnidades();
    static int  GetAccionActual(){return accionActual;}
    static void SetAccionActual(int accion){accionActual=accion;}
    static void BuscarCuartelGeneral() ;
    static void BuscarIngeniero();

    UnidadSeleccionable(int numJugador);
    virtual ~UnidadSeleccionable();
    virtual void Actualizar(){};
    virtual void Seleccionar();
    virtual void Deseleccionar();
    virtual void HaSidoDeseleccionada();
    virtual bool EstaSeleccionada();
    virtual bool EsMultiSeleccionable(){return false;}
    void SetPos(float x, float y, float z);
    virtual void ActualizarPrimeraPersona();
    virtual void EntrarPrimeraPersona();
    virtual void SalirPrimeraPersona();
    virtual void UnidadRespuestaInteligenteTerreno(float x, float z);
    virtual void UnidadRespuestaInteligenteUnidad(UnidadSeleccionable *unidad);
    virtual void UnidadRespuestaEstrictaTerreno(float x, float z);
    virtual void UnidadRespuestaEstrictaUnidad(UnidadSeleccionable *unidad);
    virtual bool EsReparable(){return false;}
    void SetNumJugador(int numJugador){UnidadSeleccionable::numJugador = numJugador;}
    int GetNumJugador(){return numJugador;}
    virtual string GetPathRecursos()=0 ;
};

}

#endif
