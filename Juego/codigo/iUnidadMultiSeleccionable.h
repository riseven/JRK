#ifndef __I_UNIDAD_MULTI_SELECCIONABLE_H__
#define __I_UNIDAD_MULTI_SELECCIONABLE_H__

#include <allegro.h>
#include <alleggl.h>

#include "iUnidadSeleccionable.h"

#include <list>
#include <vector>

using namespace std;

namespace Interfaz
{
    
class UnidadMultiSeleccionable : public UnidadSeleccionable
{
private:

    bool estaSeleccionada ;
    
protected:
    static list<UnidadMultiSeleccionable *> unidadesMultiSeleccionables ;
    static list<UnidadMultiSeleccionable *> unidadesSeleccionadas ;
    
    static vector< list<UnidadMultiSeleccionable *> > grupoSeleccion ;
    
    static bool mover ;
    static bool detener ;
    static bool poseer ;
    static bool cancelar ;
    
    static bool accionRealizada ;
    
    float angY ;
    float angXZ ;
    
    int unidadSonidosMover[20];
    int numSonidosMover ;
    int unidadSonidosAtacar[20];
    int numSonidosAtacar ;

public:
    static void Inicializar();
    static void Cerrar();
    static void Reset();
    static void ActualizarUnidadesMultiSeleccionables() ;
    static void MostrarMenuCancelar();
    static void DeseleccionarTodo();
    static void ActualizarBotones();
    static void RespuestaInteligenteTerreno(float x, float z);
    static void RespuestaInteligenteUnidad(UnidadSeleccionable *unidad);
    static void RespuestaEstrictaTerreno(float x, float z);
    static void RespuestaEstrictaUnidad(UnidadSeleccionable *unidad);
    static void AnyadirUnidad(UnidadMultiSeleccionable *unidad);
    static void QuitarUnidad(UnidadMultiSeleccionable *unidad);
    static void SeleccionarUnidad(UnidadMultiSeleccionable *unidad);
    static void AsignarGrupo(int grupo);
    static void AnyadirGrupo(int grupo);
    static void SeleccionarUnidadOrden(int orden);
    static void DeseleccionarUnidadOrden(int orden);
    static list<UnidadMultiSeleccionable *> GetUnidadesSeleccionadas(){return unidadesSeleccionadas;};
    
    
    UnidadMultiSeleccionable(int numJugador);
    virtual ~UnidadMultiSeleccionable();
    
    virtual void Actualizar();
    virtual void Seleccionar();
    virtual void Deseleccionar();
    virtual bool EstaSeleccionada();
    void SetEstaSeleccionada(bool estaSeleccionada);
    virtual bool EsAgresiva(){return true;}
    virtual bool EsMultiSeleccionable(){return true;}
    virtual void EntrarPrimeraPersona();
    virtual void SalirPrimeraPersona();
    virtual void ActualizarPrimeraPersona();
    virtual void UnidadRespuestaInteligenteTerreno(float x, float z);
    virtual void UnidadRespuestaInteligenteUnidad(UnidadSeleccionable *unidad);
    virtual void UnidadRespuestaEstrictaTerreno(float x, float z);
    virtual void UnidadRespuestaEstrictaUnidad(UnidadSeleccionable *unidad);
    virtual void PintarBotonesUnidad(){}
    virtual string GetPathRecursos()=0;    
};  
    
}

#endif
