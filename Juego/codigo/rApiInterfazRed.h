#ifndef __R_APIINTERFAZRED_H__
#define __R_APIINTERFAZRED_H__

#include <allegro.h>
#include <alleggl.h>

#include <string>
#include <winsock2.h>
#include <vector>

#include "rApiInterfaz.h"

using namespace std;

namespace Red
{
    
class ApiInterfazRed : public ApiInterfaz
{
private:
    SOCKET socketEntrada ;
    WSAEVENT eventoAceptarConexion ;
    vector <SOCKET> sockets ;
    vector <WSAEVENT> eventoLeer ;
    
    unsigned char *buffer ;
    string mensajePendiente ;
    
    int mascaraEnvio ;
    
    bool RecibirMensaje(int canal);
    void EnviarMensaje();

    void PutComando(int comando);
    void PutBool(bool valor);
    void PutInt(int valor);
    void PutFloat(float valor);
    void PutString(string texto); 
       
    string GetString(int &pos);
    bool GetBool(int &pos);
    int GetInt(int &pos);
    float GetFloat(int &pos);
    
public:
    ApiInterfazRed();
    ~ApiInterfazRed();
    
    void Actualizar();
    
    void SetMascaraEnvio(int mascara);
    
    
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
    void SetPorcentajeConstruidoUnidad(int handler, int porcentajeConstruido);
    void SetEstadoTecnologia(int numJugador, int numTecnologia, int estado);
    void MensajeChatJuego(int numJugador, string texto);
    void SetDinero(int numJugador, int dinero);
    void SetOcupadoPetroleo(int numPetroleo, bool ocupado);
    void SetDia(bool dia);
    void SetInvierno(bool invierno);
};

}

#endif 
