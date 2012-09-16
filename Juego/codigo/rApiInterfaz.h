#ifndef __R_APIINTERFAZ_H__
#define __R_APIINTERFAZ_H__

#include <allegro.h>
#include <alleggl.h>

#include <string>

using namespace std ;

namespace Red
{
    
class ApiInterfaz
{
public:
    ApiInterfaz(){}
    
    virtual void Actualizar()=0;
    
    virtual void SetMascaraEnvio(int mascara){} ;
    
    virtual void NuevoJugador(int numJugador, string NombreJugador)=0;
    virtual void AbandonoJugador(int numJugador){}
    virtual void SetJugadorPropio(int numJugador){}
    virtual void SetColorJugador(int numJugador, int color){}
    virtual void MensajeChatMenu(int numJugador, string texto){}
    virtual void SetMapa(string nombreFichero){}
    virtual void EmpezarPartida(){}
    virtual void CrearUnidad(int handler, int numJugador, int tipo, float x, float y, float z){}
    virtual void SetPosicionUnidad(int handler, float x, float y, float z){}
    virtual void SetAnguloUnidad(int handler, float angulo){}
    virtual void SetPorcentajeConstruidoUnidad(int handler, int porcentaje){}
    virtual void SetEstadoTecnologia(int numJugador, int numTecnologia, int estado){}
    virtual void MensajeChatJuego(int numJugador, string texto){}
    virtual void SetDinero(int numJugador, int dinero){}
    virtual void SetOcupadoPetroleo(int numPetroleo, bool ocupado){}
    virtual void SetDia(bool dia){}
    virtual void SetInvierno(bool invierno){}
};

}

#endif
