#include "iConfiguracion.h"

namespace Interfaz
{

string Configuracion::nombreDirectorio = "" ;
string Configuracion::nombreJugador = "Jugador" ;
string Configuracion::ipServidor = "0.0.0.0" ;
bool Configuracion::multiTextura = false ;
bool Configuracion::noche = true ;
bool Configuracion::estaciones = true ;

//Cambia el directoria actual de la aplicacion
void
Configuracion::SetNombreDirectorio(string NombreDirectorio)
{
    nombreDirectorio = NombreDirectorio ;
    
}

//Obtiene el directorio actual
string
Configuracion::GetNombreDirectorio()
{
    return nombreDirectorio ;
}

//Cambia el nombre del jugador
void
Configuracion::SetNombreJugador(string NombreJugador)
{
    nombreJugador = NombreJugador ;
}

//Obtiene nombre del jugador
string
Configuracion::GetNombreJugador()
{
    return nombreJugador ;
}

//Cambia la ipo del servidor para el juego en modo red
void
Configuracion::SetIpServidor(string ipServidor)
{
    Configuracion::ipServidor = ipServidor ;
}

//Obtiene la ip del servidor
string
Configuracion::GetIpServidor()
{
    return ipServidor ;
}

void
Configuracion::SetMultiTextura(bool valor)
{
    multiTextura = valor ;
}

bool
Configuracion::GetMultiTextura()
{
    return multiTextura ;
}

void
Configuracion::SetNoche(bool noche)
{
    Configuracion::noche = noche ;
}

bool
Configuracion::GetNoche()
{
    return noche ;
}

void
Configuracion::SetEstaciones(bool estaciones)
{
    Configuracion::estaciones = estaciones ;
}

bool
Configuracion::GetEstaciones()
{
    return estaciones ;
}

}
