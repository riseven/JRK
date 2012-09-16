#ifndef __R_APILOGICALOCAL_H__
#define __R_APILOGICALOCAL_H__

#include "rApiLogica.h"

namespace Red
{
    
class ApiLogicaLocal : public ApiLogica
{
public:
    void UnirsePartida(int numJugador, string nombreJugador);
    void MensajeChatMenu(int numJugador, string texto);
    void CambiarColor(int numJugador);
    void EmpezarPartida();    
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
    void ConstruirEdificio(int tipoEdificio, int numJugados, float x, float z, int numIngenieros, int *ingenieros);
    
    //! ------------------- Soldados -----------------------
    void CorrerSoldado(int handlerUnidad, bool correr){}

};   
    
}

#endif
