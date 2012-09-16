#ifndef __L_PROYECTIL_H__
#define __L_PROYECTIL_H__

#include "uWinDefs.h"
#include "uVector3D.h"
#include "lUnidad.h"

namespace Logica
{

class Proyectil : public Unidad
{

      protected:

                Util::Vector3D      destino;
                Unidad*       uniDestino;
                
                Util::Vector3D      origen;
                Unidad*       uniOrigen;
                
                int           danyo;
                
                bool          muerto;
                
                bool          impactado;
                
                Util::Vector3D      puntoChoque;
                
                double       tiempoCreacion;
                
      public:
             
             Proyectil( int id,
                   int      damage,  
                   double    mass,
                   double    velMax,
                   double    fuerzaMax): Unidad( id,
                                                 Util::Vector2D(), 
                                                 0, 
                                                 0,
                                                 0,
                                                 -1),

                                        muerto(false),
                                        impactado(false),
                                        danyo(damage)
                

                  { tiempoCreacion = Util::GetTiempoActual(); }


            bool estaMuerto()const{ return muerto; }  
            
            bool haImpactado()const{ return impactado; }
              
            void setOrigenYDestino( Unidad* ori, Unidad* dst) { uniOrigen=ori; uniDestino=dst; }

};


};



#endif
