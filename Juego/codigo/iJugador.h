#ifndef __I_JUGADOR_H__
#define __I_JUGADOR_H__

#include <allegro.h>
#include <alleggl.h>

#include <string>

#include "rProtocolo.h"

using namespace std;

namespace Interfaz
{
    
class Jugador
{
private:    
    static float colores[8][3] ;
    static int jugadorPropio ;
    
    int tecnologias[Red::Protocolo::Tecnologia::NumTecnologias] ;
    string nombre ;
    int color ;
    
    int dinero ;
    
public:
    static float GetColorRojo(int col);
    static float GetColorVerde(int col);
    static float GetColorAzul(int col);
    static int GetJugadorPropio(){return jugadorPropio;}
    static void SetJugadorPropio(int num){jugadorPropio = num;}
    
    Jugador();
    ~Jugador();
    void SetEstadoTecnologia(int tecnologia, int estado);
    int GetEstadoTecnologia(int tecnologia);
    void SetNombre(string nombre);
    string GetNombre();
    void SetColor(int color);
    int GetColor();
    int GetDinero();
    void SetDinero(int cantidad);
};  
    
}

#endif
