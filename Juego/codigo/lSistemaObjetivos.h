#ifndef SISTEMA_DE_OBJETIVOS_H
#define SISTEMA_DE_OBJETIVOS_H

//-----------------------------------------------------------------------------
// Clase que permitirá elegir un objetivo de los que se encuentren en la memoria
//-----------------------------------------------------------------------------
#include "uVector2D.h"
#include <list>

namespace Logica{

class Unidad;


class SistemaObjetivos
{
private:

  //Unidad de este sistema
  Unidad*  unidad;

  //Objetivo actual
  Unidad*  objetivoActual;
  
  //Objetivo, que puede no ser visible actualmente, pero del que se sabe su última posicion
  Unidad*  objetivoPotencial;


public:

  SistemaObjetivos(Unidad* unidad);

  //Elige un objetivo
  void       Actualizar();

  bool       hayObjetivo()const{return objetivoActual != NULL;}

  //Devuelve true si la unidad esta viendo al objetivo
  bool       esObjetivoVisible()const;

  //Devuelve true si el objetivo es disparable (no hay durezas entre ambos)
  bool       esObjetivoDisparable()const;

  //Devuelve la posicion del objetivo, o lanza una exc. si no existe
  Util::Vector2D   GetPosicionObjetivo()const;

  //Devuelve el tiempo que ha sido visible
  double      GetTiempoObjetivoHaSidoVisible()const;

  //Deuvleve el tiempo que lleva sin verse al objetivo
  double      GetTiempoSinVerObjetivo()const;
  
  //Obtiene el puntero al objetivo
  Unidad* GetObjetivo()const{ return objetivoActual; }

  //Elimina el objetivo actual
  void       LimpiarObjetivo(){objetivoActual=NULL;}
};


}

#endif
