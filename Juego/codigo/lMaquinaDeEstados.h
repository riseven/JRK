 #ifndef MAQUINADEESTADOS_H
#define MAQUINADEESTADOS_H

//------------------------------------------------------------------------
//
//  Clase que encapsula una máquina de estados genérica para ser instanciada
//  con el tipo del dueño de la misma
//
//------------------------------------------------------------------------
#include <cassert>
#include <string>

#include "lEstado.h"
#include "lMensaje.h"

namespace Logica{

template <class entidad>
class MaquinaDeEstados
{
private:

  //Unidad que es dueña de esta maquina de estados
  entidad*          unidad;

  Estado<entidad>*   estadoActual;
  
  Estado<entidad>*   estadoAnterior;

  //Estado utilizado para ser llamado siempre que se ejecuta el AFD
  Estado<entidad>*   estadoGlobal;
  

public:

  MaquinaDeEstados(entidad* uni):unidad(uni),
                                   estadoActual(NULL),
                                   estadoAnterior(NULL),
                                   estadoGlobal(NULL)
  {}

  virtual ~MaquinaDeEstados(){}

  //Métodos de inicialización
  void SetEstadoActivo(Estado<entidad>* s){estadoActual = s;}
  void SetEstadoGlobal(Estado<entidad>* s) {estadoGlobal = s;}
  void SetEstadoAnterior(Estado<entidad>* s){estadoAnterior = s;}
  
  //Actualiza la máquina de estados, ejecutando el estado global y el actual
  void  Actualizar()const
  {
    //si existe un estado global, ejecutarlo
    if(estadoGlobal)   estadoGlobal->Ejecutar(unidad);

    //igual para el estado actual
    if (estadoActual) estadoActual->Ejecutar(unidad);
  }

  //Gestión de mensajes entrantes
  bool  RecibirMensaje(const Mensaje& msg)const
  {
    //Verificar si el estado actual puede manejar el mensaje
    if (estadoActual && estadoActual->RecibirMensaje(unidad, msg))
    {
      return true;
    }
  
    //Si no , intentarlo con el global
    if (estadoGlobal && estadoGlobal->RecibirMensaje(unidad, msg))
    {
      return true;
    }

    return false;
  }

  //Cambio de estado
  void  CambiarEstado(Estado<entidad>* nuevoEstado)
  {
    assert(nuevoEstado && "<MaquinaDeEstados::CambiarEstado>:Intentando asignar null al nuevo estado");

    //Guardarse el estado actual como el previo, antes de cambiar
    estadoAnterior = estadoActual;

    //Salir del estado actual en la maquina
    estadoActual->Salir(unidad);

    //Cambiar el estado
    estadoActual = nuevoEstado;

    //Entrar en el estado nuevo
    estadoActual->Entrar(unidad);
    
    nuevoEstado->AFD = this;
  }

  //Devolver la máquina al estado anterior
  void  RevertirEstado()
  {
    CambiarEstado(estadoAnterior);
  }

  //Comprueba si la máquina está en un estado determinado
  bool  isInEstado(const Estado<entidad>& st)const
  {
    if (typeid(*estadoActual) == typeid(st)) return true;
    return false;
  }

  Estado<entidad>*  GetEstadoActual()  const{return estadoActual;}
  Estado<entidad>*  GetEstadoGlobal()   const{return estadoGlobal;}
  Estado<entidad>*  GetEstadoAnterior() const{return estadoAnterior;}

  //utilizado para obtener el nombre del estado, para depuración
  std::string         GetNombreEstadoActual()const
  {
    std::string s(typeid(*estadoActual).name());

    if (s.size() > 5)
    {
      s.erase(0, 6);
    }

    return s;
  }
};


}

#endif


