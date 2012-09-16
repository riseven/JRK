#include "iApi.h"
#include "uFrecReal.h"
#include "gApi.h"
#include "iMenuPrincipal.h"
#include "lApi.h"
#include "eExcepcionParametro.h"
#include "rProtocolo.h"
#include "iBarracones.h"
#include "iIngeniero.h"
#include "rApi.h"
#include "iPetroleo.h"
#include "iPozoDeExtraccion.h"
#include "iConfiguracion.h"
#include <stdexcept>


namespace Interfaz
{

int Api::barraProgresoCarga ;
float Api::progresoCarga = 0.0 ; 
vector <Unidad *> Api::unidades ;
vector <Jugador *> Api::jugadores ;
InfoMapa *Api::infoMapa = NULL ;
MenuPartida *Api::menuPartida = NULL ;
InterfazPanel *Api::intPanel = NULL ;
Juego *Api::juego = NULL ;
Menu *Api::menuActual = NULL ;
int Api::imagenRaton = -1 ;
int Api::sonidoInvestigacionCompletada = -1 ;
int Api::sonidoConstruccionCompletada = -1 ;


void
Api::Inicializar()
{
    try
    {
        int imagenPresentacion = Graficos::Api::CrearImagen("Graficos\\Presentacion.bmp");
        Graficos::Api::SetImagenMasked(imagenPresentacion, true);
        int spritePresentacion = Graficos::Api::CrearSprite(imagenPresentacion, 0, 0, 1);
        
        barraProgresoCarga = Graficos::Api::CrearBarraVida(924, 30, 2, 50, 688, 2);
        Graficos::Api::SetPorcentajeBarraVida(barraProgresoCarga, 0);
        
        Unidad::Inicializar();
        
        Menu::Inicializar();
        
        Graficos::Api::DestruirSprite(spritePresentacion);
        Graficos::Api::DestruirImagen(imagenPresentacion);
        
        Graficos::Api::DestruirBarraVida(barraProgresoCarga);
        
        // Inicializamos las unidades
        
        
        // Creamos el raton
        imagenRaton = Graficos::Api::CrearImagen("Graficos\\Mouse.bmp");
        Graficos::Api::SetImagenMasked(imagenRaton, true);
        Graficos::Api::SetImagenRaton(imagenRaton);
        Graficos::Api::MostrarRaton();
        
        // Creamos el menu principal
        menuActual = new MenuPrincipal();
        
        // Cargamos sonidos
        sonidoInvestigacionCompletada = Graficos::Api::CrearSonido("Sonidos\\investigacionfinalizada.wav");
        sonidoConstruccionCompletada = Graficos::Api::CrearSonido("Sonidos\\edificiocompletado.wav");
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::Api::Inicializar()").Combinar(ex);
    }
}

void
Api::SetProgresoCarga( float valor )
{
    progresoCarga += (valor-progresoCarga)*1.0;
}

void
Api::Cerrar()
{
    try
    {
        // Menu::Cerrar() ;
        Unidad::Cerrar();
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::Api::Cerrar()").Combinar(ex);
    }
}


void
Api::NuevoJugador(int numJugador, string nombreJugador)
{
    try
    {
        while ( jugadores.size() <= numJugador )
        {
            jugadores.push_back(NULL);
        }
        jugadores.at(numJugador) = new Jugador() ;
        jugadores.at(numJugador)->SetNombre(nombreJugador);
        jugadores.at(numJugador)->SetColor(0);
        menuPartida->NuevoJugador(numJugador, nombreJugador);    
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::Api::NuevoJugador(int, string)").Combinar(ex);
    }
}

void
Api::AbandonoJugador(int numJugador)
{
    menuPartida->AbandonoJugador(numJugador);
    delete jugadores.at(numJugador) ;
    jugadores.at(numJugador) = NULL ;
}

void
Api::SetJugadorPropio(int numJugador)
{
    Jugador::SetJugadorPropio(numJugador);
}

void
Api::SetColorJugador(int numJugador, int color)
{
    jugadores.at(numJugador)->SetColor(color);
}

void
Api::MensajeChatMenu(int numJugador, string texto)
{
    menuPartida->MensajeChat(numJugador, texto);
}

void
Api::SetMapa(string nombreFichero)
{
    if ( infoMapa )
    {
        delete infoMapa ;
    }
    infoMapa = new InfoMapa(nombreFichero);    
}

void
Api::EmpezarPartida()
{
    menuPartida->EmpezarPartida();    
}

void
Api::SetEstadoTecnologia(int numJugador, int numTecnologia, int estado)
{
    if ( !jugadores.at(numJugador) )
    {
        throw Error::ExcepcionParametro("Interfaz::Api::SetEstadoTecnologia(int, int, int)", 1, "Jugador no existe");
    }
    
    if ( estado == Red::Protocolo::EstadoTecnologia::Desarrollado &&
         numJugador == Jugador::GetJugadorPropio() )
    {
        Graficos::Api::ReproducirSonido(sonidoInvestigacionCompletada);
    }
    
    jugadores.at(numJugador)->SetEstadoTecnologia(numTecnologia, estado);
}

void
Api::SetDinero(int numJugador, int dinero)
{
    if ( !jugadores.at(numJugador) )
    {
        throw Error::ExcepcionParametro("Interfaz::Api::SetDiner(int, int)", 1, "Jugador no existe");
    }
    jugadores.at(numJugador)->SetDinero(dinero);
    
    if ( numJugador == Jugador::GetJugadorPropio() )
    {
        GetInterfazPanel()->GetBarraSuperior()->SetDineroActual(dinero);
    }
}

void
Api::SetOcupadoPetroleo(int numPetroleo, bool ocupado)
{
    GetInfoMapa()->SetPetroleoOcupado(numPetroleo, ocupado);
}

void
Api::SetDia(bool dia)
{
    if ( Configuracion::GetNoche() )
    {
        if ( dia )
        {
            Graficos::Api::SetIluminacionDifusa(100, 0.8, 0.8, 0.8, 1.0);
            Graficos::Api::SetIluminacionAmbiente(100, 0.3, 0.3, 0.3, 1.0);
        }
        else
        {
            Graficos::Api::SetIluminacionDifusa(100, 0.3, 0.3, 0.7, 1.0);
            Graficos::Api::SetIluminacionAmbiente(100, 0.0, 0.0, 0.0, 1.0);        
        }
    }
}

void
Api::SetInvierno(bool invierno)
{
    if ( Configuracion::GetEstaciones() )
    {
        Graficos::Api::SetInvierno(invierno);
    }
}

void    
Api::CrearUnidad(int handlerUnidad, int numJugador, int tipo, float x, float y, float z)
{
    try
    {
        while ( handlerUnidad >= unidades.size() )
        {
            unidades.push_back( NULL );
        }
    
        // Comprobamos si avisamos de construccion de edificio
        if ( numJugador == Jugador::GetJugadorPropio() )
        {
            switch (tipo)
            {
                case Red::Protocolo::Unidad::CuartelGeneral:
                case Red::Protocolo::Unidad::Barracones:
                case Red::Protocolo::Unidad::PozoDeExtraccion:
                    Graficos::Api::ReproducirSonido(sonidoConstruccionCompletada);
                    break ;
    
            }
        }
    
        switch (tipo)
        {
            case Red::Protocolo::Unidad::Petroleo:
                {
                    Petroleo *pet = new Petroleo();
                    pet->SetHandlerUnidad(handlerUnidad);
                    unidades.at(handlerUnidad) = pet ;
                }
                break ;
            case Red::Protocolo::Unidad::Soldado:
                {
                    Soldado *sol = new Soldado(numJugador) ;
                    sol->SetHandlerUnidad(handlerUnidad);
                    unidades.at(handlerUnidad) = sol ;
                }
                break ;
            case Red::Protocolo::Unidad::Ingeniero:
                {
                    Ingeniero *ing = new Ingeniero(numJugador) ;
                    ing->SetHandlerUnidad(handlerUnidad);
                    unidades.at(handlerUnidad) = ing ;
                }
                break ;
            case Red::Protocolo::Unidad::CuartelGeneral:
                {
                    CuartelGeneral *cg = new CuartelGeneral(numJugador);
                    cg->SetHandlerUnidad(handlerUnidad);
                    unidades.at(handlerUnidad) = cg;
                }
                break ;
            case Red::Protocolo::Unidad::Barracones:
                {
                    Barracones *b = new Barracones(numJugador);
                    b->SetHandlerUnidad(handlerUnidad);
                    unidades.at(handlerUnidad) = b ;
                }
                break ; 
            case Red::Protocolo::Unidad::PozoDeExtraccion:
                {
                    PozoDeExtraccion *pozo = new PozoDeExtraccion(numJugador);
                    pozo->SetHandlerUnidad(handlerUnidad);
                    unidades.at(handlerUnidad) = pozo ;
                }
                break ;
            default:
                throw Error::ExcepcionParametro("Interfaz::Api::CrearUnidad(int, int, float, float, float)",1,"Unidad no reconocida");
        }
        
        unidades.at(handlerUnidad)->SetPos(x, y, z);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::Api::CrearUnidad(int, int, float, float, float)").Combinar(ex);
    }
}



void
Api::MoverUnidad(int handlerUnidad, float x, float y, float z)
{
    try
    {
        if ( unidades.at(handlerUnidad) == NULL )
        {
            throw Error::ExcepcionParametro("Interfaz::Api::MoverUnidad(int, float, float, float)", 1);
        }
        unidades.at(handlerUnidad)->SetPos(x, y, z);
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Interfaz::Api::MoverUnidad(int, float, float, float)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::Api::MoverUnidad(int, float, float, float)").Combinar(ex);
    }
}

void
Api::SetAnguloUnidad(int handlerUnidad, float angulo)
{
    try
    {
        if ( unidades.at(handlerUnidad) == NULL )
        {
            throw Error::ExcepcionParametro("Interfaz::Api::SetAnguloUnidad(int, float)", 1);
        }    
        unidades.at(handlerUnidad)->SetAngulo(angulo);
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Interfaz::Api::SetAnguloUnidad(int, float)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::Api::SetAnguloUnidad(int, float)").Combinar(ex);
    }
}

void
Api::DestruirUnidad(int handlerUnidad)
{
    try
    {
        if ( unidades.at(handlerUnidad) == NULL )
        {
            throw Error::ExcepcionParametro("Interfaz::Api::DestruirUnida(int)", 1);
        }
        delete unidades.at(handlerUnidad) ;
        unidades.at(handlerUnidad) = NULL ;
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Interfaz::Api::DestruirUnida(int)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::Api::DestruirUnida(int)").Combinar(ex);
    }
}

void
Api::SetPorcentajeConstruidoUnidad(int handlerUnidad, int porcentajeConstruido)
{
    try
    {
        if ( unidades.at(handlerUnidad) == NULL )
        {
            throw Error::ExcepcionParametro("Interfaz::Api::SetPorcentajeConstruidoUnidad(int, int)", 1);
        }
        unidades.at(handlerUnidad)->SetPorcentajeConstruido(porcentajeConstruido);
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Interfaz::Api::SetPorcentajeConstruidoUnidad(int, int)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::Api::SetPorcentajeConstruidoUnidad(int, int)").Combinar(ex);
    }
}    

void
Api::MensajeChatJuego(int numJugador, string texto)
{
    if ( juego )
    {
        juego->MensajeChat(numJugador, texto);
    }
}

void
Api::SetInfoMapa(InfoMapa *infoMapa)
{
    Api::infoMapa = infoMapa ;
}

InfoMapa *
Api::GetInfoMapa()
{
    return infoMapa ;
}

void
Api::SetMenuPartida(MenuPartida *menuPartida)
{
    Api::menuPartida = menuPartida ;
}

int
Api::GetMaxJugadores()
{
    try
    {
        return infoMapa->GetMaxJugadores() ;
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::Api::GetMaxJugadores()").Combinar(ex);
    }
}


        

void
Api::BuclePrincipal()
{
    try
    {
        Util::FrecReal frecuenciaLogica(20);
        Util::FrecReal frecuenciaInterfaz(30); // Para evitar el acoplamiento
        
        bool salir = false ;
        while ( !salir )
        {
            /*
            if ( frecuenciaLogica.comprobar() )
            {
                Logica::Api::Actualizar();            
            }
            if ( menuActual == NULL )
            {
                salir = true ;
            }
            else
            {
                if ( frecuenciaInterfaz.comprobar() )
                {
                    menuActual->Actualizar() ;
                    //UnidadSeleccionable::ActualizarUnidadesSeleccionables();                    antiguo = 
                    Graficos::Api::Pintar();
                }
            }
            */
            Util::FrecReal::ActualizarTiempo();
            if ( frecuenciaInterfaz.comprobar() )
            {
                while ( frecuenciaLogica.comprobar() )
                {
                    Logica::Api::Actualizar();
                }
                Red::Api::Actualizar();
                if ( menuActual == NULL )
                {
                    salir = true ;
                }
                else
                {
                    menuActual->Actualizar() ;
                    Graficos::Api::Pintar();   
                }
            }
        }
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::Api::BuclePrincipal()").Combinar(ex);
    }
}

void
Api::SetMenuActual(Menu *menuActual)
{
    Api::menuActual = menuActual ;
}

void
Api::SetInterfazPanel(InterfazPanel * interfazPanel)
{
    intPanel = interfazPanel ;
}

InterfazPanel *
Api::GetInterfazPanel()
{
    return intPanel ;
}

void
Api::SetJuego(Juego *juego)
{
    Api::juego = juego ;
}

Juego *
Api::GetJuego()
{
    return juego ;
}

vector <Unidad *> 
Api::GetListaUnidades()
{
   return unidades;
}

Jugador *
Api::GetJugador(int numJugador)
{
    if ( jugadores.at(numJugador) == NULL )
    {
        throw Error::ExcepcionParametro("Interfaz::Api::GetJugador(int)", 1, "Jugador no existe");
    }
    return jugadores.at(numJugador) ;
}

}


