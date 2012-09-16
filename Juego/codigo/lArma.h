#ifndef __L_ARMA_H__
#define __L_ARMA_H__

#include "uWinDefs.h"
#include "lUnidad.h"
#include "uVector3D.h"

namespace Logica
{

class Arma
{
      protected:
                
                Unidad*        unidad; //Unidad dueña de este arma
                unsigned int   tipo;
                double         frecuenciaDisparo; //Numero de disparos por segundo
                double         tiempoEstaraDisponible;
                double         maximaDistancia;
                double         velocidadMaximaProyectiles;
                double         tiempoDeCarga;


                bool           estaListoParaDisparar();
                void           ActualizaTiempoSiguienteDisparo();
                bool           estaDisparoOcluido( const Unidad * );
  
      public:
    
               Arma(unsigned int Tipo,
                    double       frecuencia,
                    double       distancia,
                    double       velocidad,
                    double       tiempoCarga,
                    Unidad*      uni);
               
               virtual ~Arma(){}

               //Permite saber a priori si el disparo se va a poder realizar (aunque posteriormente falle)
               virtual bool  esDisparable( Unidad * uni) = 0;

               virtual void  Disparar(const Util::Vector3D &pos) = 0;

               double        GetVelocidadMaxima() const { return velocidadMaximaProyectiles; }

               unsigned int  GetTipo() const{ return tipo; }
               
               double        GetDistanciaMaxima() const{ return maximaDistancia; }
};


inline bool Arma::estaListoParaDisparar()
{
  if ( Util::GetTiempoActual() > tiempoEstaraDisponible )
    return true;

  return false;
}

//-----------------------------------------------------------------------------
inline void Arma::ActualizaTiempoSiguienteDisparo()
{
  tiempoEstaraDisponible = Util::GetTiempoActual() + 1000.0/frecuenciaDisparo;
}

}

#endif
