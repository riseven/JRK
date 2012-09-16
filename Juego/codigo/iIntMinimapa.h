#ifndef __I_INT_MINIMAPA__
#define __I_INT_MINIMAPA__

#include <string>
#include "iCamara.h"
#include <list>

using namespace std;

namespace Interfaz {

//Clase que implementa el minimapa
class IntMinimapa
{
   private:
        int imgMinimapa;
        int imgMinimapaTrans;
        int sprMinimapaTrans;
        int btnMinimapa;
        int posX, posY;                            //Posicion del minimapa dentro del interfaz
        int ancho, alto;   
        bool btnFlag;      
        bool bdrPulsado ;
        int segmentosVision[4] ;
        
        list <int > miniUnidades ;        
        
        void TransCoorAPantalla(float &x, float &y);      
        void TransCoorAMinimapa(float &x, float &y);

   public:
      
      IntMinimapa();                //Carga el minimapa indicado
      ~IntMinimapa();
      void Actualizar();
      void Activar();
      void Desactivar();
  //  bool DetectaClick(int x, int y,Camara& );           //Detecta si el click pertence a esta zona del interfaz
      bool SetMinimapa(string strNombre);        //Cambia el minimapa
      void PintaUnidades();
};

}

#endif
