#include "uWinDefs.h"
#include "lCentroDeMensajes.h"
#include "lUnidad.h"
#include "lApi.h"

using std::set;

using namespace Logica;

bool CentroDeMensajes::Descargar(Unidad* destino, const Mensaje& mensaje)
{
  if (!destino->RecibirMensaje(mensaje))
  {
    //mensaje no manejado
    #ifdef _DEBUG_
    printf("ATENCION: Mensaje no manejado\n");
    #endif
    
    return false;
  }
  else
    return true;
}

bool CentroDeMensajes::EnviarMensaje(double       retraso,
                                    int          origen,
                                    int          dest,
                                    int          msg,
                                    void*        datos = NULL)
{

  Unidad* destino = Logica::Api::GetUnidad(dest);


  if (destino == NULL)
      return false;
  
  
  //nuevo mensaje
  Mensaje mensaje(0, origen, dest, msg, datos);
  
  //Si no hay retraso , mandar el mensaje                      
  if (retraso <= 0.0)                                                        
  {
    //Enviar
    return Descargar(destino, mensaje);
  }

  //sino asignar un tiempo de envio y guardar
  else
  {
    double tiempo= Util::GetTiempoActual();

    mensaje.tiempoDeEnvio = tiempo + retraso;

    //y añadirlo al a cola
    PriorityQ.insert(mensaje);   
  }
}


void CentroDeMensajes::EnviarMensajesAtrasados()
{ 

  double tiempo = Util::GetTiempoActual(); 

  //Recoger todos los mensajes cuyo tiempo haya expirado y enviarlos
  while( !PriorityQ.empty() &&
	     (PriorityQ.begin()->tiempoDeEnvio < tiempo) && 
         (PriorityQ.begin()->tiempoDeEnvio > 0) )
  {
    const Mensaje& mensaje = *PriorityQ.begin();

    Unidad* destino = Logica::Api::GetUnidad(mensaje.destino);

    //Enviarlo
    Descargar(destino, mensaje);

	  //y eliminarlo de la cola
    PriorityQ.erase(PriorityQ.begin());
  }
}



