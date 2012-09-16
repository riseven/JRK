#ifndef ESTADO_H
#define ESTADO_H
//------------------------------------------------------------------------
//
//  Clase abstracta que define los m�todos de un estado del AFD
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

  //Se ejecutar� al entrar al estado
  virtual void Entrar(entidad*)=0;

  //M�todo de ejeuci�n del estado actual
  virtual void Ejecutar(entidad*)=0;

  //Se ejecutar� al salir del estado
  virtual void Salir(entidad*)=0;

  //Recibe los mensajes que lleguen al CentroDeMensajes
  virtual bool RecibirMensaje(entidad*, const Mensaje&)=0;
  
  MaquinaDeEstados<entidad> *AFD;
};

}

#endif
