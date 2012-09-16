#include "iSoldadoRaso.h"
#include "gApi.h"
#include "iApi.h"
#include "rApi.h"
#include <cstdlib>

namespace Interfaz
{

SoldadoRaso::SoldadoRaso()
{
    modeloHandler = Graficos::Api::CrearModelo("Unidades\\SoldadoRaso\\Ma1.3ds");
    objetoHandler = Graficos::Api::CrearObjeto(modeloHandler);
    Graficos::Api::SetObjetoSeleccionable(objetoHandler, true);
}

SoldadoRaso::~SoldadoRaso()
{
    Graficos::Api::DestruirModelo(modeloHandler);    
}

void
SoldadoRaso::Seleccionar()
{
    UnidadSeleccionable::unidadSeleccionada = this ;
    
    for ( int i = 0 ; i < 12 ; i++ )
    {
        Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(i, i<3?true:false);
    }
    
    Api::GetInterfazPanel()->GetIntAcciones()->CambiarImagen(0, UnidadSeleccionable::imagenesBotones[UnidadSeleccionable::ImagenMover]);
    Api::GetInterfazPanel()->GetIntAcciones()->CambiarImagen(1, UnidadSeleccionable::imagenesBotones[UnidadSeleccionable::ImagenAtacar]);
    Api::GetInterfazPanel()->GetIntAcciones()->CambiarImagen(2, UnidadSeleccionable::imagenesBotones[UnidadSeleccionable::ImagenDetener]);        
    
    Api::GetInterfazPanel()->GetIntAcciones()->OnClickBoton(0, &mover);
    Api::GetInterfazPanel()->GetIntAcciones()->OnClickBoton(2, &detener);
}    

void
SoldadoRaso::Actualizar()
{
    if ( mover )
    {
        Red::Api::GetApiLogica()->MoverUnidad(0, rand()%40, rand()%40);
    }
    if ( detener )
    {
        Red::Api::GetApiLogica()->DetenerUnidad(0);
    }       
}

int
SoldadoRaso::GetTipo()
{
    return Api::TipoSoldadoRaso ;
}

}
