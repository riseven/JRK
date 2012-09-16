#ifndef CENTRODEMENSAJES_H
#define CENTRODEMENSAJES_H

//------------------------------------------------------------------------
//
//  Se encarga de gestionar los mensajes de la aplicación
//
//------------------------------------------------------------------------

#include <set>
#include <string>
#include "lMensaje.h"
#include "lUnidad.h"

namespace Logica{


const double ENVIA_MENSAJE_YA = 0.0;
const int    SIN_DATOS   = 0;
const int    ORIGEN_NO_NECESARIO = -1;


class CentroDeMensajes
{
private:  
  
  //Se guardan los mensajes retrasados en un conjunto, para evitar duplicados
  std::set<Mensaje> PriorityQ;

  //Función interna que descarga el mensaje de la lista y lo envia al destino
  bool Descargar(Unidad* destino, const Mensaje& msg);


public:

  CentroDeMensajes(){}
  
  CentroDeMensajes(const CentroDeMensajes&);
  CentroDeMensajes& operator=(const CentroDeMensajes&);

  //Función principal, utilizada para almacenar un mensaje
  bool EnviarMensaje(double      retardo,
                   int         origen,
                   int         destino,
                   int         msg,
                   void*       datos);

  //Envia los mensajes atrasados que pudiera haber.
  void EnviarMensajesAtrasados();
};

}

#endif
