#ifndef __R_APILOGICA_H__
#define __R_APILOGICA_H__

#include <string>

using namespace std;

namespace Red
{
    
class ApiLogica
{
public:
    virtual void Actualizar(){}
    
    //! Indica a la lógica que un nuevo jugador se quiere unir a la partida
    //! (Solo tiene efecto en la pantalla de preparación de la partida)    
    virtual void UnirsePartida(int numJugador, string nombreJugador)=0;
    
    //! Indica a la lógica que el jugador ha mandado un nuevo mensaje de chat
    //! en la ventana de configuracion de partida.
    virtual void MensajeChatMenu(int numJugador, string texto){}
    
    //! Indica a la lógica que un jugador ha abandonado la partida
    virtual void AbandonarPartida();
    
    //! Indica a la lógica que un jugador quiere cambiar de color de equipo
    //! (Solo tiene efecto en la pantalla de preparación de la partida)
    virtual void CambiarColor(int numJugador){}
    
    //! Indica a la lógica que el servidor de la partida ha pulsado el boton
    //! Empezar partida
    virtual void EmpezarPartida();
    
    //! Indica a la lógica que el jugador ha enviado un mensaje de chat desde
    //! la partida
    virtual void MensajeChatJuego(int numJugador, string texto){}
    
    //! Indica a la lógica que se le ha ordenado a una unidad moverse a una
    //! coordenada x, z
    virtual void MoverUnidad(int handlerUnidad, float x, float z);
    
    //! Indica a la lógica que un jugador quiere cambiar de bando
    //! (Solo tiene efecto en la pantalla de preparación de la partida)    
    virtual void CambiarBando();    
    
    
    
    //! Indica a la lógica que se ha ordenado detener a una unidad
    virtual void DetenerUnidad(int handlerUnidad);
    
    //! Cambia el modo de la unidad cuando no está ejecutando órdenes
    virtual void CambiarModo(int handlerUnidad, int modo);
    
    //! Indica a la lógica que a una unidad se le ha ordenado atacar a otra 
    virtual void AtacarUnidad(int handlerUnidad, int handlerObjetivo);
    
    //! Indica a la lógica que a una unidad se le ha ordenado seguir a otra
    virtual void SeguirUnidad(int handlerUnidad, int handlerObjetivo);
    
    //! Indica a la lógica que a una unidad se le ha ordenado atacar una zona
    virtual void AtacarZona(int handlerUnidad, float x, float z);
    
    //! ------------------ Cuartel General -------------------------------------
    //! Indica a un cuartel general que construya un ingeniero
    virtual void ConstruirIngeniero(int handlerUnidad);
    
    //! Indica a un cuartel que desarrolle la motivacion
    virtual void DesarrollarMotivacion(int handlerUnidad);
    
    //! Indica a un cuartel que desarrolle el incremento de distancia de la motivación
    virtual void DesarrollarMotivacionMasDistancia(int handlerUnidad);
    
    //! ------------------- Barracones -----------------------------------------
    virtual void ConstruirSoldado(int handlerUnidad){};
    virtual void DesarrollarArmaduraSoldado(int handlerUnidad){};
    virtual void DesarrollarVelocidadDisparoSoldado(int handlerUnidad){};
    
    //! ------------------- Ingenieros -----------------------------------------
    virtual void ConstruirEdificio(int tipoEdificio, int numJugador, float x, float z, int numIngenieros, int *ingenieros){};
    virtual void RepararIngeniero(int handlerIngeniero, int handlerUnidad){}
    
    //! ------------------- Soldados -------------------------------------------
    virtual void CorrerSoldado(int handlerUnidad, bool correr){}
};


}

#endif
