#include <allegro.h>
#include <alleggl.h>

#include "rApiLogicaLocal.h"

#include "lApi.h"
#include "lenum_mensajes.h"
#include "rApi.h"

namespace Red
{

void
ApiLogicaLocal::UnirsePartida(int numJugador, string nombreJugador)
{
    Logica::Api::UnirsePartida(numJugador, nombreJugador);
}

void
ApiLogicaLocal::MensajeChatMenu(int numJugador, string texto)
{
    Api::GetApiInterfaz()->MensajeChatMenu(numJugador, texto);
}

void
ApiLogicaLocal::CambiarColor(int numJugador)
{
    Logica::Api::CambiarColor(numJugador);    
}

void
ApiLogicaLocal::EmpezarPartida()
{
    Logica::Api::EmpezarPartida();
}

void
ApiLogicaLocal::MensajeChatJuego(int numJugador, string texto)
{
    Api::GetApiInterfaz()->MensajeChatJuego(numJugador, texto);
}

void 
ApiLogicaLocal::MoverUnidad(int handlerUnidad, float x, float z)
{
         Logica::Api::EnviarMensaje( SIN_RETARDO,
                                     ORIGEN_INNECESARIO,
                                     handlerUnidad,
                                     Logica::AC_MoverUnidad,
                                     new Util::Vector2D(x,z) );
}

void
ApiLogicaLocal::DetenerUnidad(int handlerUnidad)
{
         Logica::Api::EnviarMensaje( SIN_RETARDO,
                                     ORIGEN_INNECESARIO,
                                     handlerUnidad,
                                     Logica::AC_DetenerUnidad,
                                     NULL );
}

void
ApiLogicaLocal::CambiarModo(int handlerUnidad, int modo)
{
  int* m = new int;
  *m = modo;
  
         Logica::Api::EnviarMensaje( SIN_RETARDO,
                                     ORIGEN_INNECESARIO,
                                     handlerUnidad,
                                     Logica::AC_CambiarModo,
                                     m );
}

void
ApiLogicaLocal::ConstruirIngeniero(int handlerUnidad)
{
    Logica::Api::EnviarMensaje(0, -1, handlerUnidad, Logica::MSG_ConstruirIngeniero, NULL );
}

void
ApiLogicaLocal::DesarrollarMotivacion(int handlerUnidad)
{
    Logica::Api::EnviarMensaje(0, -1, handlerUnidad, Logica::MSG_DesarrollarMotivacion, NULL );
}    

void
ApiLogicaLocal::DesarrollarMotivacionMasDistancia(int handlerUnidad)
{
    Logica::Api::EnviarMensaje(0, -1, handlerUnidad, Logica::MSG_DesarrollarMotivacionMasDistancia, NULL );
}

void 
ApiLogicaLocal::ConstruirSoldado(int handlerUnidad)
{
    Logica::Api::EnviarMensaje(0, -1, handlerUnidad, Logica::MSG_ConstruirSoldado, NULL);
}

void 
ApiLogicaLocal::DesarrollarArmaduraSoldado(int handlerUnidad)
{
    Logica::Api::EnviarMensaje(0, -1, handlerUnidad, Logica::MSG_DesarrollarArmaduraSoldado, NULL);
}

void 
ApiLogicaLocal::DesarrollarVelocidadDisparoSoldado(int handlerUnidad)
{
    Logica::Api::EnviarMensaje(0, -1, handlerUnidad, Logica::MSG_DesarrollarVelocidadDisparoSoldado, NULL);        
}

void
ApiLogicaLocal::ConstruirEdificio(int tipoEdificio, int numJugador, float x, float z, int numIngenieros, int *ingenieros)
{
    Logica::Api::ConstruirEdificio(tipoEdificio, numJugador, x, z);
    // Aqui se debería notificar a los ingenieros que vayan a repara el nuevo edificio    
    
}

}

