#include "iUnidadUniSeleccionable.h"

#include "iAcademiaDeFrancotiradores.h"
#include "iAcademiaDeParacaidismo.h"
#include "iBarracones.h"
#include "iCentroDeComunicaciones.h"
#include "iCuartelGeneral.h"
#include "iHangar.h"
#include "iLaboratorio.h"
#include "iPlantaDeVehiculos.h"
#include "iPozoDeExtraccion.h"
#include "iSiloDeMisiles.h"
#include "iTorreDeVigilancia.h"
#include "iApi.h"
#include "gApi.h"

namespace Interfaz
{

UnidadUniSeleccionable * UnidadUniSeleccionable::unidadSeleccionada ;

void
UnidadUniSeleccionable::Inicializar()
{
    unidadSeleccionada = NULL ;
    
    Api::SetProgresoCarga( Api::GetProgresoCarga()+1.0 );
    Graficos::Api::SetPorcentajeBarraVida(Api::GetBarraProgresoCarga(), Api::GetProgresoCarga());
    Graficos::Api::Pintar();

    
    AcademiaDeFrancotiradores::Inicializar();
    AcademiaDeParacaidismo::Inicializar();
    Barracones::Inicializar();
    CentroDeComunicaciones::Inicializar();
    CuartelGeneral::Inicializar();
    Hangar::Inicializar();
    Laboratorio::Inicializar();
    PlantaDeVehiculos::Inicializar();
    PozoDeExtraccion::Inicializar();
    SiloDeMisiles::Inicializar();
    TorreDeVigilancia::Inicializar();
}

void
UnidadUniSeleccionable::Reset()
{
    unidadSeleccionada = NULL ;
}

void
UnidadUniSeleccionable::Cerrar()
{
    unidadSeleccionada = NULL ;
    
    AcademiaDeFrancotiradores::Cerrar();
    AcademiaDeParacaidismo::Cerrar();
    Barracones::Cerrar();
    CentroDeComunicaciones::Cerrar();
    CuartelGeneral::Cerrar();
    Hangar::Cerrar();
    Laboratorio::Cerrar();
    PlantaDeVehiculos::Cerrar();
    PozoDeExtraccion::Cerrar();
    SiloDeMisiles::Cerrar();
    TorreDeVigilancia::Cerrar();    
}

void
UnidadUniSeleccionable::ActualizarUnidadesUniSeleccionables()
{ 
    if ( unidadSeleccionada )
    {
        unidadSeleccionada->Actualizar() ;
    }  
}

void
UnidadUniSeleccionable::SeleccionarUnidad(UnidadUniSeleccionable *unidad)
{
    if ( unidadSeleccionada && unidadSeleccionada != unidad)
    {
        unidadSeleccionada->HaSidoDeseleccionada();
    }

    unidadSeleccionada = unidad ;
    UnidadSeleccionable::SetMultiSeleccion(false);
    unidad->Seleccionar();
}

void
UnidadUniSeleccionable::DeseleccionarTodo()
{
    if ( unidadSeleccionada )
    {
        unidadSeleccionada->HaSidoDeseleccionada();
        unidadSeleccionada = NULL ;
    }
}    
 
UnidadUniSeleccionable::UnidadUniSeleccionable(int numJugador):
    UnidadSeleccionable(numJugador)
{
}

UnidadUniSeleccionable::~UnidadUniSeleccionable()
{
} 
    
void
UnidadUniSeleccionable::Actualizar()
{
}


void
UnidadUniSeleccionable::Seleccionar()
{
    UnidadSeleccionable::Seleccionar();
    
    //Muestro la unidad uniseleccionable en el panel
    Api::GetInterfazPanel()->GetIntUnidad()->CambiarImagenUnidad(this);
    
}

void
UnidadUniSeleccionable::HaSidoDeseleccionada()
{
    UnidadSeleccionable::HaSidoDeseleccionada();
    
    Api::GetInterfazPanel()->GetIntSeleccion()->SetVisibleBarraProgreso(false);
}

bool
UnidadUniSeleccionable::EstaSeleccionada()
{
    return unidadSeleccionada == this ;
}
    
    
}
