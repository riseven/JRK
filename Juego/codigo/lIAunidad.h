/*
 *
 *
 */
#ifndef __lIA_UNIDAD_H_
#define __lIA_UNIDAD_H_

#include "uFrecEv.h"
#include "lUnidadDinamica.h"
#include "lSistemaSensorial.h"
#include "lSistemaObjetivos.h"
#include "lSistemaMotor.h"
#include "lMaquinaDeEstados.h"
#include "lMensaje.h"


namespace Logica{

class Unidad;
class UnidadDinamica;


class IAUnidad
{
      public:

           static const int MODO_PASIVO = 1;
           static const int MODO_DEFENSIVO = 2;
           static const int MODO_AGRESIVO = 3;
           static const int MODO_MANTENERTERRENO = 4;

          
          //Constructor a partir de una referencia a una unidad
          IAUnidad(Unidad * uni);
          ~IAUnidad();
                    
          SistemaSensorial* GetMemoria() const { return memoria; }
          SistemaObjetivos* GetObjetivo() const { return objetivo; }
          SistemaMotor*     GetMovimiento() const { return movimiento; }
          MaquinaDeEstados<UnidadDinamica>* GetAFD() const {return afd; }
          
          void ElegirObjetivo(bool e) { elegir_objetivo=e; }
          Util::Vector2D& getObjetivoIA() { return objetivo_ia; }
          void setObjetivoIA( const Util::Vector2D & v ) { objetivo_ia = v; }
          
          void GuardarReposo() { posicion_reposo=unidad->getPosicion(); direccion_reposo=unidad->getDireccion(); }
          Util::Vector2D& getPosicionReposo() { return posicion_reposo; }
          void setPosicionReposo( const Util::Vector2D & v ) { posicion_reposo=v; }
          Util::Vector2D& getDireccionReposo() { return direccion_reposo; }
          void setDireccionReposo( const Util::Vector2D & v ) { direccion_reposo=v; }
          
          void setModo(int m) { modo=m; }
          int  getModo() const { return modo; }
          
          virtual void Actualizar();
          
          virtual bool RecibirMensaje(const Mensaje &msg);
             
             
      protected:
                void setEstado(int est) { estado=est; } 
                int getEstado() { return estado; }

                               
                //Unidad a la que dirige, sólo será dirigible a las dinámicas
                UnidadDinamica * unidad;
                
      private:
              int accion,estado,modo;
              
              //Sistema sensorial con memoria
              SistemaSensorial *memoria;
              //Sistema de objetivos
              SistemaObjetivos *objetivo;
              //Maquina de estados
              MaquinaDeEstados<UnidadDinamica> *afd;
              //Sistema motor
              SistemaMotor *movimiento;
              
              Util::FrecEv frecVision, frecObjetivo, frecprueba;
              
              bool elegir_objetivo;
              
              Util::Vector2D objetivo_ia;
              
              Util::Vector2D posicion_reposo;
              Util::Vector2D direccion_reposo;
              
              
};

}

#endif
