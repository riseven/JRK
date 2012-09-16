#ifndef __L_JUGADOR_H__
#define __L_JUGADOR_H__

#include <string>

/*
 *  Clase que guarda informacion de los jugadores de la partida
 *  ya sean humanos o IA
 */

using namespace std;

namespace Logica
{

class Jugador
{
private:
    string nombre ;
    int color ;
    float dinero ;
public:
    Jugador(string nombre);   
    string GetNombre(){return nombre;}
    int GetColor(){return color;}
    void SetColor(int color){Jugador::color = color;}
    int GetDinero();
    void AddDinero(float cantidad);
};   
    
}

#endif
