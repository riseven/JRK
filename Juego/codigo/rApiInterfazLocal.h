#ifndef __R_APIINTERFAZLOCAL_H__
#define __R_APIINTERFAZLOCAL_H__

#include <allegro.h>
#include <alleggl.h>

#include <string>

#include "rApiInterfaz.h"

using namespace std;

namespace Red
{
    
class ApiInterfazLocal : public ApiInterfaz
{
public:
    ApiInterfazLocal() ;
    
    void Actualizar();
    void NuevoJugador(int numJugador, string nombreJugador);
    void AbandonoJugador(int numJugador);
    void SetJugadorPropio(int numJugador);
    void SetColorJugador(int numJugador, int color);
    void MensajeChatMenu(int numJugador, string texto);
    void SetMapa(string nombreFichero);
    void EmpezarPartida();
    void CrearUnidad(int handler, int numJugador, int tipo, float x, float y, float z); 
    void SetPosicionUnidad(int handler, float x, float y, float z);
    void SetAnguloUnidad(int handler, float angulo);
    void SetPorcentajeConstruidoUnidad(int handler, int porcentaje);                
    void SetEstadoTecnologia(int numJugador, int numTecnologia, int estado);
    void MensajeChatJuego(int numJugador, string texto);
    void SetDinero(int numJugador, int dinero);
    void SetOcupadoPetroleo(int numPetroleo, bool ocupado);
    void SetDia(bool dia);
    void SetInvierno(bool invierno);
};    
    
}

#endif 
