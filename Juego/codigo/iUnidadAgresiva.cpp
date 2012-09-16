#include "iUnidadAgresiva.h"
#include "gApi.h"
#include "iApi.h"
#include "rApi.h"
#include "rProtocolo.h"
#include <cstdlib>

#include "iArtificiero.h"
#include "iArtilleriaMovil.h"
#include "iFrancotirador.h"
#include "iHelicoptero.h"
#include "iJeep.h"
#include "iParacaidista.h"
#include "iSAMMovil.h"
#include "iSoldado.h"
#include "iTanque.h"

namespace Interfaz
{

bool UnidadAgresiva::atacar ;
bool UnidadAgresiva::modoAgresivo ;
bool UnidadAgresiva::modoDefensivo ;
bool UnidadAgresiva::modoMantenerTerreno ;
bool UnidadAgresiva::modoPasivo ;


void
UnidadAgresiva::Inicializar()
{
    Artificiero::Inicializar();
    ArtilleriaMovil::Inicializar();
    Francotirador::Inicializar();
    Helicoptero::Inicializar();
    Jeep::Inicializar();
    Paracaidista::Inicializar();
    SAMMovil::Inicializar();
    Soldado::Inicializar();
    Tanque::Inicializar();
}

void
UnidadAgresiva::Reset()
{
    atacar = false ;
    modoAgresivo = false ;
    modoDefensivo = false ;
    modoMantenerTerreno = false ;
    modoPasivo = false ;
}

//Creo una unidad Soldado Raso a partir de su modelo
UnidadAgresiva::UnidadAgresiva(int numJugador):
    UnidadMultiSeleccionable(numJugador)
{
    modoActual = 1 ;
}

//Destruyo la unidad
UnidadAgresiva::~UnidadAgresiva()
{
}

//Selecciona la unidad y muestra las acciones disponibles para ésta
void
UnidadAgresiva::Seleccionar()
{
    UnidadMultiSeleccionable::Seleccionar();
}    


//Actualiza el estado de la unidad dependiendo de la accion seleccionada
void
UnidadAgresiva::Actualizar()
{ 
}

void
UnidadAgresiva::PintarBotones()
{
    int modo = ((UnidadAgresiva *)(UnidadMultiSeleccionable::GetUnidadesSeleccionadas().front()))->modoActual ;
        
    Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(1, true);
    Api::GetInterfazPanel()->GetIntAcciones()->CambiarImagen(1, UnidadSeleccionable::imagenesBotones[UnidadSeleccionable::ImagenAtacar]);            
    Api::GetInterfazPanel()->GetIntAcciones()->OnClickBoton(1, &atacar);
    Api::GetInterfazPanel()->GetIntAcciones()->AsociarHotKey(1, KEY_A);
    Api::GetInterfazPanel()->GetIntAcciones()->SetTextoBoton(1, "A");
    Api::GetInterfazPanel()->GetIntAcciones()->SetTextoToolTip(1, "Atacar / Atacar zona");    
    
    Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(4, true);
    Api::GetInterfazPanel()->GetIntAcciones()->CambiarImagen(4, UnidadSeleccionable::imagenesBotones[UnidadSeleccionable::ImagenOfensivo]);
    Api::GetInterfazPanel()->GetIntAcciones()->OnClickBoton(4, &modoAgresivo);
    Api::GetInterfazPanel()->GetIntAcciones()->AsociarHotKey(4, KEY_G);
    Api::GetInterfazPanel()->GetIntAcciones()->SetFijoBoton(4, modo==1);
    Api::GetInterfazPanel()->GetIntAcciones()->SetTextoBoton(4, "G");
    Api::GetInterfazPanel()->GetIntAcciones()->SetTextoToolTip(4, "Modo agresivo");    
    
    Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(5, true);
    Api::GetInterfazPanel()->GetIntAcciones()->CambiarImagen(5, UnidadSeleccionable::imagenesBotones[UnidadSeleccionable::ImagenDefensivo]);        
    Api::GetInterfazPanel()->GetIntAcciones()->OnClickBoton(5, &modoDefensivo);
    Api::GetInterfazPanel()->GetIntAcciones()->AsociarHotKey(5, KEY_E);    
    Api::GetInterfazPanel()->GetIntAcciones()->SetFijoBoton(5, modo==2);
    Api::GetInterfazPanel()->GetIntAcciones()->SetTextoBoton(5, "E");
    Api::GetInterfazPanel()->GetIntAcciones()->SetTextoToolTip(5, "Modo defensivo");    
    
    Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(6, true);
    Api::GetInterfazPanel()->GetIntAcciones()->CambiarImagen(6, UnidadSeleccionable::imagenesBotones[UnidadSeleccionable::ImagenMantenerTerreno]);      
    Api::GetInterfazPanel()->GetIntAcciones()->OnClickBoton(6, &modoMantenerTerreno);
    Api::GetInterfazPanel()->GetIntAcciones()->AsociarHotKey(6, KEY_T);  
    Api::GetInterfazPanel()->GetIntAcciones()->SetFijoBoton(6, modo==3);    
    Api::GetInterfazPanel()->GetIntAcciones()->SetTextoBoton(6, "T");
    Api::GetInterfazPanel()->GetIntAcciones()->SetTextoToolTip(6, "Modo mantener terreno");    
      
    Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(7, true);
    Api::GetInterfazPanel()->GetIntAcciones()->CambiarImagen(7, UnidadSeleccionable::imagenesBotones[UnidadSeleccionable::ImagenPasivo]);        
    Api::GetInterfazPanel()->GetIntAcciones()->OnClickBoton(7, &modoPasivo);    
    Api::GetInterfazPanel()->GetIntAcciones()->AsociarHotKey(7, KEY_P);    
    Api::GetInterfazPanel()->GetIntAcciones()->SetFijoBoton(7, modo==4);    
    Api::GetInterfazPanel()->GetIntAcciones()->SetTextoBoton(7, "P");
    Api::GetInterfazPanel()->GetIntAcciones()->SetTextoToolTip(7, "Modo pasivo");    
}    
    
void
UnidadAgresiva::ActualizarUnidadesAgresivas()
{
    if ( atacar )
    {
        atacar = false ;
        UnidadMultiSeleccionable::MostrarMenuCancelar();
        UnidadSeleccionable::SetAccionActual(UnidadSeleccionable::AccionAtacar);
    }
    if ( modoAgresivo )
    {
        for ( list <UnidadMultiSeleccionable *>::iterator i = UnidadMultiSeleccionable::unidadesSeleccionadas.begin() ; i != UnidadMultiSeleccionable::unidadesSeleccionadas.end() ; ++i )
        {
            Red::Api::GetApiLogica()->CambiarModo( (*i)->GetHandlerUnidad() , Red::Protocolo::ModoUnidad::Agresivo) ;
            ((UnidadAgresiva *)(*i))->modoActual = 1;
        }
        Api::GetInterfazPanel()->GetIntAcciones()->SetFijoBoton(4, true);
        Api::GetInterfazPanel()->GetIntAcciones()->SetFijoBoton(5, false);
        Api::GetInterfazPanel()->GetIntAcciones()->SetFijoBoton(6, false);
        Api::GetInterfazPanel()->GetIntAcciones()->SetFijoBoton(7, false);
    }
    if ( modoDefensivo )
    {
        for ( list <UnidadMultiSeleccionable *>::iterator i = UnidadMultiSeleccionable::unidadesSeleccionadas.begin() ; i != UnidadMultiSeleccionable::unidadesSeleccionadas.end() ; ++i )
        {
            Red::Api::GetApiLogica()->CambiarModo( (*i)->GetHandlerUnidad() , Red::Protocolo::ModoUnidad::Defensivo ) ;
            ((UnidadAgresiva *)(*i))->modoActual = 2;            
        }
        Api::GetInterfazPanel()->GetIntAcciones()->SetFijoBoton(4, false);
        Api::GetInterfazPanel()->GetIntAcciones()->SetFijoBoton(5, true);
        Api::GetInterfazPanel()->GetIntAcciones()->SetFijoBoton(6, false);
        Api::GetInterfazPanel()->GetIntAcciones()->SetFijoBoton(7, false);        
    }
    if ( modoMantenerTerreno )
    {
        for ( list <UnidadMultiSeleccionable *>::iterator i = UnidadMultiSeleccionable::unidadesSeleccionadas.begin() ; i != UnidadMultiSeleccionable::unidadesSeleccionadas.end() ; ++i )
        {
            Red::Api::GetApiLogica()->CambiarModo( (*i)->GetHandlerUnidad() , Red::Protocolo::ModoUnidad::MantenerPosicion ) ;
            ((UnidadAgresiva *)(*i))->modoActual = 3;            
        }
        Api::GetInterfazPanel()->GetIntAcciones()->SetFijoBoton(4, false);
        Api::GetInterfazPanel()->GetIntAcciones()->SetFijoBoton(5, false);
        Api::GetInterfazPanel()->GetIntAcciones()->SetFijoBoton(6, true);
        Api::GetInterfazPanel()->GetIntAcciones()->SetFijoBoton(7, false);                
    }
    if ( modoPasivo )
    {
        for ( list <UnidadMultiSeleccionable *>::iterator i = UnidadMultiSeleccionable::unidadesSeleccionadas.begin() ; i != UnidadMultiSeleccionable::unidadesSeleccionadas.end() ; ++i )
        {
            Red::Api::GetApiLogica()->CambiarModo( (*i)->GetHandlerUnidad() , Red::Protocolo::ModoUnidad::Pasivo ) ;
            ((UnidadAgresiva *)(*i))->modoActual = 4;            
        }    
        Api::GetInterfazPanel()->GetIntAcciones()->SetFijoBoton(4, false);
        Api::GetInterfazPanel()->GetIntAcciones()->SetFijoBoton(5, false);
        Api::GetInterfazPanel()->GetIntAcciones()->SetFijoBoton(6, false);
        Api::GetInterfazPanel()->GetIntAcciones()->SetFijoBoton(7, true);        
    }
}

void
UnidadAgresiva::UnidadRespuestaInteligenteUnidad(UnidadSeleccionable *unidad)
{
    if ( unidad->GetNumJugador() == numJugador )
    {
        if ( numSonidosMover > 0 )
        {
            Graficos::Api::ReproducirSonido( unidadSonidosMover[ rand()%numSonidosMover ] );   
        }        
        Red::Api::GetApiLogica()->SeguirUnidad(handlerUnidad, unidad->GetHandlerUnidad());
    }
    else
    {
        Red::Api::GetApiLogica()->AtacarUnidad(handlerUnidad, unidad->GetHandlerUnidad());
    }
}

void
UnidadAgresiva::UnidadRespuestaEstrictaTerreno(float x, float z)
{
    switch ( UnidadSeleccionable::GetAccionActual() )
    {
    case UnidadSeleccionable::AccionMover:
        if ( numSonidosMover > 0 )
        {
            Graficos::Api::ReproducirSonido( unidadSonidosMover[ rand()%numSonidosMover ] );   
        }        
        Red::Api::GetApiLogica()->MoverUnidad(handlerUnidad, x, z);
        break ;
    case UnidadSeleccionable::AccionAtacar:
        if ( numSonidosAtacar > 0 )
        {
            Graficos::Api::ReproducirSonido( unidadSonidosAtacar[ rand()%numSonidosAtacar ] );   
        }              
        Red::Api::GetApiLogica()->AtacarZona(handlerUnidad, x, z);
        break;
    default:
        UnidadMultiSeleccionable::UnidadRespuestaEstrictaTerreno(x, z);
    }
}

void
UnidadAgresiva::UnidadRespuestaEstrictaUnidad(UnidadSeleccionable *unidad)
{
    switch ( UnidadSeleccionable::GetAccionActual() )
    {
    case UnidadSeleccionable::AccionMover:
        if ( numSonidosMover > 0 )
        {
            Graficos::Api::ReproducirSonido( unidadSonidosMover[ rand()%numSonidosMover ] );   
        }        
        Red::Api::GetApiLogica()->SeguirUnidad(handlerUnidad, unidad->GetHandlerUnidad());
        break;
    case UnidadSeleccionable::AccionAtacar:
        if ( numSonidosAtacar > 0 )
        {
            Graficos::Api::ReproducirSonido( unidadSonidosAtacar[ rand()%numSonidosAtacar ] );   
        }                     
        Red::Api::GetApiLogica()->AtacarUnidad(handlerUnidad, unidad->GetHandlerUnidad());
        break;
    default:
        UnidadMultiSeleccionable::UnidadRespuestaEstrictaUnidad(unidad);
    }
}

}
