/*
 * Unidad que contendrá un recurso determinado
 * , es una unidad sin IA, totalmente estático
 * contiene un valor que representa la cantidad
 * 
 */
 
#ifndef __UNIDADRECURSO_H__
#define __UNIDADRECURSO_H__

#include "logica.h"
#include "lUnidad.h"


namespace Logica{
class UnidadRecurso : public Unidad
{
      UnidadRecurso(int ID,
               Util::Vector2D pos, 
               int valor);
               
      void Actualizar() {}
               
      private:
              int valor;

};
}
#endif
