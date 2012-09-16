/*
 * Implementación Clase Unidad:
 * 
 */

#include "lIAunidad.h"
#include "lUnidad.h"
#include "lenum_mensajes.h"


using namespace Logica;
using namespace Util;

/*
 * Constructor
 */
Unidad::Unidad(int ID,
               Vector2D pos, 
               int vidaMax,
               double campoDeVision,
               double distanciaDeVision,
               int eq):
                         id(ID),
                         posicion(pos),
                         vidaMaxima(vidaMax),
                         fov(campoDeVision),
                         rov(distanciaDeVision),
                         vida(vidaMax),
                         equipo(eq)
                 
{
    ia=NULL; 

    moral=0;
    //cada unidad sabrá a que bando pertenece y de que raza es
    bando=0;
    raza=0;
    radio=5.0;
    //Por defecto las unidades son fijas
    fija=true;
    
    estadoInterno=vivo;
}

/*
 * Métodos para utilizar la IA de la unidad
 *
 */

bool 
Unidad::asociaIA(int tipoIA)
{
     IAUnidad * tempIA=NULL;
     
     tempIA = new IAUnidad(this);
     
     /*
     switch (tipoIA)
     {
            
            //se crea una nueva instancia para cada tipo de IA
            case IAUnidad::MOVIL://(En un futuro no se podrán crear IA's de movil directamente
                 tempIA= new IAUnidadMovil(this);
            break;
            
            case IAUnidad::OFENSIVA:
                 tempIA =new IAUnidadOfensiva(this);
            break;
            default:
                    //Si llega aquí el tipo no es valido, y la IA no se tocará
            break;
            
     }*/
     
     //Si se ha podido instanciar...
     if (tempIA!=NULL)
     {
     
                  if (ia!=NULL) delete ia;
                  ia=tempIA;
     }
     
     return (ia!=NULL);  
}
 
void
Unidad::quitaIA()
{
     if (ia!=NULL)
     {
                  delete ia;
                  ia=NULL;
     }
}


/*
 * Oculta la llamada directa a Actualizar de la IA
 */
void
Unidad::ActualizarIAUnidad()
{
     if (ia!=NULL)
        ia->Actualizar();
        
        
     #ifdef __DEBUG_LOGICA
     else printf("lUnidad.cpp:ActualizaIAUnidad: Atención ia sin asociar\n");
     #endif
}

/*
 * Manejador de mensajes
 *  -Intenta manejar el mensaje, si no puede se lo pasa a la IA
 */
bool
Unidad::RecibirMensaje(const Mensaje &mensaje)
{
        switch ( mensaje.msg )
        {
               case MSG_TeHeDado:
               
                        //Se espera un entero 
                        vida-=(int)mensaje.datos;
                        
                        if (vida<0)
                        {
                          vida=0;
                          estadoInterno=muerto;
                        }
                          
                    return true;
                    
               case MSG_TomaVida:
                    
                        //Se espera un entero 
                        vida+=(int)mensaje.datos;
                        
                        if (vida>vidaMaxima)
                          vida=vidaMaxima;
                    
                    return true;
        };
        
        //Se pasa la pelota a la IA si existe
        return ( ia && ia->RecibirMensaje(mensaje) );           
}
