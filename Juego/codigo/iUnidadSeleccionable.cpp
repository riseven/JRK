#include "iUnidadSeleccionable.h"
#include "iUnidadUniSeleccionable.h"
#include "iUnidadMultiSeleccionable.h"
#include "iApi.h"
#include "gApi.h"
#include "rProtocolo.h"
#include "eExcepcionParametro.h"

namespace Interfaz
{

bool UnidadSeleccionable::multiSeleccion ;
list <UnidadSeleccionable *> UnidadSeleccionable::unidadesSeleccionables ;
int UnidadSeleccionable::imagenesBotones[UnidadSeleccionable::numImagenesBotones] ;
bool UnidadSeleccionable::estadoHotKey[UnidadSeleccionable::numHotKeys] ;
int UnidadSeleccionable::handlerImagenSeleccion ;
int UnidadSeleccionable::handlerModeloSeleccion[8] ;
int UnidadSeleccionable::accionActual=AccionNinguna;

UnidadSeleccionable * UnidadSeleccionable::ultimaBusqueda = NULL ;

list <UnidadSeleccionable *> UnidadSeleccionable::unidadesApuntadas ;

//Inicializa la gestion de una unidada seleccionable
void
UnidadSeleccionable::Inicializar()
{    
    //Establece botones para las acciones de este tipo de unidades
    imagenesBotones[ImagenMover]=Graficos::Api::CrearImagen("Graficos\\accionMover.bmp");
    imagenesBotones[ImagenDetener]=Graficos::Api::CrearImagen("Graficos\\accionDetener.bmp");
    imagenesBotones[ImagenTomar]=Graficos::Api::CrearImagen("Graficos\\accionTomar.bmp");
    // Unidad agresiva
    imagenesBotones[ImagenAtacar]=Graficos::Api::CrearImagen("Graficos\\accionAtacar.bmp");
    imagenesBotones[ImagenOfensivo]=Graficos::Api::CrearImagen("Graficos\\accionOfensivo.bmp");
    imagenesBotones[ImagenDefensivo]=Graficos::Api::CrearImagen("Graficos\\accionDefensivo.bmp");
    imagenesBotones[ImagenMantenerTerreno]=Graficos::Api::CrearImagen("Graficos\\accionMantenerTerreno.bmp");
    imagenesBotones[ImagenPasivo]=Graficos::Api::CrearImagen("Graficos\\accionPasivo.bmp");                
    // Cuartel General
    imagenesBotones[ImagenConstruirIngeniero]=Graficos::Api::CrearImagen("Graficos\\accionConstruirIngeniero.bmp");
    imagenesBotones[ImagenDesarrollarMotivacion]=Graficos::Api::CrearImagen("Graficos\\accionDesarrollarMotivacion.bmp");    
    imagenesBotones[ImagenDesarrollarMotivacionMasDistancia]=Graficos::Api::CrearImagen("Graficos\\accionDesarrollarMotivacionMasDistancia.bmp");    
    // Barracones
    imagenesBotones[ImagenConstruirSoldado]=Graficos::Api::CrearImagen("Graficos\\accionConstruirSoldado.bmp");
    imagenesBotones[ImagenDesarrollarArmaduraSoldado]=Graficos::Api::CrearImagen("Graficos\\accionDesarrollarArmaduraSoldado.bmp");    
    imagenesBotones[ImagenDesarrollarVelocidadDisparoSoldado]=Graficos::Api::CrearImagen("Graficos\\accionDesarrollarVelocidadDisparoSoldado.bmp");        
    // Soldados
    imagenesBotones[ImagenCorrerSoldado]=Graficos::Api::CrearImagen("Graficos\\accionCorrer.bmp");    
    // Ingenieros
    imagenesBotones[ImagenConstruirEdificio]=Graficos::Api::CrearImagen("Graficos\\accionConstruir.bmp");
    imagenesBotones[ImagenRepararIngeniero]=Graficos::Api::CrearImagen("Graficos\\accionReparar.bmp");
    imagenesBotones[ImagenConstruirCuartelGeneral]=Graficos::Api::CrearImagen("Graficos\\accionConstruirCuartelGeneral.bmp");
    imagenesBotones[ImagenConstruirBarracones]=Graficos::Api::CrearImagen("Graficos\\accionConstruirBarracones.bmp");
    imagenesBotones[ImagenConstruirPozoDeExtraccion]=Graficos::Api::CrearImagen("Graficos\\accionConstruirPozoExtraccion.bmp");    
    // Especial
    imagenesBotones[ImagenCancelarAccion]=Graficos::Api::CrearImagen("Graficos\\accionCancelarAccion.bmp");

    handlerImagenSeleccion = Graficos::Api::CrearImagen("Texturas\\CirculoSeleccion.bmp");
    for ( int i = 0 ; i < 8 ; i++ )
    {
        handlerModeloSeleccion[i] = Graficos::Api::CrearModeloQuad(handlerImagenSeleccion, Jugador::GetColorRojo(i), Jugador::GetColorVerde(i), Jugador::GetColorAzul(i), 0.3);
    }
    
    for ( int i = 0 ; i < numHotKeys ; i++ )
    {
        estadoHotKey[i] = 0 ;
    }
    unidadesSeleccionables.clear();
    multiSeleccion = false ;
    
    Api::SetProgresoCarga( Api::GetProgresoCarga()+1.0 );
    Graficos::Api::SetPorcentajeBarraVida(Api::GetBarraProgresoCarga(), Api::GetProgresoCarga());
    Graficos::Api::Pintar();
    
    UnidadUniSeleccionable::Inicializar();
    UnidadMultiSeleccionable::Inicializar();
}

void
UnidadSeleccionable::Reset()
{
    for ( int i = 0 ; i < numHotKeys ; i++ )
    {
        estadoHotKey[i] = 0 ;
    }
    
    multiSeleccion = false ;
    
    unidadesSeleccionables.clear();
    unidadesApuntadas.clear();
    
    accionActual = AccionNinguna ;
    ultimaBusqueda = NULL ;
    
    UnidadUniSeleccionable::Reset();
    UnidadMultiSeleccionable::Reset();
}

void
UnidadSeleccionable::Cerrar()
{
    multiSeleccion = false ;
    unidadesSeleccionables.clear();    
    UnidadUniSeleccionable::Cerrar();
}


// Comprueba las hotkeys globales, es decir, aquellas que no
// estan asociadas a una unidad. Estas teclas siempre realizan
// la misma accion, como por ejemlo:
//  -Seleccionar cuartel genear.
//  -Seleccionar siguiente unidad.
void
UnidadSeleccionable::ActualizarUnidadesSeleccionables()
{
    if ( multiSeleccion )
    {
        UnidadMultiSeleccionable::ActualizarUnidadesMultiSeleccionables();
    }
    else
    {
        UnidadUniSeleccionable::ActualizarUnidadesUniSeleccionables();
    }
}

UnidadSeleccionable *
UnidadSeleccionable::BuscarUnidad(int handlerObjeto)
{
    for ( list <UnidadSeleccionable *>::iterator i = unidadesSeleccionables.begin() ; i != unidadesSeleccionables.end() ; ++i )
    {
        if ( (*i)->objetoHandler == handlerObjeto )
        {
            return *i ;
        }
    }
    throw Error::ExcepcionParametro("UnidadSeleccionable::BuscarUnidad(int)", 1);
}

void
UnidadSeleccionable::RespuestaInteligenteTerreno(float x, float z)
{
    if ( multiSeleccion )
    {
        UnidadMultiSeleccionable::RespuestaInteligenteTerreno(x, z);
    }
}

void
UnidadSeleccionable::RespuestaInteligenteUnidad(UnidadSeleccionable *unidad)
{
    if ( multiSeleccion )
    {
        UnidadMultiSeleccionable::RespuestaInteligenteUnidad(unidad);
    }    
}

void
UnidadSeleccionable::RespuestaEstrictaTerreno(float x, float z)
{
    if ( multiSeleccion )
    {
        UnidadMultiSeleccionable::RespuestaEstrictaTerreno(x,z);
    }
}

void
UnidadSeleccionable::RespuestaEstrictaUnidad(UnidadSeleccionable *unidad)
{
    if ( multiSeleccion )
    {
        UnidadMultiSeleccionable::RespuestaEstrictaUnidad(unidad);
    }    
}

void
UnidadSeleccionable::ApuntarUnidad(UnidadSeleccionable *unidad)
{
    // Pintamos la barra de vida
    Graficos::Api::ActivarBarraVida(unidad->handlerBarraVida);
    
    if ( !unidad->EstaSeleccionada() )
    {
        // Pintamos el circulo de seleccion
        Graficos::Api::ActivarObjeto(unidad->handlerCirculoSeleccion);
    }
    
    unidadesApuntadas.push_back(unidad);
}

void
UnidadSeleccionable::DesapuntarUnidades()
{
    for ( list <UnidadSeleccionable *>::iterator i = unidadesApuntadas.begin() ; i != unidadesApuntadas.end() ; ++i )
    {
        Graficos::Api::DesactivarBarraVida( (*i)->handlerBarraVida );
         
        if ( !(*i)->EstaSeleccionada() )
        {
            Graficos::Api::DesactivarObjeto( (*i)->handlerCirculoSeleccion );
        }
    }
    unidadesApuntadas.clear();
}

//Crea una unidad seleccionable
UnidadSeleccionable::UnidadSeleccionable(int numJugador):
    numJugador(numJugador)
{
    //Guarda lista con unidades seleccionables
    unidadesSeleccionables.push_back(this);

    handlerCirculoSeleccion = Graficos::Api::CrearObjeto(handlerModeloSeleccion[Api::GetJugador(numJugador)->GetColor()]);
    Graficos::Api::SetObjetoSiempreVisible(handlerCirculoSeleccion, true);
    Graficos::Api::DesactivarObjeto(handlerCirculoSeleccion);
    
    handlerBarraVida = Graficos::Api::CrearBarraVida(100, 15, 1, 0, 0, 10);
    Graficos::Api::DesactivarBarraVida(handlerBarraVida);
}

//Destruye unidad seleccionable
UnidadSeleccionable::~UnidadSeleccionable()
{ 
    try
    {
        unidadesSeleccionables.remove(this);
    
        Graficos::Api::DestruirObjeto(handlerCirculoSeleccion);        
        
        Graficos::Api::DestruirBarraVida(handlerBarraVida);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::UnidadSeleccionable::~UnidadSeleccionable()").Combinar(ex) ;
    }
}

//Accion ejecutada al ser seleccionada
void
UnidadSeleccionable::Seleccionar()
{
    Unidad::SetAlgunaUnidadSeleccionada(true);
    Graficos::Api::ActivarObjeto(handlerCirculoSeleccion);
}

void
UnidadSeleccionable::Deseleccionar()
{
   if( unidadesSeleccionables.empty() )
      Unidad::SetAlgunaUnidadSeleccionada(false);
   
   HaSidoDeseleccionada();   
}

void
UnidadSeleccionable::HaSidoDeseleccionada()
{
    Graficos::Api::DesactivarObjeto(handlerCirculoSeleccion);
}

bool
UnidadSeleccionable::EstaSeleccionada()
{
    return false ;
}

void
UnidadSeleccionable::SetMultiSeleccion(bool multiSeleccion)
{
    UnidadSeleccionable::multiSeleccion = multiSeleccion ;
    if ( multiSeleccion )
    {
        UnidadUniSeleccionable::DeseleccionarTodo() ;
    }
    else
    {
        UnidadMultiSeleccionable::DeseleccionarTodo() ;
    }
}


list<UnidadSeleccionable *>::iterator
UnidadSeleccionable::BuscarUltimaBusqueda()
{
    if ( ultimaBusqueda )
    {
        for ( list<UnidadSeleccionable *>::iterator i = unidadesSeleccionables.begin() ; i != unidadesSeleccionables.end() ; ++i )
        {
            if ( (*i) == ultimaBusqueda )
            {
                return i ;
            }
        }
    }
    return unidadesSeleccionables.begin() ;
}

//Selecciona el cuartel general
void
UnidadSeleccionable::BuscarCuartelGeneral()
{
    if ( unidadesSeleccionables.size() < 1 )
    {
        return ;
    }
    
    list<UnidadSeleccionable *>::iterator i = BuscarUltimaBusqueda();
    list<UnidadSeleccionable *>::iterator ult = i ;
    do
    {
        ++i ;
        if ( i == unidadesSeleccionables.end() )
        {
            i = unidadesSeleccionables.begin() ;
        }
        if ( (*i)->GetTipo() == Red::Protocolo::Unidad::CuartelGeneral )
        {
            if ( (*i)->GetNumJugador() == Jugador::GetJugadorPropio() )
            {
                ultimaBusqueda = *i ;
                UnidadUniSeleccionable::SeleccionarUnidad((UnidadUniSeleccionable *)(*i));
                return ;
            }
        }    
    }while( i != ult );
}

void
UnidadSeleccionable::BuscarIngeniero()
{
    if ( unidadesSeleccionables.size() < 1 )
    {
        return ;
    }
    
    list<UnidadSeleccionable *>::iterator i = BuscarUltimaBusqueda();
    list<UnidadSeleccionable *>::iterator ult = i ;
    do
    {
        ++i ;
        if ( i == unidadesSeleccionables.end() )
        {
            i = unidadesSeleccionables.begin() ;
        }
        if ( (*i)->GetTipo() == Red::Protocolo::Unidad::Ingeniero )
        {
            if ( (*i)->GetNumJugador() == Jugador::GetJugadorPropio() )
            {
                ultimaBusqueda = *i ;
                UnidadMultiSeleccionable::SeleccionarUnidad((UnidadMultiSeleccionable *)(*i));
                return ;
            }
        }    
    }while( i != ult );
}

// Recorre la lista de unidades seleccionables para seleccionar
// la siguiente unidad creada  
void
UnidadSeleccionable::BuscarSiguienteUnidad()
{
    /*
    if ( !unidadSeleccionada )
    {
        if ( !unidadesSeleccionables.empty() )
        {
            (*unidadesSeleccionables.begin())->Seleccionar() ;
        }
    }
    else
    {
        for ( list <UnidadSeleccionable *>::iterator i = unidadesSeleccionables.begin() ; i != unidadesSeleccionables.end() ; ++i )
        {
            if ( (*i) != unidadSeleccionada )
            {
                continue ;
            }
            ++i ;
            if ( i != unidadesSeleccionables.end() )
            {
                (*i)->Seleccionar();
            }
            else
            {
                i = unidadesSeleccionables.begin() ;
                if ( (*i) != NULL )
                {
                    (*i)->Seleccionar() ;
                }
            }
            break ;
        }
    }      
    */         
}

//Cambia la posicion de una unidad seleccionable
void 
UnidadSeleccionable::SetPos(float x, float y, float z)
{
    Unidad::SetPos(x, y, z);
    Graficos::Api::MoverObjeto(handlerCirculoSeleccion, x, y, z);   
    
    // Movemos la barra de vida
    float altura = Graficos::Api::GetAlturaObjeto(objetoHandler);
    Util::Vector2D pos = Graficos::Api::GetCoordenadas3Dto2D(x,y+altura,z);
    Graficos::Api::CentrarBarraVida(handlerBarraVida, pos.getX(), pos.getY());
}

void
UnidadSeleccionable::ActualizarPrimeraPersona()
{
    throw Error::Excepcion("Esta unidad no permite ser poseida");
}

void
UnidadSeleccionable::EntrarPrimeraPersona()
{
    throw Error::Excepcion("Esta unidad no permite ser poseida");
}

void
UnidadSeleccionable::SalirPrimeraPersona()
{
    throw Error::Excepcion("Esta unidad no permite ser poseida");
}

void
UnidadSeleccionable::UnidadRespuestaInteligenteTerreno(float x, float z)
{   
}

void
UnidadSeleccionable::UnidadRespuestaInteligenteUnidad(UnidadSeleccionable *unidad)
{   
}

void
UnidadSeleccionable::UnidadRespuestaEstrictaTerreno(float x, float z)
{
}

void
UnidadSeleccionable::UnidadRespuestaEstrictaUnidad(UnidadSeleccionable *unidad)
{
}

}
