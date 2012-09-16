#ifndef ESTADO_H
#define ESTADO_H
//------------------------------------------------------------------------
//
//  Clase abstracta que define los métodos de un estado del AFD
//
//------------------------------------------------------------------------

namespace Logica{

class Mensaje;

template <class T>
class MaquinaDeEstados;

template <class entidad>
class Estado
{
public:

  virtual ~Estado(){}

  //Se ejecutará al entrar al estado
  virtual void Entrar(entidad*)=0;

  //Método de ejeución del estado actual
  virtual void Ejecutar(entidad*)=0;

  //Se ejecutará al salir del estado
  virtual void Salir(entidad*)=0;

  //Recibe los mensajes que lleguen al CentroDeMensajes
  virtual bool RecibirMensaje(entidad*, const Mensaje&)=0;
  
  MaquinaDeEstados<entidad> *AFD;
};

}

#endif
