#include "lJugador.h"

namespace Logica
{
    
Jugador::Jugador(string nombre)
{
    Jugador::nombre = nombre ;
    dinero = 200.0 ;
}   
    
int
Jugador::GetDinero()
{
    return dinero;
}

void
Jugador::AddDinero(float cantidad)
{
    dinero += cantidad ;
}
    
}  
