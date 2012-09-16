#ifndef __INTERFAZPANEL__
#define __INTERFAZPANEL__

#include <allegro.h>
#include <alleggl.h>

#include "iIntAcciones.h"
#include "iIntUnidad.h"
#include "iIntSeleccion.h"
#include "iIntMinimapa.h"
#include "iIntBarraSuperior.h"
#include "gImagen.h"
#include "iCamara.h"
#include <string>

//using namespace std;

namespace Interfaz {

//Clase que impementa el interfaz grafico del juego
class InterfazPanel
{
   private:
      IntMinimapa *intMinimapa;
      IntUnidad *intUnidad;
      IntSeleccion *intSeleccion;
      IntAcciones *intAcciones;
      IntBarraSuperior *intBarraSuperior;
      int imgPanel;
      int sprPanel;
      int posX, posY;
      string strInterfaz;
      
      bool visible ;
      
   public:
      InterfazPanel();                       //Carga el panel
      ~InterfazPanel();                      //Destruye el panel
      void Actualizar();                     //Comprueba eventos en el panel
      void Activar();
      void Desactivar();
      
      
      bool SetMinimapa(string nombre);       //Cambia el minimapa
      void SetUnidad(int );                  //Muestra info de unidad
      void AnyadeSeleccion(list <int >);     //Rellena panel con lista de unidades
      void AnayadeUnidad(int );              //Anyade unidad al panel
      void QuitaUnidad(int );                //Quita unidad del panel
      void MuestraAcciones(int );            //Muestra acciones disponibles para la unidad
      
      bool EsVisible(){return visible;}
      
      IntAcciones * GetIntAcciones();
      IntSeleccion * GetIntSeleccion(){ return intSeleccion; } 
      IntUnidad * GetIntUnidad() { return intUnidad; }
      IntBarraSuperior * GetBarraSuperior() { return intBarraSuperior; }
};

}
#endif
