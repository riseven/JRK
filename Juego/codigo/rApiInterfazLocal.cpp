#include "rApiInterfazLocal.h"
#include "iApi.h"

namespace Red
{

ApiInterfazLocal::ApiInterfazLocal():
    ApiInterfaz()
{
}
 
void 
ApiInterfazLocal::Actualizar()
{
    // En la versión local no se comprueban los eventos de red
}

void
ApiInterfazLocal::NuevoJugador(int numJugador, string nombreJugador)
{
    Interfaz::Api::NuevoJugador(numJugador, nombreJugador);
}

void
ApiInterfazLocal::AbandonoJugador(int numJugador)
{
    Interfaz::Api::AbandonoJugador(numJugador);
}

void
ApiInterfazLocal::SetJugadorPropio(int numJugador)
{
    Interfaz::Api::SetJugadorPropio(numJugador);
}


void
ApiInterfazLocal::SetColorJugador(int numJugador, int color)
{
    Interfaz::Api::SetColorJugador(numJugador, color);
}

void
ApiInterfazLocal::MensajeChatMenu(int numJugador, string texto)
{
    Interfaz::Api::MensajeChatMenu(numJugador, texto);
}

void
ApiInterfazLocal::SetMapa(string nombreFichero)
{
    Interfaz::Api::SetMapa(nombreFichero);
}

void
ApiInterfazLocal::EmpezarPartida()
{
    Interfaz::Api::EmpezarPartida();
}

void
ApiInterfazLocal::CrearUnidad(int handler, int numJugador, int tipo, float x, float y, float z)
{
    Interfaz::Api::CrearUnidad(handler, numJugador, tipo, x, y, z);     
}

void
ApiInterfazLocal::SetPosicionUnidad(int handler, float x, float y, float z)
{
    Interfaz::Api::MoverUnidad(handler, x, y, z);
}

void
ApiInterfazLocal::SetAnguloUnidad(int handler, float angulo)
{
    Interfaz::Api::SetAnguloUnidad(handler, angulo);
}

void
ApiInterfazLocal::SetPorcentajeConstruidoUnidad(int handler, int porcentaje)
{
    Interfaz::Api::SetPorcentajeConstruidoUnidad(handler, porcentaje);
}

void
ApiInterfazLocal::SetEstadoTecnologia(int numJugador, int numTecnologia, int estado)
{
    Interfaz::Api::SetEstadoTecnologia(numJugador, numTecnologia, estado);
}  

void
ApiInterfazLocal::MensajeChatJuego(int numJugador, string texto)
{
    Interfaz::Api::MensajeChatJuego(numJugador, texto);
}

void
ApiInterfazLocal::SetDinero(int numJugador, int dinero)
{
    Interfaz::Api::SetDinero(numJugador, dinero);
}

void
ApiInterfazLocal::SetOcupadoPetroleo(int numPetroleo, bool ocupado)
{
    Interfaz::Api::SetOcupadoPetroleo(numPetroleo, ocupado);    
}

void
ApiInterfazLocal::SetDia(bool dia)
{
    Interfaz::Api::SetDia(dia);
}

void
ApiInterfazLocal::SetInvierno(bool invierno)
{
    Interfaz::Api::SetInvierno(invierno);
}

}
