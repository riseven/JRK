#include "iInterfazPanel.h"
#include "gApi.h"
#include "iApi.h"
#include "iCamara.h"
#include "eExcepcionLocalizada.h"

namespace Interfaz
{

//------------------------------------------------------------------------------
// InterfazJuego
//------------------------------------------------------------------------------


/* Carga el interfaz del juego. Este interfaz está compuesto por 4 zonas:
 *   - IntMinimapa: Muestra el minimapa
 *   - IntUnidad: Muestra información de unidad seleccionada
 *   - IntSeleccion: Muestra las unidades seleccionadas
 *   - IntAcciones: Muestra las acciones que pueden realizar la(s) unidad(es) seleccionada(s).
 */
InterfazPanel::InterfazPanel()
{
    Api::SetInterfazPanel(this);
    
    posX = 0;
    posY = 540;   // Coordenadas del interfaz (PRUEBA)
    strInterfaz="Graficos//Interfaz.bmp";
    
    //Carga la imagen del interfaz
    imgPanel = Graficos::Api::CrearImagen(strInterfaz);
    Graficos::Api::SetImagenMasked(imgPanel, true);
    sprPanel = Graficos::Api::CrearSprite(imgPanel, posX, posY, 1);
        
    //Crea todos los sub-interfaces
    intMinimapa = new IntMinimapa();
    intUnidad = new IntUnidad();    
    intSeleccion = new IntSeleccion();    
    intAcciones = new IntAcciones();    
    intBarraSuperior = new IntBarraSuperior();
    
    visible = true ;
}


/* Libera memoria reservada para el visor
 */
InterfazPanel::~InterfazPanel()
{
    try
    {
        Api::SetInterfazPanel(NULL);
        delete intMinimapa;
        delete intUnidad;
        delete intSeleccion;
        delete intAcciones;
        delete intBarraSuperior;
        Graficos::Api::DestruirSprite(sprPanel);
        Graficos::Api::DestruirImagen(imgPanel);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::InterfazPanel::~InterfazPanel()").Combinar(ex) ;
    }
}

//Comprueba eventos sobre el panel
void
InterfazPanel::Actualizar()
{
    if(visible)
    {
       intMinimapa->Actualizar() ;
       intSeleccion->Actualizar() ;
       intUnidad->Actualizar();
       intAcciones->Actualizar() ;
       intBarraSuperior->Actualizar() ;
    }
}


/* Metodo que cambia la imagen del minimapa
 */
bool InterfazPanel::SetMinimapa(string nombre)
{
    try
    {
        return intMinimapa->SetMinimapa(nombre);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::InterfazPanel::SetMinimapa(string)").Combinar(ex);
    }
}

IntAcciones *
InterfazPanel::GetIntAcciones()
{
    return intAcciones ;
}

void
InterfazPanel::Activar()
{
    visible = true ;
   intMinimapa->Activar();
   intSeleccion->Activar();
   intUnidad->Activar();
   intAcciones->Activar();
   intBarraSuperior->Activar();
   
   Graficos::Api::ActivarSprite( sprPanel );
}

void
InterfazPanel::Desactivar()
{
    visible = false ;
   intMinimapa->Desactivar();
   intSeleccion->Desactivar();
   intUnidad->Desactivar();
   intAcciones->Desactivar();
   intBarraSuperior->Desactivar();
   
   Graficos::Api::DesactivarSprite( sprPanel );
}

}
