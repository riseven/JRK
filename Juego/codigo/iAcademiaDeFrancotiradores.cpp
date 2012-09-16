#include "iAcademiaDeFrancotiradores.h"
#include "gApi.h"
#include "iApi.h"
#include "eExcepcionLocalizada.h"
#include "rProtocolo.h"

namespace Interfaz
{

int AcademiaDeFrancotiradores::modeloHandler ;

//Crea el cuartel general a partir de su modelo y lo marca como seleccionable
AcademiaDeFrancotiradores::AcademiaDeFrancotiradores(int numJugador):
    UnidadUniSeleccionable(numJugador)
{
    try
    {
        objetoHandler = Graficos::Api::CrearObjeto(modeloHandler);
        Graficos::Api::SetObjetoSeleccionable(objetoHandler, true);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::AcademiaDeFrancotiradores::AcademiaDeFrancotiradores()").Combinar(ex);
    }
}

//Destruye el modelo del cuartel general
AcademiaDeFrancotiradores::~AcademiaDeFrancotiradores()
{
    try
    {
        Graficos::Api::DestruirObjeto(objetoHandler);            
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::AcademiaDeFrancotiradores::~AcademiaDeFrancotiradores()").Combinar(ex);
    }
}

//Selecciona el cuartel general y muestra las acciones disponibles para éste
void
AcademiaDeFrancotiradores::Seleccionar()
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
AcademiaDeFrancotiradores::Actualizar()
{
}

//Obtiene el tipo de unidad
int
AcademiaDeFrancotiradores::GetTipo()
{
    return Red::Protocolo::Unidad::AcademiaDeFrancotiradores ;
}

void
AcademiaDeFrancotiradores::Inicializar()
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
        throw Error::ExcepcionLocalizada("Interfaz::AcademiaDeFrancotiradores::Inicializar()").Combinar(ex);
    }
}

void
AcademiaDeFrancotiradores::Cerrar()
{
    try
    {
        Graficos::Api::DestruirModelo(modeloHandler);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::AcademiaDeFrancotiradores::Cerrar()").Combinar(ex);
    }
}

}   
