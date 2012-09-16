/*
 * IA básica de guerra, ahora derivará de IAUnidadMovil,
 * en un futuro una unidad de IA mas sofisticada podrá derivar
 * tanto de IAUnidadMovil como de IAUnidadOfensiva con herencia
 * múltiple.
 *
 * Recibirá acciones dirigidas a otras unidades y a posiciones
 * en el terreno (ataque y movimiento) 
 */
 
#ifndef __IAUNIDADOFENSIVA_H__
#define __IAUNIDADOFENSIVA_H__

#include "logica.h"
#include "lIAUnidadMovil.h"
#include "ufrecev.h"


namespace Logica{
          
class IAUnidadOfensiva : public IAUnidadMovil
{
      public:
          
          IAUnidadOfensiva(Unidad * uni);
          ~IAUnidadOfensiva() {} 

          //Hacer virtual si se hereda de esta clase
          void Actualizar();
          

             
      protected:
          
          //Eventos
          bool eventoEnemigoVisible();
          bool eventoObjetivoVisible();
          bool eventoEnemigoCerca();
          bool eventoObjetivoCerca();
          bool eventoEnemigoMuerto();
          bool eventoObjetivoMuerto();
          bool eventoPocaVida();
          bool eventoLejosInicio();
          bool eventoObstaculo();
          
                 
          //Acciones primitivas
           //Dispara arma de fuego y calcula un daño al enemigo
          void PasoDisparar(int unidest);
           //Ataque cuerpo a cuerpo
          void PasoAtacar();
          
          //Acciones auxiliares
          int  AuxBuscarMejorEnemigo();
          bool AuxHayObstaculos(int uni);
          bool AuxHayEdificios(float x,float y,float x2,float y2);
          bool AuxEsVisiblePorUnidad(int uni);


          //Estado actual del automata
          int estado;
          
          //Datos para accion con objetivo
          int unidad_objetivo;
          Vector2D posicion_objetivo;
                
      private:
              //Estados internos del autómata de esta IA
              
              static const int ESPERANDO=1;
              static const int MIRANDO=2;
              static const int DISPARANDO=3;
              static const int VOLVER_A_ESPERANDO=4;
              static const int CORRIENDO=5;
              static const int CORREGIR_CORRIENDO=6;
              static const int ESPERANDO_A_ATACAR=7;
              static const int CORRER_A_ENEMIGO=8;
              static const int CORREGIR_CORRIENDO_ATACANDO=9;
              static const int DISPARAR_CORRIENDO=10;
              static const int HUIR=11;
              static const int PARAR_DE_HUIR=12;
              static const int DISPARAR_SIN_PARAR=13;
              
              //Memoria del automata
              Vector2D ultima_direccion,ultima_posicion,posicion_inicial,ultima_posicion_enemigo;
              int ultimo_enemigo_visto;
              

        

};
}

#endif
