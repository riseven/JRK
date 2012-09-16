#include "iUnidadMultiSeleccionable.h"

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
#include "iUnidadAgresiva.h"
#include "rApi.h"
#include "gApi.h"

#include "iIngeniero.h"
#include "iMedico.h"
#include "iTransporteBlindado.h"
#include "iZapador.h"


namespace Interfaz
{
list <UnidadMultiSeleccionable *> UnidadMultiSeleccionable::unidadesMultiSeleccionables ;
list <UnidadMultiSeleccionable *> UnidadMultiSeleccionable::unidadesSeleccionadas ;
vector < list<UnidadMultiSeleccionable *> > UnidadMultiSeleccionable::grupoSeleccion(10) ;
bool UnidadMultiSeleccionable::mover ;
bool UnidadMultiSeleccionable::detener ;
bool UnidadMultiSeleccionable::poseer ;
bool UnidadMultiSeleccionable::cancelar ;
bool UnidadMultiSeleccionable::accionRealizada ;

void
UnidadMultiSeleccionable::Inicializar()
{
    unidadesMultiSeleccionables.clear();
    unidadesSeleccionadas.clear();
    for ( vector< list<UnidadMultiSeleccionable *> >::iterator i = grupoSeleccion.begin() ; i != grupoSeleccion.end() ; ++i )
    {
        (*i).clear() ;
    }
    
    mover = false ;
    detener = false ;
    poseer = false ;
    cancelar = false ;
    
    accionRealizada = false ;
    
    Api::SetProgresoCarga( Api::GetProgresoCarga()+1.0 );
    Graficos::Api::SetPorcentajeBarraVida(Api::GetBarraProgresoCarga(), Api::GetProgresoCarga());
    Graficos::Api::Pintar();      
    
    Ingeniero::Inicializar();
    Medico::Inicializar();
    TransporteBlindado::Inicializar();
    Zapador::Inicializar();
    
    UnidadAgresiva::Inicializar();
}

void
UnidadMultiSeleccionable::Reset()
{
    unidadesMultiSeleccionables.clear();
    unidadesSeleccionadas.clear();
    
    for ( vector< list<UnidadMultiSeleccionable *> >::iterator i = grupoSeleccion.begin() ; i != grupoSeleccion.end() ; ++i )
    {
        (*i).clear() ;
    }
    
    mover = false ;
    detener = false ;
    poseer = false ;
    cancelar = false ;
    
    accionRealizada = false ;  
    
    Ingeniero::Reset();
    
    UnidadAgresiva::Reset();
}

void
UnidadMultiSeleccionable::Cerrar()
{
    unidadesMultiSeleccionables.clear();
    unidadesSeleccionadas.clear();
}

void
UnidadMultiSeleccionable::ActualizarUnidadesMultiSeleccionables()
{
    if ( UnidadSeleccionable::GetAccionActual() == UnidadSeleccionable::AccionNinguna )
    {
        if ( mover )
        { 
            // Muestro el menu de cancelar
            UnidadSeleccionable::SetAccionActual(UnidadSeleccionable::AccionMover);
            MostrarMenuCancelar();
            /*
            for ( list <UnidadMultiSeleccionable *>::iterator i = unidadesSeleccionadas.begin() ; i != unidadesSeleccionadas.end() ; ++i )
            {
                Red::Api::GetApiLogica()->MoverUnidad((*i)->handlerUnidad, rand()%40, rand()%40);
            }
            */
        }
        if ( detener )
        {
            for ( list <UnidadMultiSeleccionable *>::iterator i = unidadesSeleccionadas.begin() ; i != unidadesSeleccionadas.end() ; ++i )
            {
                Red::Api::GetApiLogica()->DetenerUnidad((*i)->handlerUnidad);
            }
        }
        if ( poseer )
        {
            for ( list <UnidadMultiSeleccionable *>::iterator i = unidadesSeleccionadas.begin() ; i != unidadesSeleccionadas.end() ; ++i )
            {
                Api::GetJuego()->PoseerUnidad((UnidadSeleccionable *)(*i));
                break ;
            }        
        }
    }
    else
    {
        if ( cancelar || accionRealizada)
        {
            accionRealizada = false ;
            ActualizarBotones();
        }
    }
    
    accionRealizada = false ;
    
    
    // Ahora comprobamos si todas las unidades son agresivas
    bool todasAgresivas = true ;
    for ( list <UnidadMultiSeleccionable *>::iterator i = unidadesSeleccionadas.begin() ; i != unidadesSeleccionadas.end() ; ++i )
    {
        if ( !(*i)->EsAgresiva() )
        {
            todasAgresivas = false ;
            break ;
        }
    }
    if ( todasAgresivas )
    {
        UnidadAgresiva::ActualizarUnidadesAgresivas();
    }
    
    
    // Ahora comprobamos si todas son iguales
    if ( unidadesSeleccionadas.size() > 0 )
    {
        bool todasIguales = true ;
        int tipo = unidadesSeleccionadas.front()->GetTipo() ;
        for ( list <UnidadMultiSeleccionable *>::iterator i = unidadesSeleccionadas.begin() ; i != unidadesSeleccionadas.end() ; ++i )
        {
            if ( (*i)->GetTipo() != tipo )
            {
                todasIguales = false ;
                break ;
            }
        }
        if ( todasIguales )
        {
            unidadesSeleccionadas.front()->Actualizar();
        }
    }    
}

void
UnidadMultiSeleccionable::MostrarMenuCancelar()
{    
    for ( int i = 0 ; i < 12 ; i++ )
    {
        Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(i, false);
    }
    
    Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(11, true);
    Api::GetInterfazPanel()->GetIntAcciones()->CambiarImagen(11, UnidadSeleccionable::imagenesBotones[UnidadSeleccionable::ImagenCancelarAccion]);    
    Api::GetInterfazPanel()->GetIntAcciones()->OnClickBoton(11, &cancelar);
    Api::GetInterfazPanel()->GetIntAcciones()->AsociarHotKey(11, KEY_C); 
    Api::GetInterfazPanel()->GetIntAcciones()->SetTextoBoton(11, "C");
    Api::GetInterfazPanel()->GetIntAcciones()->SetTextoToolTip(11, "Cancelar");
       
}

void
UnidadMultiSeleccionable::SeleccionarUnidad(UnidadMultiSeleccionable *unidad)
{
    for ( list <UnidadMultiSeleccionable *>::iterator i = unidadesSeleccionadas.begin() ; i != unidadesSeleccionadas.end() ; ++i )
    {
        (*i)->SetEstaSeleccionada(false);
        (*i)->HaSidoDeseleccionada();
    }
    unidadesSeleccionadas.clear();
    
    unidad->SetEstaSeleccionada(true);
    unidadesSeleccionadas.push_back(unidad);
    
    UnidadSeleccionable::SetMultiSeleccion(true);
    
    unidad->Seleccionar();
    
    ActualizarBotones();
}

void
UnidadMultiSeleccionable::AnyadirUnidad(UnidadMultiSeleccionable *unidad)
{
    for ( list <UnidadMultiSeleccionable *>::iterator i = unidadesSeleccionadas.begin() ; i != unidadesSeleccionadas.end() ; ++i )
    {
        if ( (*i) == unidad )
        {
            return ;
        }
    }
    
    unidad->SetEstaSeleccionada(true);
    unidadesSeleccionadas.push_back(unidad);
    
    UnidadSeleccionable::SetMultiSeleccion(true);
    
    unidad->Seleccionar();
    
    ActualizarBotones();
}

void
UnidadMultiSeleccionable::QuitarUnidad(UnidadMultiSeleccionable *unidad)
{
    for ( list <UnidadMultiSeleccionable *>::iterator i = unidadesSeleccionadas.begin() ; i != unidadesSeleccionadas.end() ; ++i )   
    {
        if ( (*i) == unidad )
        {
            unidad->SetEstaSeleccionada(false);
            unidadesSeleccionadas.remove(unidad);
            
            ActualizarBotones();
            
            unidad->Deseleccionar();
            break;
        }
    }
}

void
UnidadMultiSeleccionable::AsignarGrupo(int grupo)
{
    grupoSeleccion.at(grupo).clear();
    for ( list<UnidadMultiSeleccionable *>::iterator i = unidadesSeleccionadas.begin() ; i != unidadesSeleccionadas.end() ; ++i )
    {
        grupoSeleccion[grupo].push_back(*i);
    }
}

void
UnidadMultiSeleccionable::AnyadirGrupo(int grupo)
{
    for ( list<UnidadMultiSeleccionable *>::iterator i = grupoSeleccion.at(grupo).begin() ; i != grupoSeleccion[grupo].end() ; ++i )
    {
        AnyadirUnidad(*i);
    }
}

void
UnidadMultiSeleccionable::DeseleccionarTodo()
{ 
    for ( list <UnidadMultiSeleccionable *>::iterator i = unidadesSeleccionadas.begin() ; i != unidadesSeleccionadas.end() ; ++i )
    {
        (*i)->SetEstaSeleccionada(false);
        (*i)->HaSidoDeseleccionada();
    }
    unidadesSeleccionadas.clear();
    
    ActualizarBotones();
}

void
UnidadMultiSeleccionable::ActualizarBotones()
{
    Api::GetInterfazPanel()->GetIntSeleccion()->ActualizarSeleccion(unidadesSeleccionadas);
    UnidadSeleccionable::SetAccionActual(UnidadSeleccionable::AccionNinguna);
    
    for ( int i = 0 ; i < 12 ; i++ )
    {
        Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(i, false);
    }

    if ( unidadesSeleccionadas.size() != 0 )
    {
        // Primero pintamos los botones que tendran todas las unidades
        Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(0, true);
        Api::GetInterfazPanel()->GetIntAcciones()->CambiarImagen(0, UnidadSeleccionable::imagenesBotones[UnidadSeleccionable::ImagenMover]);
        Api::GetInterfazPanel()->GetIntAcciones()->OnClickBoton(0, &mover);
        Api::GetInterfazPanel()->GetIntAcciones()->AsociarHotKey(0, KEY_M);
        Api::GetInterfazPanel()->GetIntAcciones()->SetTextoBoton(0, "M");
        Api::GetInterfazPanel()->GetIntAcciones()->SetTextoToolTip(0, "Mover / Seguir");
                
        Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(2, true);
        Api::GetInterfazPanel()->GetIntAcciones()->CambiarImagen(2, UnidadSeleccionable::imagenesBotones[UnidadSeleccionable::ImagenDetener]);
        Api::GetInterfazPanel()->GetIntAcciones()->OnClickBoton(2, &detener);
        Api::GetInterfazPanel()->GetIntAcciones()->AsociarHotKey(2, KEY_D);
        Api::GetInterfazPanel()->GetIntAcciones()->SetTextoBoton(2, "D");
        Api::GetInterfazPanel()->GetIntAcciones()->SetTextoToolTip(2, "Detener");        
                
        Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(3, true);
        Api::GetInterfazPanel()->GetIntAcciones()->CambiarImagen(3, UnidadSeleccionable::imagenesBotones[UnidadSeleccionable::ImagenTomar]);            
        Api::GetInterfazPanel()->GetIntAcciones()->OnClickBoton(3, &poseer);
        Api::GetInterfazPanel()->GetIntAcciones()->AsociarHotKey(3, KEY_SPACE);        
        Api::GetInterfazPanel()->GetIntAcciones()->SetTextoBoton(3, "SPA");
        Api::GetInterfazPanel()->GetIntAcciones()->SetTextoToolTip(3, "Controlar unidad");        
                
        // Ahora comprobamos si todas las unidades son agresivas
        bool todasAgresivas = true ;
        for ( list <UnidadMultiSeleccionable *>::iterator i = unidadesSeleccionadas.begin() ; i != unidadesSeleccionadas.end() ; ++i )
        {
            if ( !(*i)->EsAgresiva() )
            {
                todasAgresivas = false ;
                break ;
            }
        }
        if ( todasAgresivas )
        {
            UnidadAgresiva::PintarBotones();
        }
        
        // Ahora comprobamos si todas las unidades son del mismo tipo
        bool todasIguales = true ;
        int tipo = unidadesSeleccionadas.front()->GetTipo();
        for ( list <UnidadMultiSeleccionable *>::iterator i = unidadesSeleccionadas.begin() ; i != unidadesSeleccionadas.end() ; ++i )
        {
            if ( (*i)->GetTipo() != tipo )
            {
                todasIguales = false ;
                break ;
            }
        }
        if ( todasIguales )
        {
            unidadesSeleccionadas.front()->PintarBotonesUnidad();        
        }
    }    
}

void
UnidadMultiSeleccionable::RespuestaInteligenteTerreno(float x, float z)
{
    for ( list <UnidadMultiSeleccionable *>::iterator i = unidadesSeleccionadas.begin() ; i != unidadesSeleccionadas.end() ; ++i )
    {
        (*i)->UnidadRespuestaInteligenteTerreno(x,z);
    }
    accionRealizada = true ;
}

void
UnidadMultiSeleccionable::RespuestaInteligenteUnidad(UnidadSeleccionable *unidad)
{
    for ( list <UnidadMultiSeleccionable *>::iterator i = unidadesSeleccionadas.begin() ; i != unidadesSeleccionadas.end() ; ++i )
    {
        (*i)->UnidadRespuestaInteligenteUnidad(unidad);
    }
    accionRealizada = true ;
}

void
UnidadMultiSeleccionable::RespuestaEstrictaTerreno(float x, float z)
{
    for ( list <UnidadMultiSeleccionable *>::iterator i = unidadesSeleccionadas.begin() ; i != unidadesSeleccionadas.end() ; ++i )
    {
        (*i)->UnidadRespuestaEstrictaTerreno(x,z);
    }
    accionRealizada = true ;
}

void
UnidadMultiSeleccionable::RespuestaEstrictaUnidad(UnidadSeleccionable *unidad)
{
    for ( list <UnidadMultiSeleccionable *>::iterator i = unidadesSeleccionadas.begin() ; i != unidadesSeleccionadas.end() ; ++i )
    {
        (*i)->UnidadRespuestaEstrictaUnidad(unidad);
    }
    accionRealizada = true ;
}
 
UnidadMultiSeleccionable::UnidadMultiSeleccionable(int numJugador):
    UnidadSeleccionable(numJugador)
{
    estaSeleccionada = false ;
    numSonidosMover = 0 ;
    numSonidosAtacar = 0 ;
}

UnidadMultiSeleccionable::~UnidadMultiSeleccionable()
{
    estaSeleccionada = false ;
}

void
UnidadMultiSeleccionable::Actualizar()
{
}

void
UnidadMultiSeleccionable::Seleccionar()
{
    //Marco como seleccionada
    //UnidadMultiSeleccionable::SeleccionarUnidad(this);
    
    UnidadSeleccionable::Seleccionar();
}

void
UnidadMultiSeleccionable::Deseleccionar()
{
    //Marco como seleccionada
    //UnidadMultiSeleccionable::SeleccionarUnidad(this);
    
    UnidadSeleccionable::Deseleccionar();
}


bool
UnidadMultiSeleccionable::EstaSeleccionada()
{
    return estaSeleccionada ;   
}

void
UnidadMultiSeleccionable::SetEstaSeleccionada(bool estaSeleccionada)
{
    UnidadMultiSeleccionable::estaSeleccionada = estaSeleccionada ;
}
  
void
UnidadMultiSeleccionable::EntrarPrimeraPersona()
{
    angY = GetAngulo() ;
    angXZ = 0.0 ;
    Graficos::Api::DesactivarObjeto(objetoHandler);
    
    // Borramos las mouse_mickeys
    int x, y ;
    get_mouse_mickeys(&x, &y);
}  

void
UnidadMultiSeleccionable::SalirPrimeraPersona()
{
    Graficos::Api::ActivarObjeto(objetoHandler);
}
    
void
UnidadMultiSeleccionable::ActualizarPrimeraPersona()
{
    float x, y, z ;
    GetPos(x,y,z);
    
    // Giramos la camara
    int mx, my ;
    get_mouse_mickeys(&mx, &my);
    angY += mx*0.1 ;
    angXZ += my*0.1 ;

    Graficos::Api::SetPosicionCamara(x, y, z);
    Graficos::Api::SetInclinacionCamara(0); 
    
    // Realizo las transformacions opengl de camara
    Graficos::Api::ResetCamara();
    Graficos::Api::EscalarCamara(2.0, 2.0, 2.0);
    Graficos::Api::RotarCamara(angXZ, 1, 0, 0);
    Graficos::Api::RotarCamara(angY+90, 0, 1, 0);
    Graficos::Api::TrasladarCamara(-x, -y-1.0, -z);
}

void
UnidadMultiSeleccionable::UnidadRespuestaInteligenteTerreno(float x, float z)
{
    if ( numSonidosMover > 0 )
    {
        Graficos::Api::ReproducirSonido( unidadSonidosMover[ rand()%numSonidosMover ] );   
    }    
    Red::Api::GetApiLogica()->MoverUnidad(handlerUnidad, x, z);
}

void
UnidadMultiSeleccionable::UnidadRespuestaInteligenteUnidad(UnidadSeleccionable *unidad)
{
    if ( numSonidosMover > 0 )
    {
        Graficos::Api::ReproducirSonido( unidadSonidosMover[ rand()%numSonidosMover ] );   
    }    
    Red::Api::GetApiLogica()->SeguirUnidad(handlerUnidad, unidad->GetHandlerUnidad());
}

void
UnidadMultiSeleccionable::UnidadRespuestaEstrictaTerreno(float x, float z)
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
    }    
}

void
UnidadMultiSeleccionable::UnidadRespuestaEstrictaUnidad(UnidadSeleccionable *unidad)
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
    }
}

void
UnidadMultiSeleccionable::SeleccionarUnidadOrden(int orden)
{
   int num=0;
   for( list <UnidadMultiSeleccionable *>::iterator i = unidadesSeleccionadas.begin() ; i != unidadesSeleccionadas.end() ; ++i, num++ )
      if(num==orden)
      {         
         SeleccionarUnidad( *i );
         break;
      }
}

void
UnidadMultiSeleccionable::DeseleccionarUnidadOrden(int orden)
{
   int num=0;
   for( list <UnidadMultiSeleccionable *>::iterator i = unidadesSeleccionadas.begin() ; i != unidadesSeleccionadas.end() ; ++i, num++ )
      if(num==orden)
      {        
         QuitarUnidad( *i );
         break;
      }
}
    
}

