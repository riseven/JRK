/*
 * Definici�n Clase Unidad Din�mica:
 *   -Ser� la unidad normal, tendr� IA asociada y dispondr� de todos los subsistemas.
 *           -Ej de unidades din�micas:
                 - Soldados, veh�culos, etc.
 *
 */

#ifndef __lUNIDADDINAMICA_H_
#define __lUNIDADDINAMICA_H_

#include "logica.h"
#include "uVector2D.h"
#include "lMensaje.h"
#include "lUnidad.h"


namespace Logica{

class UnidadDinamica : public Unidad
{
            
      public:
                  
         UnidadDinamica(int ID,
               Util::Vector2D pos, 
               int vidaMax, 
               double fuerzaMax, 
               double velocidadMax,
               double velocidadGiroMax,
               double campoDeVision,
               double distanciaDeVision,
               int eq,
               double masa); 

         double getFuerzaMaxima() const { return fuerzaMaxima; }
         double getVelocidadMaxima() const { return velocidadMaxima; }
         double getMasa() const { return masa; }
         double getDistanciaDefensa() const { return distanciaDefensa; }

         //Atributos moficicables desde fuera de la unidad
         Util::Vector2D getVelocidad() const { return velocidad; }
         void     setVelocidad(const Util::Vector2D & vel) { velocidad=vel; }
         
         //Cualquier clase que quiera recibir mensajes deber� implementar
         //este m�todo y si no puede manejar los mensajes recibidos devovler� false
         virtual bool RecibirMensaje(const Mensaje &msg);
         
         //Actualiza la unidad
         virtual void Actualizar()=0;
         
        
         // M�todos IA
         bool HaLlegado(const Util::Vector2D &);
         void MirarA(const Util::Vector2D & );


      protected:         
         
         //
         // Datos de una unidad din�mica
         //
         Util::Vector2D    velocidad;
         //Util::Vector2D    lado;
         double      masa;
         double      velocidadMaxima;
         double      fuerzaMaxima;
         double      velocidadGiroMaxima;
         double      distanciaDefensa;
         
         double      angulo;
         
   
};

}

#endif
