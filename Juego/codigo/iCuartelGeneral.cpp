#include "iCuartelGeneral.h"
#include "gApi.h"
#include "iApi.h"
#include "eExcepcionLocalizada.h"
#include "rProtocolo.h"
#include "rApi.h"

namespace Interfaz
{

int CuartelGeneral::textura ;
int CuartelGeneral::modeloHandler[8] ;

//Crea el cuartel general a partir de su modelo y lo marca como seleccionable
CuartelGeneral::CuartelGeneral(int numJugador):
    UnidadUniSeleccionable(numJugador)
{
    try  
    {
        int color = Api::GetJugador(numJugador)->GetColor();
        objetoHandler = Graficos::Api::CrearObjeto(modeloHandler[color]);
        Graficos::Api::SetObjetoSeleccionable(objetoHandler, true);
        Graficos::Api::SetObjetoBloqueante(objetoHandler, true);
        
        float radio = Graficos::Api::GetRadioObjeto(objetoHandler);
        Graficos::Api::SetEscalaObjeto(handlerCirculoSeleccion, radio*1.25);
        
        construirIngeniero = false ;
        desarrollarMotivacion = false ;
        desarrollarMotivacionMasDistancia = false ;
        porcentajeConstruido = -1 ;
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::CuartelGeneral::CuartelGeneral()").Combinar(ex);
    }
}

//Destruye el modelo del cuartel general
CuartelGeneral::~CuartelGeneral()
{
    try
    {         
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::CuartelGeneral::~CuartelGeneral()").Combinar(ex);
    }
}

void
CuartelGeneral::SetPorcentajeConstruido(int porcentaje)
{
    if ( EstaSeleccionada() )
    {
        if ( porcentajeConstruido == -1 && porcentaje != -1)
        {
            Api::GetInterfazPanel()->GetIntSeleccion()->SetVisibleBarraProgreso(true);
        
            Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(8, false);
            Api::GetInterfazPanel()->GetIntAcciones()->OnClickBoton(8, NULL);     
        }
        if ( porcentaje == -1 && porcentajeConstruido != -1)
        {
            Api::GetInterfazPanel()->GetIntSeleccion()->SetVisibleBarraProgreso(false);
            MostrarBotonesTecnologia();
        }
        Api::GetInterfazPanel()->GetIntSeleccion()->SetPorcentajeBarraProgreso(porcentaje);
    }
    porcentajeConstruido = porcentaje ;
}

//Selecciona el cuartel general y muestra las acciones disponibles para éste
void
CuartelGeneral::Seleccionar()
{
    UnidadUniSeleccionable::Seleccionar();
    
    for ( int i = 0 ; i < 12 ; i++ )
    {
        Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(i, false);
    } 
    
    // Cambia el boton de accion 0 (Ingeniero)
    Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(0, true);
    Api::GetInterfazPanel()->GetIntAcciones()->CambiarImagen(0, UnidadSeleccionable::imagenesBotones[UnidadSeleccionable::ImagenConstruirIngeniero]);
    Api::GetInterfazPanel()->GetIntAcciones()->OnClickBoton(0, &construirIngeniero);
    Api::GetInterfazPanel()->GetIntAcciones()->AsociarHotKey(0, KEY_I);
    Api::GetInterfazPanel()->GetIntAcciones()->SetTextoBoton(0, "I");
    Api::GetInterfazPanel()->GetIntAcciones()->SetTextoToolTip(0, "Construir ingeniero. Coste 50");
    
    MostrarBotonesTecnologia();
    
    if ( porcentajeConstruido == -1 )
    {
        Api::GetInterfazPanel()->GetIntSeleccion()->SetVisibleBarraProgreso(false);
    }
    else
    {
        Api::GetInterfazPanel()->GetIntSeleccion()->SetVisibleBarraProgreso(true);
        Api::GetInterfazPanel()->GetIntSeleccion()->SetPorcentajeBarraProgreso(porcentajeConstruido);        
    }
}    

void
CuartelGeneral::MostrarBotonesTecnologia()
{
    // Controlamos los botones de tecnologias
    // Ponemos el boton de accion 8 (Motivacion)
    if ( Api::GetJugador(numJugador)->GetEstadoTecnologia(Red::Protocolo::Tecnologia::Motivacion) == Red::Protocolo::EstadoTecnologia::NoDesarrollado )
    {
        Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(8, true);
        Api::GetInterfazPanel()->GetIntAcciones()->CambiarImagen(8, UnidadSeleccionable::imagenesBotones[UnidadSeleccionable::ImagenDesarrollarMotivacion]);
        Api::GetInterfazPanel()->GetIntAcciones()->OnClickBoton(8, &desarrollarMotivacion);    
        Api::GetInterfazPanel()->GetIntAcciones()->AsociarHotKey(8, KEY_M);
        Api::GetInterfazPanel()->GetIntAcciones()->SetTextoBoton(8, "M");
        Api::GetInterfazPanel()->GetIntAcciones()->SetTextoToolTip(8, "Desarrollar motivacion. Coste 600");        
    }
    else if ( Api::GetJugador(numJugador)->GetEstadoTecnologia(Red::Protocolo::Tecnologia::Motivacion) == Red::Protocolo::EstadoTecnologia::Desarrollado &&
              Api::GetJugador(numJugador)->GetEstadoTecnologia(Red::Protocolo::Tecnologia::MotivacionMasDistancia) == Red::Protocolo::EstadoTecnologia::NoDesarrollado )
    {
        Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(8, true);
        Api::GetInterfazPanel()->GetIntAcciones()->CambiarImagen(8, UnidadSeleccionable::imagenesBotones[UnidadSeleccionable::ImagenDesarrollarMotivacionMasDistancia]);
        Api::GetInterfazPanel()->GetIntAcciones()->OnClickBoton(8, &desarrollarMotivacionMasDistancia);    
        Api::GetInterfazPanel()->GetIntAcciones()->AsociarHotKey(8, KEY_M);
        Api::GetInterfazPanel()->GetIntAcciones()->SetTextoBoton(8, "M");
        Api::GetInterfazPanel()->GetIntAcciones()->SetTextoToolTip(8, "Desarrollar motivacion extra. Coste 600");        
    }
    else
    {   
        Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(8, false);
        Api::GetInterfazPanel()->GetIntAcciones()->OnClickBoton(8, NULL);            
    }
}

void
CuartelGeneral::Actualizar()
{
    if ( construirIngeniero )
    {
        Red::Api::GetApiLogica()->ConstruirIngeniero(handlerUnidad);        
    }
    if ( desarrollarMotivacion )
    {
        Red::Api::GetApiLogica()->DesarrollarMotivacion(handlerUnidad);
    }
    if ( desarrollarMotivacionMasDistancia )
    {
        Red::Api::GetApiLogica()->DesarrollarMotivacionMasDistancia(handlerUnidad);
    }
}

//Obtiene el tipo de unidad
int
CuartelGeneral::GetTipo()
{
    return Red::Protocolo::Unidad::CuartelGeneral;
}

void
CuartelGeneral::Inicializar()
{
    try
    {
        textura = Graficos::Api::CrearImagen("Unidades\\CuartelGeneral\\textura.bmp");
        for ( int i = 0 ; i < 8 ; i++ )
        {
            modeloHandler[i] = Graficos::Api::CrearModelo("Unidades\\CuartelGeneral\\modelo.3ds", textura, Jugador::GetColorRojo(i), Jugador::GetColorVerde(i), Jugador::GetColorAzul(i));

            Api::SetProgresoCarga( Api::GetProgresoCarga()+1.0 );
            Graficos::Api::SetPorcentajeBarraVida(Api::GetBarraProgresoCarga(), Api::GetProgresoCarga());
            Graficos::Api::Pintar();      
        }
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::CuartelGeneral::Inicializar()").Combinar(ex);
    }
}

void
CuartelGeneral::Cerrar()
{
    try
    {
        for ( int i = 0 ; i < 8 ; i++ )
        {
            Graficos::Api::DestruirModelo(modeloHandler[i]);
        }
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::CuartelGeneral::Cerrar()").Combinar(ex);
    }
}

}
