#include <allegro.h>
#include <alleggl.h>
#include "iJugador.h"

namespace Interfaz
{
    
float Jugador::colores[8][3] = 
    {
        {1,0,0},
        {0,1,0},
        {0,0,1},
        {1,1,0},
        {0,1,1},
        {1,0.1,1},
        {0.5,0.5,1},
        {0.5,1,0.5}
    };
int Jugador::jugadorPropio = 0 ;
    
Jugador::Jugador()
{
    for ( int i = 0 ; i < Red::Protocolo::Tecnologia::NumTecnologias ; i++ )
    {
        tecnologias[i] = false ;
    }
    nombre = "" ;
    
    color = 0 ;
} 

Jugador::~Jugador()
{
    for ( int i = 0 ; i < Red::Protocolo::Tecnologia::NumTecnologias ; i++ )
    {
        tecnologias[i] = false ;
    }
}

void
Jugador::SetEstadoTecnologia(int tecnologia, int estado)
{
    tecnologias[tecnologia] = estado ;
}

int
Jugador::GetEstadoTecnologia(int tecnologia)
{
    return tecnologias[tecnologia] ;
}

void
Jugador::SetNombre(string nombre)
{
    Jugador::nombre = nombre ;
}

string
Jugador::GetNombre()
{
    return nombre ;
}

void
Jugador::SetColor(int color)
{
    Jugador::color = color ;
}

int
Jugador::GetColor()
{
    return color ;
}


float
Jugador::GetColorRojo(int col)
{
    return colores[col][0] ;
}

float
Jugador::GetColorVerde(int col)
{
    return colores[col][1];
}

float
Jugador::GetColorAzul(int col)
{
    return colores[col][2] ;
}

int
Jugador::GetDinero()
{
    return dinero ;
}

void
Jugador::SetDinero(int cantidad)
{
    dinero = cantidad ;    
}
    
}
