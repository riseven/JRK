#ifndef ENUM_MENSAJES_H
#define ENUM_MENSAJES_H

#include <string>

namespace Logica{

#define SIN_RETARDO 0.0
#define ORIGEN_INNECESARIO -1

//
// Definición de todos los mensajes
//
enum tipo_mensaje
{
  MSG_Vacio=0,
  MSG_TeHeDado,
  MSG_TomaVida,
  MSG_EstasMuerto,
  MSG_SonidoDeArma,
  MSG_Refuerzos,
  
    // Cuartel General
  MSG_ConstruirIngeniero,
  MSG_DesarrollarMotivacion,
  MSG_DesarrollarMotivacionMasDistancia,
  
  // Barracones
  MSG_ConstruirSoldado,
  MSG_DesarrollarArmaduraSoldado,
  MSG_DesarrollarVelocidadDisparoSoldado,
  
//  Acciones       -     Parametros
  
  AC_MoverUnidad,     //(float x, float z); Desplaza a la unidad a un punto del mapa
  AC_MoverAgresivo,   //(float x, float z); Mueve a un punto de forma agresiva, disparando a los objetivos que encuentre
  AC_DetenerUnidad,   //();
  AC_AtacarUnidad,    //(int handlerObjetivo); 
  AC_SeguirUnidad,    //(int handlerObjetivo);
  AC_AtacarZona,      //(float x, float z);  Se mueve aleatoriamente alrededor de un punto, atacando enemigos 
  AC_AccionDefecto,   //()
  AC_CambiarArma,     //(int idArma )
  AC_CambiarModo,     //(int idModo )
  
  
//  Mensajes relacionados con una unidad poseida (Bajo el control del usuario en 1ª persona)

  MSG_Poseer,         //Notifica a una unidad para que desactive la IA, y la prepare para recibir ordenes 
  MSG_Desposeer,      //Restaura la IA, y restablece la unidad en modo automatico
  
  MSG_POS_Adelante,   //Pulsación de tecla para ir adelante
  MSG_POS_Atras,      //Pulsación de tecla para ir atras
  MSG_POS_Izquierda,  //Pulsación de tecla para ir a la izquierda
  MSG_POS_Derecha,    //Pulsación de tecla para ir a la derecha
  
  MSG_POS_CambiarArma,//Cambia el arma a la indicada
  MSG_POS_Disparar    //Indica que desea hacer un disparo con el arma actual, se indicará dirección de disparo
  
};

//Para depurar
inline std::string MensajeAString(int msg)
{
  switch(msg)
  {
  case MSG_Vacio:

    return "MSG_Vacio";

  case MSG_TeHeDado:

    return "MSG_TeHeDado";
    
  case   MSG_TomaVida:
  
    return "MSG_TomaVida";

  case MSG_SonidoDeArma:

    return "MSG_SonidoDeArma";

  default:

    return "Mensaje no definido";
  }
}

}

#endif
