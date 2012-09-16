#ifndef __R_APILOGICARED_H__
#define __R_APILOGICARED_H__

#include <allegro.h>
#include <alleggl.h>

#include <winsock2.h>
#include <string>
#include "rApiLogica.h"

using namespace std ;

namespace Red
{
    
class ApiLogicaRed : public ApiLogica
{
private:
    SOCKET socketComunicacion ;
    WSAEVENT eventoLeer ;
    string mensajePendiente ;
    unsigned char *buffer ;
    
    int posEscritura ;
    int posLectura ;
    int posMaxima ;
    int primerTope ;
    int segundoTope ;
    
    
    void PutComando(int comando);
    void PutString(string texto);
    void PutBool(bool valor);
    void PutInt(int valor);
    void PutFloat(float valor);
    
    void IncrementarPosicion();
    int GetSiguientePosicion();
    int GetSiguientePosicion(int cant);
    int PendienteLeer();
    bool GetBool(int &pos);
    int GetInt(int &pos);
    float GetFloat(int &pos);
    string GetString(int &pos);
    
    void EnviarMensaje();
    bool ApiLogicaRed::RecibirMensaje();
    
public:
    ApiLogicaRed();
    ~ApiLogicaRed();
    void Actualizar();
    void UnirsePartida(int numJugador, string nombreJugador);
    void MensajeChatMenu(int numJugador, string texto);
    void CambiarColor(int numJugador);
    void MensajeChatJuego(int numJugador, string texto);
    
    void MoverUnidad(int handlerUnidad, float x, float y);
    void DetenerUnidad(int handlerUnidad);
    void CambiarModo(int handlerUnidad, int modo);
    
    // -------------------- Cuartel General ----------------
    void ConstruirIngeniero(int handlerUnidad);
    void DesarrollarMotivacion(int handlerUnidad);
    void DesarrollarMotivacionMasDistancia(int handlerUnidad);
    
    //! ------------------- Barracones ---------------------
    void ConstruirSoldado(int handlerUnidad);
    void DesarrollarArmaduraSoldado(int handlerUnidad);
    void DesarrollarVelocidadDisparoSoldado(int handlerUnidad);  
    
    //! ------------------- Ingenieros ---------------------
    void ConstruirEdificio(int tipoEdificio, int numJugador, float x, float z, int numIngenieros, int *ingenieros);   
    
    //! ------------------- Soldados -----------------------
    void CorrerSoldado(int handlerUnidad, bool correr);
};   
    
}

#endif
