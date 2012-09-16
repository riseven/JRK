#include "iArtilleriaMovil.h"
#include "gApi.h"
#include "iApi.h"
#include "eExcepcionLocalizada.h"
#include "rProtocolo.h"

namespace Interfaz
{

int ArtilleriaMovil::modeloHandler ;

//Crea el cuartel general a partir de su modelo y lo marca como seleccionable
ArtilleriaMovil::ArtilleriaMovil(int numJugador):
    UnidadAgresiva(numJugador)
{
    try
    {
        objetoHandler = Graficos::Api::CrearObjeto(modeloHandler);
        Graficos::Api::SetObjetoSeleccionable(objetoHandler, true);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::ArtilleriaMovil::ArtilleriaMovil()").Combinar(ex);
    }
}

//Destruye el modelo del cuartel general
ArtilleriaMovil::~ArtilleriaMovil()
{
    try
    {
        Graficos::Api::DestruirModelo(modeloHandler);    
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::ArtilleriaMovil::~ArtilleriaMovil()").Combinar(ex);
    }
}

//Selecciona el cuartel general y muestra las acciones disponibles para éste
void
ArtilleriaMovil::Seleccionar()
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
ArtilleriaMovil::Actualizar()
{
}

//Obtiene el tipo de unidad
int
ArtilleriaMovil::GetTipo()
{
    return Red::Protocolo::Unidad::ArtilleriaMovil ;
}

void
ArtilleriaMovil::Inicializar()
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
        throw Error::ExcepcionLocalizada("Interfaz::ArtilleriaMovil::Inicializar()").Combinar(ex);
    }
}

}
