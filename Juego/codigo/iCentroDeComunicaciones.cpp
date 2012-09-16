#include "iCentroDeComunicaciones.h"
#include "gApi.h"
#include "iApi.h"
#include "eExcepcionLocalizada.h"
#include "rProtocolo.h"

#include <string>
namespace Interfaz
{

int CentroDeComunicaciones::modeloHandler ;

//Crea el cuartel general a partir de su modelo y lo marca como seleccionable
CentroDeComunicaciones::CentroDeComunicaciones(int numJugador):
    UnidadUniSeleccionable(numJugador)
{
    try
    {
        objetoHandler = Graficos::Api::CrearObjeto(modeloHandler);
        Graficos::Api::SetObjetoSeleccionable(objetoHandler, true);    
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::CentroDeComunicaciones::CentroDeComunicaciones()").Combinar(ex);
    }
}

//Destruye el modelo del cuartel general
CentroDeComunicaciones::~CentroDeComunicaciones()
{
    try
    { 
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::CentroDeComunicaciones::~CentroDeComunicaciones()").Combinar(ex);
    }
}

//Selecciona el cuartel general y muestra las acciones disponibles para éste
void
CentroDeComunicaciones::Seleccionar()
{
    UnidadUniSeleccionable::Seleccionar();
    
    for ( int i = 0 ; i < 12 ; i++ )
    {
        Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(i, i<1?true:false);
    }
    
    //PRUEBA: Cambia el boton de accion 0
    Api::GetInterfazPanel()->GetIntAcciones()->CambiarImagen(0, UnidadSeleccionable::imagenesBotones[UnidadSeleccionable::ImagenMover]);
}    

void
CentroDeComunicaciones::Actualizar()
{
}

//Obtiene el tipo de unidad
int
CentroDeComunicaciones::GetTipo()
{
    return Red::Protocolo::Unidad::CentroDeComunicaciones ;
}

void
CentroDeComunicaciones::Inicializar()
{
    try
    {
        modeloHandler = Graficos::Api::CrearModelo("");    
        
        Api::SetProgresoCarga( Api::GetProgresoCarga()+1.0 );
        Graficos::Api::SetPorcentajeBarraVida(Api::GetBarraProgresoCarga(), Api::GetProgresoCarga());
        Graficos::Api::Pintar();              
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::CentroDeComunicaciones::Inicializar()").Combinar(ex);
    }
}

void
CentroDeComunicaciones::Cerrar()
{
    try
    {
        Graficos::Api::DestruirModelo(modeloHandler);        
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::CentroDeComunicaciones::Cerrar()").Combinar(ex);
    }
}

}
