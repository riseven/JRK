/*
 * Implementación Clase UnidadEstatica:
 * 
 */

#include "lUnidad.h"
#include "lUnidadEstatica.h"
#include "lenum_mensajes.h"


using namespace Logica;
using namespace Util;

/*
 * Constructor
 */
UnidadEstatica::UnidadEstatica(int ID,
               Vector2D pos, 
               int vidaMax,
               double campoDeVision,
               double distanciaDeVision,
               int eq):Unidad(ID,pos,vidaMax,campoDeVision,distanciaDeVision,eq)
{
   memoria=NULL;
   setFija(true);
}


/*
 * Manejador de mensajes
 *  -Intenta manejar el mensaje, si no puede se lo pasa al padre,
 *  y si él no puede, se lo pasará a la IA si existe
 */
bool
UnidadEstatica::RecibirMensaje(const Mensaje &mensaje)
{      
        //Pasa la pelota
        return Unidad::RecibirMensaje(mensaje);
}
