#include "iPozoDeExtraccion.h"
#include "gApi.h"
#include "iApi.h"
#include "eExcepcionLocalizada.h"
#include "rProtocolo.h"

namespace Interfaz
{
int PozoDeExtraccion::textura ;
int PozoDeExtraccion::modeloHandler[8] ;

//Crea el cuartel general a partir de su modelo y lo marca como seleccionable
PozoDeExtraccion::PozoDeExtraccion(int numJugador):
    UnidadUniSeleccionable(numJugador)
{
    try
    {
        int color = Api::GetJugador(numJugador)->GetColor();        
        objetoHandler = Graficos::Api::CrearObjeto(modeloHandler[color]);
        Graficos::Api::SetObjetoSeleccionable(objetoHandler, true);
        Graficos::Api::SetObjetoBloqueante(objetoHandler, true);
        Graficos::Api::SetTransparenteObjeto(objetoHandler, true);
        
        float radio = Graficos::Api::GetRadioObjeto(objetoHandler);
        Graficos::Api::SetEscalaObjeto(handlerCirculoSeleccion, radio*1.25);
        
                
                
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::PozoDeExtraccion::PozoDeExtraccion()").Combinar(ex);
    }
}

//Destruye el modelo del cuartel general
PozoDeExtraccion::~PozoDeExtraccion()
{
    try
    {
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::PozoDeExtraccion::~PozoDeExtraccion()").Combinar(ex);
    }
}

//Selecciona el cuartel general y muestra las acciones disponibles para éste
void
PozoDeExtraccion::Seleccionar()
{
    UnidadUniSeleccionable::Seleccionar();
}    

void
PozoDeExtraccion::Actualizar()
{
}

//Obtiene el tipo de unidad
int
PozoDeExtraccion::GetTipo()
{
    return Red::Protocolo::Unidad::PozoDeExtraccion ;
}

void
PozoDeExtraccion::Inicializar()
{
    try
    {
        textura = Graficos::Api::CrearImagen("Unidades\\PozoDeExtraccion\\textura.bmp") ;
        for ( int i = 0 ; i < 8 ; i++ )
        {
            modeloHandler[i] = Graficos::Api::CrearModelo("Unidades\\PozoDeExtraccion\\modelo.3ds", textura, Jugador::GetColorRojo(i), Jugador::GetColorVerde(i), Jugador::GetColorAzul(i));

            Api::SetProgresoCarga( Api::GetProgresoCarga()+1.0 );
            Graficos::Api::SetPorcentajeBarraVida(Api::GetBarraProgresoCarga(), Api::GetProgresoCarga());
            Graficos::Api::Pintar();      
        }      
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::PozoDeExtraccion::Inicializar()").Combinar(ex);
    }
}

void
PozoDeExtraccion::Cerrar()
{
    try
    {

    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::PozoDeExtraccion::Cerrar()").Combinar(ex);
    }
}

}
