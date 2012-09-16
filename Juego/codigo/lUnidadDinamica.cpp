/*
 * Implementación Clase UnidadDinamica:
 * 
 */

#include "lIAunidad.h"
#include "lUnidad.h"
#include "lUnidadDinamica.h"
#include "lenum_mensajes.h"


using namespace Logica;
using namespace Util;

/*
 * Constructor
 */
UnidadDinamica::UnidadDinamica(int ID,
               Vector2D pos, 
               int vidaMax, 
               double fuerzaMax, 
               double velocidadMax,
               double velocidadGiroMax,
               double campoDeVision,
               double distanciaDeVision,
               int eq,
               double Masa):Unidad(ID,pos,vidaMax,campoDeVision,distanciaDeVision,eq),

                         fuerzaMaxima(fuerzaMax), 
                         velocidadMaxima(velocidadMax),
                         velocidadGiroMaxima(velocidadGiroMax),
                         masa(Masa)
{
                         setFija(false);
}


/*
 * Manejador de mensajes
 *  -Intenta manejar el mensaje, si no puede se lo pasa al padre,
 *  y si él no puede, se lo pasará a la IA si existe
 */
bool
UnidadDinamica::RecibirMensaje(const Mensaje &mensaje)
{      
       
       //Implementado en el recibirMensaje de la IA
/*       switch ( mensaje.msg )
       {
              case AC_MoverUnidad:
                 //Se mueve la unidad utilizando el método implementado en la IA  
                 ia->MoverUnidad();
              return true;
              
              case AC_DetenerUnidad:
                 //Se detiene la unidad utilizando el método implementado en IA
                 DetenerUnidad();                   
              return true;
       };*/
       
        //Pasa la pelota
        return Unidad::RecibirMensaje(mensaje); // || ia->RecibirMensaje(mensaje);           
}

//Ha llegado si la distancia que recorrerá en la siguiente iteracion sobrepasa
//el destino, es decir, la velocidad es mayor que la distancia que le separa del
//destino
bool
UnidadDinamica::HaLlegado(const Vector2D & destino)
{
  float velCuad = velocidadMaxima*velocidadMaxima;//velocidad*velocidad;
  float distCuad = Vec2DDistanciaCuad( posicion , destino );

  return (velCuad>=distCuad);
}

void
UnidadDinamica::MirarA(const Vector2D & destino)
{
  direccion= Vec2DNormalizar( destino-posicion );
}
