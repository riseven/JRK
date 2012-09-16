#include "iBarracones.h"
#include "gApi.h"
#include "iApi.h"
#include "eExcepcionLocalizada.h"
#include "rProtocolo.h"
#include "rApi.h"



namespace Interfaz
{

int Barracones::textura ;
int Barracones::modeloHandler[8] ;

//Crea el cuartel general a partir de su modelo y lo marca como seleccionable
Barracones::Barracones(int numJugador):
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
        
        construirSoldado = false ;
        desarrollarArmaduraSoldado = false ;
        desarrollarVelocidadDisparoSoldado = false ;
        
        porcentajeConstruido = -1 ;
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::Barracones::Barracones()").Combinar(ex);
    }
}

//Destruye el modelo del cuartel general
Barracones::~Barracones()
{
    try
    {   
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::Barracones::~Barracones()").Combinar(ex);
    }
}

void
Barracones::SetPorcentajeConstruido(int porcentaje)
{
    if ( EstaSeleccionada() )
    {
        if ( porcentajeConstruido == -1 && porcentaje != -1)
        {
            Api::GetInterfazPanel()->GetIntSeleccion()->SetVisibleBarraProgreso(true);
        
            Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(4, false);
            Api::GetInterfazPanel()->GetIntAcciones()->OnClickBoton(4, NULL);
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
Barracones::Seleccionar()
{
    UnidadUniSeleccionable::Seleccionar();
    
    for ( int i = 0 ; i < 12 ; i++ )
    {
        Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(i, false);
    }

    if ( porcentajeConstruido == -1 )
    {
        Api::GetInterfazPanel()->GetIntSeleccion()->SetVisibleBarraProgreso(false);
    }
    else
    {
        Api::GetInterfazPanel()->GetIntSeleccion()->SetVisibleBarraProgreso(true);
        Api::GetInterfazPanel()->GetIntSeleccion()->SetPorcentajeBarraProgreso(porcentajeConstruido);        
    }    

    Api::GetInterfazPanel()->GetIntAcciones()->CambiarImagen(0, UnidadSeleccionable::imagenesBotones[UnidadSeleccionable::ImagenConstruirSoldado]);
    Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(0, true);
    Api::GetInterfazPanel()->GetIntAcciones()->OnClickBoton(0, &construirSoldado);
    Api::GetInterfazPanel()->GetIntAcciones()->AsociarHotKey(0, KEY_S);   
    Api::GetInterfazPanel()->GetIntAcciones()->SetTextoBoton(0, "S");
    Api::GetInterfazPanel()->GetIntAcciones()->SetTextoToolTip(0, "Soldado. Coste 100");     
    
    MostrarBotonesTecnologia();    
}    

void
Barracones::MostrarBotonesTecnologia()
{
    if ( Api::GetJugador(numJugador)->GetEstadoTecnologia( Red::Protocolo::Tecnologia::ArmaduraSoldado ) == Red::Protocolo::EstadoTecnologia::NoDesarrollado )
    {
        Api::GetInterfazPanel()->GetIntAcciones()->CambiarImagen(4, UnidadSeleccionable::imagenesBotones[UnidadSeleccionable::ImagenDesarrollarArmaduraSoldado]);
        Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(4, true);
        Api::GetInterfazPanel()->GetIntAcciones()->OnClickBoton(4, &desarrollarArmaduraSoldado);
        Api::GetInterfazPanel()->GetIntAcciones()->AsociarHotKey(4, KEY_A);
        Api::GetInterfazPanel()->GetIntAcciones()->SetTextoBoton(4, "A");
        Api::GetInterfazPanel()->GetIntAcciones()->SetTextoToolTip(4, "Desarrollar armadura mejorada del soldado. Coste 400");        
    }
    else
    {
        Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(4, false);
        Api::GetInterfazPanel()->GetIntAcciones()->OnClickBoton(4, NULL);
    }
    
    if ( Api::GetJugador(numJugador)->GetEstadoTecnologia( Red::Protocolo::Tecnologia::VelocidadDisparoSoldado ) == Red::Protocolo::EstadoTecnologia::NoDesarrollado )
    {
        Api::GetInterfazPanel()->GetIntAcciones()->CambiarImagen(8, UnidadSeleccionable::imagenesBotones[UnidadSeleccionable::ImagenDesarrollarVelocidadDisparoSoldado]);
        Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(8, true);
        Api::GetInterfazPanel()->GetIntAcciones()->OnClickBoton(8, &desarrollarVelocidadDisparoSoldado);
        Api::GetInterfazPanel()->GetIntAcciones()->AsociarHotKey(8, KEY_V);        
        Api::GetInterfazPanel()->GetIntAcciones()->SetTextoBoton(8, "V");
        Api::GetInterfazPanel()->GetIntAcciones()->SetTextoToolTip(8, "Desarrollar mayor velocidad de disparo del soldado. Coste 400");        
    }
    else
    {
        Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(8, false);
        Api::GetInterfazPanel()->GetIntAcciones()->OnClickBoton(8, NULL);
    }
}    

void
Barracones::Actualizar()
{
    
    if ( construirSoldado )
    {
        Red::Api::GetApiLogica()->ConstruirSoldado(handlerUnidad);
    }
    if ( desarrollarArmaduraSoldado )
    {
        Red::Api::GetApiLogica()->DesarrollarArmaduraSoldado(handlerUnidad);
    }
    if ( desarrollarVelocidadDisparoSoldado )
    {
        Red::Api::GetApiLogica()->DesarrollarVelocidadDisparoSoldado(handlerUnidad);
    }
    
}

//Obtiene el tipo de unidad
int
Barracones::GetTipo()
{
    return Red::Protocolo::Unidad::Barracones ;
}

void
Barracones::Inicializar()
{
    try
    {
        textura = Graficos::Api::CrearImagen("Unidades\\Barracones\\textura.bmp");
        for ( int i = 0 ; i < 8 ; i++ )
        {
            modeloHandler[i] = Graficos::Api::CrearModelo("Unidades\\Barracones\\modelo.3ds", textura, Jugador::GetColorRojo(i), Jugador::GetColorVerde(i), Jugador::GetColorAzul(i) );

            Api::SetProgresoCarga( Api::GetProgresoCarga()+1.0 );
            Graficos::Api::SetPorcentajeBarraVida(Api::GetBarraProgresoCarga(), Api::GetProgresoCarga());
            Graficos::Api::Pintar();              
        }      
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::Barracones::Inicializar()").Combinar(ex);
    }
}

void
Barracones::Cerrar()
{
    try
    {
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::Barracones::Cerrar()").Combinar(ex);
    }
}

}
