/*
 * Definición Clase Unidad Estática:
 *            - Clase más ligera pensada para unidades sin movimiento y con IA nula 
 *            o muy reducida que se implementará en la propia unidad, para ello 
 *            dispondrá del subsistema sensorial.
 *            
 *            Ej de unidades estáticas:
 *                  - Torretas de vigilancia.
 *
 */

#ifndef __lUNIDADESTATICA_H_
#define __lUNIDADESTATICA_H_

#include "logica.h"
#include "uVector2D.h"
#include "lMensaje.h"
#include "lUnidad.h"
#include "lSistemaSensorial.h"

namespace Logica{

class UnidadEstatica : public Unidad
{
            
      public:
                  
         UnidadEstatica(int ID,
               Util::Vector2D pos, 
               int vidaMax,
               double campoDeVision,
               double distanciaDeVision,
               int eq); 


         //Cualquier clase que quiera recibir mensajes deberá implementar
         //este método y si no puede manejar los mensajes recibidos devovlerá false
         virtual bool RecibirMensaje(const Mensaje &msg);
         
         //Actualiza la unidad
         virtual void Actualizar()=0;
         
      protected:
                //Sistema mínimo para percibir el mundo y poder actuar
                SistemaSensorial *memoria;

};

}

#endif
