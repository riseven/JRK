#include "iIngeniero.h"
#include "gApi.h"
#include "iApi.h"
#include "eExcepcionLocalizada.h"
#include "rProtocolo.h"
#include "iCuartelGeneral.h"
#include "iBarracones.h"
#include "iPozoDeExtraccion.h"
#include "rApi.h"

namespace Interfaz
{

int Ingeniero::textura ;
int Ingeniero::modeloHandler[8] ;

bool Ingeniero::construirPulsado = false;
bool Ingeniero::repararPulsado = false ;
bool Ingeniero::construirCuartelGeneral = false ;
bool Ingeniero::construirBarracones = false ;
bool Ingeniero::construirPozo = false ;
bool Ingeniero::cancelar = false ;
bool Ingeniero::situandoEdificio = false ;
int Ingeniero::tipoEdificio = -1 ;
int Ingeniero::objetoEdificio = -1 ;
int Ingeniero::sonidosMover[3] ;

//Crea el cuartel general a partir de su modelo y lo marca como seleccionable
Ingeniero::Ingeniero(int numJugador):
    UnidadMultiSeleccionable(numJugador)
{
    try
    {
        int color = Api::GetJugador(numJugador)->GetColor();
        objetoHandler = Graficos::Api::CrearObjeto(modeloHandler[color]);
        Graficos::Api::SetObjetoSeleccionable(objetoHandler, true);
        Graficos::Api::SetObjetoBloqueante(objetoHandler, true);
        
        float radio = Graficos::Api::GetRadioObjeto(objetoHandler);
        Graficos::Api::SetEscalaObjeto(handlerCirculoSeleccion, radio*1.25);  
        
        unidadSonidosMover[0] = sonidosMover[0] ;
        unidadSonidosMover[1] = sonidosMover[1] ;
        unidadSonidosMover[2] = sonidosMover[2] ;
        numSonidosMover = 3 ;
        
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::Ingeniero::Ingeniero()").Combinar(ex);
    }
}

//Destruye el modelo del cuartel general
Ingeniero::~Ingeniero()
{
    try
    {
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::Ingeniero::~Ingeniero()").Combinar(ex);
    }
}

//Selecciona el cuartel general y muestra las acciones disponibles para éste
void
Ingeniero::Seleccionar()
{
    UnidadMultiSeleccionable::Seleccionar();
}    

void
Ingeniero::Actualizar()
{
    ActualizarIngeniero();
}

//Obtiene el tipo de unidad
int
Ingeniero::GetTipo()
{
    return Red::Protocolo::Unidad::Ingeniero ;
}

void
Ingeniero::PintarBotonesUnidad()
{
    Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(8, true);
    Api::GetInterfazPanel()->GetIntAcciones()->CambiarImagen(8, UnidadSeleccionable::imagenesBotones[ UnidadSeleccionable::ImagenConstruirEdificio ]);      
    Api::GetInterfazPanel()->GetIntAcciones()->OnClickBoton(8, &construirPulsado);
    Api::GetInterfazPanel()->GetIntAcciones()->AsociarHotKey(8, KEY_C);
    Api::GetInterfazPanel()->GetIntAcciones()->SetTextoBoton(8, "C");
    Api::GetInterfazPanel()->GetIntAcciones()->SetTextoToolTip(8, "Construir edificio");    

    Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(9, true);
    Api::GetInterfazPanel()->GetIntAcciones()->CambiarImagen(9, UnidadSeleccionable::imagenesBotones[ UnidadSeleccionable::ImagenRepararIngeniero ]);    
    Api::GetInterfazPanel()->GetIntAcciones()->OnClickBoton(9, &repararPulsado);
    Api::GetInterfazPanel()->GetIntAcciones()->AsociarHotKey(9, KEY_R);
    Api::GetInterfazPanel()->GetIntAcciones()->SetTextoBoton(9, "R");
    Api::GetInterfazPanel()->GetIntAcciones()->SetTextoToolTip(9, "Reparar edificio o vehiculo");    
}

void
Ingeniero::UnidadRespuestaInteligenteUnidad(UnidadSeleccionable *unidad)
{
    if ( unidad->EsReparable() )
    {
        Red::Api::GetApiLogica()->RepararIngeniero(handlerUnidad, unidad->GetHandlerUnidad());
    }
    else
    {
        UnidadMultiSeleccionable::UnidadRespuestaInteligenteUnidad(unidad);
    }  
}

void
Ingeniero::UnidadRespuestaEstrictaTerreno(float x, float z)
{
    if ( situandoEdificio )
    {
        SituarEdificio(x, z);        
    }
    else
    {
        UnidadMultiSeleccionable::UnidadRespuestaEstrictaTerreno(x,z);
    }
}

void
Ingeniero::UnidadRespuestaEstrictaUnidad(UnidadSeleccionable *unidad)
{
    if ( situandoEdificio )
    {
        Util::Vector3D pos = Graficos::Api::GetCoordenadas2Dto3D(Graficos::Api::CoordenadasVentana::PosicionRaton);
        SituarEdificio( pos.GetX(), pos.GetZ() );
    }   
    else
    {
        switch ( UnidadSeleccionable::GetAccionActual() )
        {
        case UnidadSeleccionable::AccionReparar:
            if ( unidad->EsReparable() )
            {
                Red::Api::GetApiLogica()->RepararIngeniero(handlerUnidad, unidad->GetHandlerUnidad());
            }
            break ;
        default:
            UnidadMultiSeleccionable::UnidadRespuestaEstrictaUnidad(unidad);
        }
    }
}

void
Ingeniero::Inicializar()
{
    try
    {
        textura = Graficos::Api::CrearImagen("Unidades\\Ingeniero\\textura.bmp");
        for ( int i = 0 ; i < 8 ; i++ )
        {
            modeloHandler[i] = Graficos::Api::CrearModelo("Unidades\\Ingeniero\\modelo.3ds", textura, Jugador::GetColorRojo(i), Jugador::GetColorVerde(i), Jugador::GetColorAzul(i) );

            Api::SetProgresoCarga( Api::GetProgresoCarga()+1.0 );
            Graficos::Api::SetPorcentajeBarraVida(Api::GetBarraProgresoCarga(), Api::GetProgresoCarga());
            Graficos::Api::Pintar();    
        }
        sonidosMover[0] = Graficos::Api::CrearSonido("Sonidos\\miedo.wav");
        sonidosMover[1] = Graficos::Api::CrearSonido("Sonidos\\yavoy.wav");        
        sonidosMover[2] = Graficos::Api::CrearSonido("Sonidos\\vamos.wav");        
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::Ingeniero::Inicializar()").Combinar(ex);
    }
}

void
Ingeniero::Reset()
{
    construirPulsado = false ;
    repararPulsado = false ;
    construirCuartelGeneral = false ;
    construirBarracones = false ;
    cancelar = false ;
    situandoEdificio = false ;
    tipoEdificio = -1 ;
    if ( objetoEdificio != -1 )
    {
        Graficos::Api::DestruirObjeto(objetoEdificio);
        objetoEdificio = -1 ;
    }
}

void 
Ingeniero::ActualizarIngeniero()
{    
    // Actualizo la posicion del edificio
    if ( objetoEdificio != -1 )
    {

        Util::Vector3D pos = Graficos::Api::GetCoordenadas2Dto3D(Graficos::Api::CoordenadasVentana::PosicionRaton);
        float x = pos.GetX();
        float z = pos.GetZ();
        float escala = Graficos::Api::GetEscalaTerreno();
        float y = Graficos::Api::GetAltura(x/escala, z/escala);
        Graficos::Api::MoverObjeto(objetoEdificio, x, y, z);

        if ( tipoEdificio != Red::Protocolo::Unidad::PozoDeExtraccion )
        {
            // Comprobamos si colisiona con algo
            // Faltaria tambien comprobar el mapa de durezas, pero de momento no se hace
            if ( Graficos::Api::ColisionObjeto(objetoEdificio) )
            {
                Graficos::Api::SetMascaraObjeto(objetoEdificio, true, false, false, true);
            }
            else
            {
                Graficos::Api::SetMascaraObjeto(objetoEdificio, true, true, true, true);
            }
        }
        else
        {
            // Comprobamos si estamos sobre un charco de petroleo libre
            bool puesto = false ;
            int i = 0 ;
            for ( i = 0 ; i < Api::GetInfoMapa()->GetNumPetroleos() ; i++ )
            {
                Util::Vector2D pos = Api::GetInfoMapa()->GetPosicionPetroleo(i) ;
                if ( pos.getX() == ((int)(x/escala)) && pos.getY() == ((int)(z/escala)) )
                {
                    if ( !Api::GetInfoMapa()->GetPetroleoOcupado(i) )
                    {
                        puesto = true ;
                        Graficos::Api::SetMascaraObjeto(objetoEdificio, true, true, true, true);
                        x = ((int)(x/escala))*escala + escala/2 ;
                        z = ((int)(z/escala))*escala + escala/2 ;
                        y = Graficos::Api::GetAlturaSuave(x, z);
                        Graficos::Api::MoverObjeto(objetoEdificio, x, y, z);
                    }
                    break ;
                }
            }
            if ( !puesto )
            {
                Graficos::Api::SetMascaraObjeto(objetoEdificio, true, false, false, true);
            }
        }
    } 
    
    bool quitarBotonesEdificios = false ;
    
    if ( construirPulsado )
    {
        construirPulsado = false ;
        
        for ( int i = 0 ; i < 12 ; i++ )
        {
            Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(i, false);
        }
        
        Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(0, true);
        Api::GetInterfazPanel()->GetIntAcciones()->CambiarImagen(0, UnidadSeleccionable::imagenesBotones[ UnidadSeleccionable::ImagenConstruirCuartelGeneral ]);      
        Api::GetInterfazPanel()->GetIntAcciones()->OnClickBoton(0, &construirCuartelGeneral);            
        Api::GetInterfazPanel()->GetIntAcciones()->AsociarHotKey(0, KEY_U);        
        Api::GetInterfazPanel()->GetIntAcciones()->SetTextoBoton(0, "U");
        Api::GetInterfazPanel()->GetIntAcciones()->SetTextoToolTip(0, "Construir cuartel general. Coste 1000");

        Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(1, true);
        Api::GetInterfazPanel()->GetIntAcciones()->CambiarImagen(1, UnidadSeleccionable::imagenesBotones[ UnidadSeleccionable::ImagenConstruirBarracones ]);      
        Api::GetInterfazPanel()->GetIntAcciones()->OnClickBoton(1, &construirBarracones);
        Api::GetInterfazPanel()->GetIntAcciones()->AsociarHotKey(1, KEY_B);
        Api::GetInterfazPanel()->GetIntAcciones()->SetTextoBoton(1, "B");
        Api::GetInterfazPanel()->GetIntAcciones()->SetTextoToolTip(1, "Construir barracones. Coste 350");        
        
        Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(4, true);
        Api::GetInterfazPanel()->GetIntAcciones()->CambiarImagen(4, UnidadSeleccionable::imagenesBotones[ UnidadSeleccionable::ImagenConstruirPozoDeExtraccion ]);      
        Api::GetInterfazPanel()->GetIntAcciones()->OnClickBoton(4, &construirPozo);
        Api::GetInterfazPanel()->GetIntAcciones()->AsociarHotKey(4, KEY_P); 
        Api::GetInterfazPanel()->GetIntAcciones()->SetTextoBoton(4, "P");
        Api::GetInterfazPanel()->GetIntAcciones()->SetTextoToolTip(4, "Construir pozo de extraccion petrolifera. Coste 500");

        Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(11, true);
        Api::GetInterfazPanel()->GetIntAcciones()->CambiarImagen(11, UnidadSeleccionable::imagenesBotones[ UnidadSeleccionable::ImagenCancelarAccion ]);      
        Api::GetInterfazPanel()->GetIntAcciones()->OnClickBoton(11, &cancelar);                    
        Api::GetInterfazPanel()->GetIntAcciones()->AsociarHotKey(11, KEY_C);
        Api::GetInterfazPanel()->GetIntAcciones()->SetTextoBoton(11, "C");
        Api::GetInterfazPanel()->GetIntAcciones()->SetTextoToolTip(11, "Cancelar");        
    }
    else if ( repararPulsado )
    {
        repararPulsado = false ;
        UnidadSeleccionable::SetAccionActual(UnidadSeleccionable::AccionReparar);
        UnidadMultiSeleccionable::MostrarMenuCancelar();
    }
    else if ( construirCuartelGeneral )
    {
        objetoEdificio = Graficos::Api::CrearObjeto( CuartelGeneral::GetModeloHandler( Api::GetJugador( Jugador::GetJugadorPropio() )->GetColor() ));        
        Graficos::Api::SetAnguloObjeto( objetoEdificio, 90);
        situandoEdificio = true ;
        tipoEdificio = Red::Protocolo::Unidad::CuartelGeneral ;
        UnidadSeleccionable::SetAccionActual( UnidadSeleccionable::SituarEdificio );
        
        quitarBotonesEdificios = true ;
        construirCuartelGeneral = false ;
    }
    else if ( construirBarracones )
    {
        objetoEdificio = Graficos::Api::CrearObjeto( Barracones::GetModeloHandler(Api::GetJugador(Jugador::GetJugadorPropio())->GetColor() ));        
        Graficos::Api::SetAnguloObjeto( objetoEdificio, 90);        
        situandoEdificio = true ;
        tipoEdificio = Red::Protocolo::Unidad::Barracones ;
        UnidadSeleccionable::SetAccionActual( UnidadSeleccionable::SituarEdificio );
        
        quitarBotonesEdificios = true ;
        construirBarracones = false ;
    }
    else if ( construirPozo )
    {
        objetoEdificio = Graficos::Api::CrearObjeto( PozoDeExtraccion::GetModeloHandler(Api::GetJugador(Jugador::GetJugadorPropio())->GetColor() ));        
        Graficos::Api::SetAnguloObjeto( objetoEdificio, 90);        
        situandoEdificio = true ;
        tipoEdificio = Red::Protocolo::Unidad::PozoDeExtraccion ;
        UnidadSeleccionable::SetAccionActual( UnidadSeleccionable::SituarEdificio );
        
        quitarBotonesEdificios = true ;
        construirPozo = false ;
    }    
    else if ( cancelar )
    {
        cancelar = false ;
        UnidadMultiSeleccionable::ActualizarBotones();
        situandoEdificio = false ;
    }
    
    // Comprobamos si se ha pulsado en un edificio y por tanto hay que quitar los botones
    // de construccion de edificio
    if ( quitarBotonesEdificios )
    {
        for ( int i = 0 ; i < 11 ; i++ )
        {
            Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(i, false);
        }
    }
    
    // Comprobamos si se ha dejado de situar el edificio
    if ( !situandoEdificio && objetoEdificio != -1 )
    {
        Graficos::Api::DestruirObjeto(objetoEdificio);
        objetoEdificio = -1 ;
    }
}    

void
Ingeniero::SituarEdificio(float x, float z)
{
    // Comprobamos que se puede construir
    bool puedeConstruir = false ;
    
    if ( tipoEdificio != Red::Protocolo::Unidad::PozoDeExtraccion )
    {
        // Comprobamos si colisiona con algo
        // Faltaria tambien comprobar el mapa de durezas, pero de momento no se hace
        if ( !Graficos::Api::ColisionObjeto(objetoEdificio) )
        {
            puedeConstruir = true ;
        }
    }
    else
    {
        // Comprobamos si estamos sobre un charco de petroleo libre
        int i = 0 ;
        float escala = Graficos::Api::GetEscalaTerreno() ;
        for ( i = 0 ; i < Api::GetInfoMapa()->GetNumPetroleos() ; i++ )
        {
            if ( !Api::GetInfoMapa()->GetPetroleoOcupado(i) )            
            {                
                Util::Vector2D pos = Api::GetInfoMapa()->GetPosicionPetroleo(i) ;
                if ( pos.getX() == ((int)(x/escala)) && pos.getY() == ((int)(z/escala)) )
                {
                    puedeConstruir = true ;
                    x = ((int)(x/escala))*escala + escala/2 ;
                    z = ((int)(z/escala))*escala + escala/2 ;
                    break ;
                }
            }
        }
    }
    
    
    
    if ( puedeConstruir )
    {
        // Notificamos a la logica
        list<UnidadMultiSeleccionable *> unidadesSeleccionadas = UnidadMultiSeleccionable::GetUnidadesSeleccionadas();
        int *ingenieros = new int[unidadesSeleccionadas.size()];
        int pos = 0 ;
        for ( list<UnidadMultiSeleccionable *>::iterator i = unidadesSeleccionadas.begin() ; i != unidadesSeleccionadas.end() ; ++i )
        {
            ingenieros[pos++] = (*i)->GetHandlerUnidad() ;
        }
        Red::Api::GetApiLogica()->ConstruirEdificio(tipoEdificio, Jugador::GetJugadorPropio(), x, z, unidadesSeleccionadas.size(), ingenieros);
        delete[] ingenieros ;
    }
    
    situandoEdificio = false ;
    if ( objetoEdificio != -1 )
    {
        Graficos::Api::DestruirObjeto(objetoEdificio);
        objetoEdificio = -1 ;
    }
}

}
