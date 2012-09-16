/*
 * Definici�n Clase Unidad Est�tica:
 *            - Clase m�s ligera pensada para unidades sin movimiento y con IA nula 
 *            o muy reducida que se implementar� en la propia unidad, para ello 
 *            dispondr� del subsistema sensorial.
 *            
 *            Ej de unidades est�ticas:
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


         //Cualquier clase que quiera recibir mensajes deber� implementar
         //este m�todo y si no puede manejar los mensajes recibidos devovler� false
         virtual bool RecibirMensaje(const Mensaje &msg);
         
         //Actualiza la unidad
         virtual void Actualizar()=0;
         
      protected:
                //Sistema m�nimo para percibir el mundo y poder actuar
                SistemaSensorial *memoria;

};

}

#endif
