#ifndef __I_INT_ACCIONES__
#define __I_INT_ACCIONES__

#include <allegro.h>
#include <alleggl.h>
#include <string>

#include <iostream>


using namespace std;

namespace Interfaz {

//Clase que implementa la interfaz de las acciones que una unidad puede realizar
class IntAcciones
{
   private:
     int imagenSeleccion ;
     int btAcciones[12];
     bool *onClicks[12];
     int hotKey[12];             //Tecla asociada a la accion
     int hotKeyPulsada[12];      //Estado de la tecla asociada     
     bool accionesActivas[12];
     int imgTexto[12];
     int sprTexto[12];
     int xIni,yIni;
     bool onMouseOver[12];
     int sprToolTip[12];
     int imgToolTip[12];
     
   public:
      IntAcciones();
      ~IntAcciones();
      void Activar();
      void Desactivar();
      
      void SetVisible(int numBoton, bool visible);
      void CambiarImagen(int numBoton, int handlerImagen);
      void OnClickBoton(int numBoton, bool *OnClickFlag);
      void AsociarHotKey(int numBoton, int codigoTecla);
      void SetFijoBoton(int numBoton, bool fijo);
      void Actualizar();
      void SetTextoBoton(int numBoton, string texto);
      void SetTextoToolTip(int toolTip, string texto);
      
};

}

#endif
