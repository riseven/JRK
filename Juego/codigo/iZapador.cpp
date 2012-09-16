#include "iZapador.h"
#include "gApi.h"
#include "iApi.h"
#include "eExcepcionLocalizada.h"
#include "rProtocolo.h"

namespace Interfaz
{

int Zapador::modeloHandler ;

//Crea el cuartel general a partir de su modelo y lo marca como seleccionable
Zapador::Zapador(int numJugador):
    UnidadMultiSeleccionable(numJugador)
{
    try
    {
        objetoHandler = Graficos::Api::CrearObjeto(modeloHandler);
        Graficos::Api::SetObjetoSeleccionable(objetoHandler, true);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::Zapador::Zapador()").Combinar(ex);
    }
}

//Destruye el modelo del cuartel general
Zapador::~Zapador()
{
    try
    {
        Graficos::Api::DestruirModelo(modeloHandler);    
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::Zapador::~Zapador()").Combinar(ex);
    }
}

//Selecciona el cuartel general y muestra las acciones disponibles para �ste
void
Zapador::Seleccionar()
{
    UnidadMultiSeleccionable::Seleccionar();
    
    for ( int i = 0 ; i < 12 ; i++ )
    {
        Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(i, i<1?true:false);
    }
    
    //PRUEBA: Cambia el boton de accion 0
    Api::GetInterfazPanel()->GetIntAcciones()->CambiarImagen(0, UnidadSeleccionable::imagenesBotones[UnidadSeleccionable::ImagenMover]);
}    

void
Zapador::Actualizar()
{
}

//Obtiene el tipo de unidad
int
Zapador::GetTipo()
{
    return Red::Protocolo::Unidad::Zapador ;
}

void
Zapador::Inicializar()
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
        throw Error::ExcepcionLocalizada("Interfaz::Zapador::Inicializar()").Combinar(ex);
    }
}

}
