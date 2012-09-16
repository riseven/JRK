#ifndef __I_INT_SELECCION__
#define __I_INT_SELECCION__

#include <list>
#include "iUnidadMultiSeleccionable.h"
#include "gBarraVida.h"

using namespace std;
namespace Interfaz {
          
//Clase que implementa la interfaz de las unidades seleccionadas
class IntSeleccion
{
private:
        
    int imagenBarraProgreso ;
    int imagenCuadroProgreso ;
    //int imagenBarraVida;
    //int imagenCuadroVida;
    int barraProgreso ;
    
    void Actualiza();

    int btUnidades[12];
    int barraVida[12];
    int imgUnidades[12];
   //bool actualizar;

    bool onClicks[12];
    int xIni,yIni;
    
    int numRetratosActivos;
    
   public:
      IntSeleccion();
      ~IntSeleccion();
      void Activar();
      void Desactivar();
      
      
      void Actualizar();
      void SetVisibleBarraProgreso(bool visible);
      void SetPorcentajeBarraProgreso(int porcentaje);
      void OnClickBoton(int numBoton, bool *OnClickFlag);
      void ActualizarSeleccion(list <UnidadMultiSeleccionable *>);

};

}

#endif
