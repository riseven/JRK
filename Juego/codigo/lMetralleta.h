#ifndef __L_METRALLETA_H__
#define __L_METRALLETA_H__

#include "lArma.h"

namespace Logica
{

class Metralleta : public Arma
{
  
      public:
    
               Metralleta( Unidad * uni):Arma(Red::Protocolo::Armas::Metralleta,
                                              15,
                    double       distancia,
                    double       velocidad,
                    double       tiempoCarga,
                    Unidad*      uni);
               
               virtual ~Arma(){}

               //Permite saber a priori si el disparo se va a poder realizar (aunque posteriormente falle)
               virtual bool  esDisparable( Unidad * uni) = 0;

               virtual void  Disparar(const Util::Vector3D &pos) = 0;

};


#endif
