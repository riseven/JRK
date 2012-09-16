#ifndef MENSAJE_H
#define MENSAJE_H
//------------------------------------------------------------------------
//
// Clase que guarda un mensaje que se utilizará para la comunicación entre agentes del juego
//
//------------------------------------------------------------------------
#include <iostream>
#include <math.h>

namespace Logica{

struct Mensaje
{
  // ID de la unidad origen
  int          origen;

  // ID de la unidad destino
  int          destino;

  //El id mensaje del que se trata, 
  int          msg;

  //Para poder retrasar el envio de mensajes, tiempo en milisegundos
  double       tiempoDeEnvio;

  //Datos del mensaje, cada receptor del mensaje sabrá tratar los datos
  void*        datos;


  Mensaje():tiempoDeEnvio(-1),
                  origen(-1),
                  destino(-1),
                  msg(-1)
  {}


  Mensaje(double time,
           int    origen,
           int    destino,
           int    msg,
           void*  info = NULL): tiempoDeEnvio(time),
                               origen(origen),
                               destino(destino),
                               msg(msg),
                               datos(info)
  {}
  
  ~Mensaje()
  {
     if (datos) free(datos);//delete datos ;
     datos=NULL;
  }
 
};


const double minimoRetraso = 0.25;


//Dos mensajes son iguales si lo son en todos los campos y diferencian como mucho en "minimoRetraso" en el tiempo de envio
inline bool operator==(const Mensaje& t1, const Mensaje& t2)
{
  return ( fabs(t1.tiempoDeEnvio-t2.tiempoDeEnvio) < minimoRetraso) &&
          (t1.origen == t2.origen)        &&
          (t1.destino == t2.destino)    &&
          (t1.msg == t2.msg);
}

inline bool operator<(const Mensaje& t1, const Mensaje& t2)
{
  if (t1 == t2)
  {
    return false;
  }

  else
  {
    return  (t1.tiempoDeEnvio < t2.tiempoDeEnvio);
  }
}

inline std::ostream& operator<<(std::ostream& os, const Mensaje& t)
{
  os << "time: " << t.tiempoDeEnvio << "  origen: " << t.origen
     << "   destino: " << t.destino << "   Msg: " << t.msg;

  return os;
}

//Función para convertir al tipo deseado el puntero a void de los datos. Uso: TipoA a=ConvertirA<TipoA>(datos);
template <class T>
inline T ConvertirA(void* p)
{
  return *(T*)(p);
}

}

#endif
