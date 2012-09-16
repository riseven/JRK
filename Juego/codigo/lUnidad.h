/*
 * Definición Clase Unidad:
 *   -Es la clase base de todas las unidades,
 *    define atributos y métodos comunes.
 * 
 *   -Mantiene actualizada su posición utilizando
 *    la velocidad simbólica y direccion devueltas
 *    por la IA en cada iteración
 *
 */

#ifndef __lUNIDAD_H_
#define __lUNIDAD_H_

#include "logica.h"
#include "uVector2D.h"
#include "lMensaje.h"


namespace Logica{

      enum EstadoInterno{
           muerto,
           vivo
      };

class Api;
class IAUnidad;

class Unidad
{
            
      public:

             
         //Unidades definidas en rProtocolo.h
         
           
         Unidad(int ID,
               Util::Vector2D pos, 
               int vidaMax, 
               double campoDeVision,
               double distanciaVision,
               int eq); 
         
         //Atributos moficicables desde fuera de la unidad
        const Util::Vector2D& getPosicion() const { return posicion; }
         void     setPosicion(const float x, const float y) { posicion.setX(x); posicion.setY(y); }
         void     setPosicion(const Util::Vector2D & pos) { posicion=pos; }
         
         Util::Vector2D getDireccion() { return direccion; }
         void     setDireccion(const Util::Vector2D &v) { direccion=v; }
         
         double   getFOV() const { return fov; }         
         void     setFOV(float f) { fov=f; }
         
         double   getROV() const { return rov; }
         double   getROVCuad() const { return rov*rov; }
         void     setROV(float f) { rov=f; }
         
         double   getRadio() const { return radio; }
         void     setRadio( double rad ) { radio=rad; }
         
         bool     esFija() const { return fija; }
         void     setFija(bool f) { fija=f; }
         
         void     setEquipo(const int eq) { equipo=eq; }
         int      getEquipo() const { return equipo; }
         
         //Cualquier clase que quiera recibir mensajes deberá implementar
         //este método y si no puede manejar los mensajes recibidos devovlerá false
         virtual bool RecibirMensaje(const Mensaje &msg);
         
         //Id asignado
         int getID() const { return id; }
         
         int getVida() const { return vida; }
         
         bool estaVivo() const { return estadoInterno==vivo; }
         
         IAUnidad* getIA() const { return ia; }
         
         //Actualiza la unidad
         virtual void Actualizar()=0;
                  
      protected:
         void setVida(int v) { vida=v; }
         
         // Métodos IA
         bool asociaIA(int tipoIA); //Establece una nueva IA para el jugador,borra la anterior si ya tenía una
         void quitaIA(); //Borra la IA, obligatorio cuando se destruye la clase
         //Encapsula la llamada a Actualiza de IAUnidad para las clases derivadas
         void ActualizarIAUnidad();
         
                 
         //
         // Datos de una unidad básica
         //
         Util::Vector2D    posicion;
         Util::Vector2D    direccion;
         int         vida;
         int         vidaMaxima;
         int         moral;
         int         moralMaxima;
         double      fov;
         double      rov;
         double      radio;
         int         bando;
         int         raza;
         int         equipo;
         bool        fija; //Indica si la unidad se puede mover
         
         EstadoInterno estadoInterno;
          
      private:
         //ID que asignará la API a esta unidad
         int id;
         IAUnidad *ia; //IA asociada a esta unidad     

      
};

}

#endif
