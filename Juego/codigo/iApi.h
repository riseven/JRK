#ifndef __I_API_H__
#define __I_API_H__

#include <allegro.h>
#include <alleggl.h>

#include "iInterfazPanel.h"
#include "iSoldado.h"
#include "iCuartelGeneral.h"
#include "iInfoMapa.h"
#include "iMenuPartida.h"
#include "iJuego.h"
#include "iJugador.h"
#include "iUnidadSeleccionable.h"

#include <vector>
using namespace std ;

namespace Interfaz
{
    
class Api
{
friend class UnidadSeleccionable ;
    
public:
    static vector <Unidad *> unidades ;
    static vector <Jugador *> jugadores ;
    
private:
    static int barraProgresoCarga ;
    static float progresoCarga ;
    static InfoMapa *infoMapa ;
    static MenuPartida *menuPartida ;
    static InterfazPanel *intPanel ;
    static Juego *juego;
    static Menu *menuActual ;
    static int imagenRaton ;
    
    static int sonidoInvestigacionCompletada ;
    static int sonidoConstruccionCompletada ;
    
public:
    
    // Api publica
    static void NuevoJugador(int numJugador, string nombreJugador);
    static void AbandonoJugador(int numJugador);
    static void SetJugadorPropio(int numJugador);
    static void SetColorJugador(int numJugador, int color);
    static void MensajeChatMenu(int numJugador, string texto);
    static void SetMapa(string nombreFichero);
    static void EmpezarPartida();
    static void MoverUnidad(int handlerUnidad, float x, float y, float z);
    static void SetAnguloUnidad(int handlerUnidad, float angulo);
    static void MensajeChatJuego(int numJugador, string texto);
    static void SetDinero(int numJugador, int dinero);
    static void SetOcupadoPetroleo(int numPetroleo, bool ocupado);
    static void SetEstadoTecnologia(int numJugador, int numTecnologia, int estado);
    static void SetDia(bool dia);
    static void SetInvierno(bool invierno);
    static void CrearUnidad(int handlerUnidad, int numJugador, int tipo, float x, float y, float z); 


    static void DestruirUnidad(int handlerUnidad);
    static void SetPorcentajeConstruidoUnidad(int handlerUnidad, int porcentajeConstruido);
    static vector <Unidad *> GetListaUnidades();
  //  static static list<UnidadMultiSeleccionable *> GetUnidadesSeleccionadas();
    
    // Api publica directa
    static int GetMaxJugadores();
    static void Inicializar();
    static int  GetBarraProgresoCarga(){return barraProgresoCarga;}
    static float GetProgresoCarga(){return progresoCarga;}
    static void SetProgresoCarga(float valor);
    static void Cerrar();
    static void BuclePrincipal();
    static void SetMenuActual(Menu *menuActual);
    
    // Api privada
    static void SetInfoMapa(InfoMapa *infoMapa);
    static InfoMapa *GetInfoMapa();
    static void SetMenuPartida(MenuPartida *menuPartida);
    static void SetJuego(Juego *juego);
    static Juego *GetJuego();
    static void SetInterfazPanel( InterfazPanel *interfazPanel);
    static InterfazPanel *GetInterfazPanel();
    static Jugador *GetJugador(int jugador);

};



}

#endif
