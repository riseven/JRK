/*
 *  Unidad de batalla básica, por ahora utilzará la IAUnidadMovil
 */
 
#ifndef __UNIDAD_SOLDADO__
#define __UNIDAD_SOLDADO__

#include "uUtils.h"
#include "lUnidadDinamica.h"

namespace Logica{

class UnidadSoldado : public UnidadDinamica
{
      public:
            static const int VIDAMAXIMA=100;
            static const float VELOCIDADMAXIMA=0.1;
            static const float FUERZAMAXIMA=4.0;
            static const float VELGIROMAXIMA=1.0;
            static const float CAMPODEVISION=DEGTORAD(135.0);
            static const float DISTANCIADEVISION=250.0;
            static const float DISTANCIADEFENSA=25.0;
            static const int EXPMAX=10;
            static const double MASA=1.0;
            
             
             
            UnidadSoldado(int ID, Util::Vector2D pos, int equipo );
               
            ~UnidadSoldado();
      
            void Actualizar();
            
      private:
              int experiencia;

};

}

#endif
