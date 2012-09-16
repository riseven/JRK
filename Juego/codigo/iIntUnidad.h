#ifndef __I_INT_UNIDAD__
#define __I_INT_UNIDAD__

#include "iUnidadUniSeleccionable.h"
#include "iUnidad.h"

namespace Interfaz {

//Clase que implementa el interfaz de la unidad seleccionada
class IntUnidad 
{
   private:
      int imgUnidad;
      int btnUnidad;
      int imgFrente;      
      int posX, posY;
      bool onClick;
      //float px,py,pz;
      UnidadSeleccionable *ultUnidad;
   
   public:
      IntUnidad();
      ~IntUnidad();
      void CambiarImagenUnidad(UnidadSeleccionable *unidad);
      void Activar();
      void Desactivar();
      void Actualizar();
};

}

#endif
