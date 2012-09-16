/*
 * Clase base de IA encargada de gestionar unidades con movimiento por el mapa
 * Las clases de IA que sean base para otras implementarán unicamente eventos
 * y acciones primitivas, que serán utilizadas por las clases derivadas para
 * implementar el autómata, por está razón el método de actualización es 
 * virtual.
 *
 */
#ifndef __IAUNIDADMOVIL_H__
#define __IAUNIDADMOVIL_H__

#include "logica.h"
#include "lIAUnidad.h"
#include "ufrecev.h"


namespace Logica{

class Unidad;

class IAUnidadMovil  : public IAUnidad
{
      public:
          
          IAUnidadMovil(Unidad * uni);
          ~IAUnidadMovil();

          //Hacer virtual si se hereda de esta clase
          virtual void Actualizar();

             
      protected:
          
          //Eventos
          bool eventoHaLlegado();
          bool eventoColision();
          //Frecuencias de comprobacion
          Util::FrecEv frecevHaLlegado,frecevColision;
                
          //Acciones primitivas
          void PasoAndar();
          void PasoParar();
          void PasoCorrer();
          
          //Acciones auxiliares
          Util::Vector2D AuxMover(const Util::Vector2D &dest);
          float AuxCalcularAngulo(const Util::Vector2D & p);
          void AuxCorregirDir(Util::Vector2D &vel);
          
          //Datos de acciones con movimiento
          Util::Vector2D destino,dir;
                
      private:

              
 
};

}
#endif
