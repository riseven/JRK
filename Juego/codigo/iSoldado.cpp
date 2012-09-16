#include "iSoldado.h"
#include "gApi.h"
#include "iApi.h"
#include "rApi.h"
#include "rProtocolo.h"
#include <cstdlib>

namespace Interfaz
{

int Soldado::textura ;
int Soldado::modeloHandler[8] ;
bool Soldado::correrPulsado = false ;
int Soldado::sonidosMover[3] ;
int Soldado::sonidosAtacar[5] ;

//Creo una unidad Soldado Raso a partir de su modelo
Soldado::Soldado(int numJugador):
    UnidadAgresiva(numJugador)
{
    /*
    if ( modeloHandler == -1 )
    {
        modeloHandler = Graficos::Api::CrearModelo("Unidades\\SoldadoRaso\\Jeep.3ds");
        //modeloHandler = Graficos::Api::CrearModelo("");
        
        //modeloHandler = Graficos::Api::CrearModelo("Unidades\\SoldadoRaso\\Ma1.3ds");        
    }
    */
    int color = Api::GetJugador(numJugador)->GetColor();
    objetoHandler = Graficos::Api::CrearObjeto(modeloHandler[color] );
    Graficos::Api::SetObjetoSeleccionable(objetoHandler, true);
    Graficos::Api::SetObjetoBloqueante(objetoHandler, true);
    
    float radio = Graficos::Api::GetRadioObjeto(objetoHandler);
    Graficos::Api::SetEscalaObjeto(handlerCirculoSeleccion, radio*1.25);  
    
    corriendo = false ;
    
    for ( int i = 0 ; i < 3 ; i++ )
    {
        unidadSonidosMover[i] = sonidosMover[i] ;
    }
    numSonidosMover = 3 ;   
    for ( int i = 0 ; i < 5 ; i++ )
    {
        unidadSonidosAtacar[i] = sonidosAtacar[i] ;
    }
    numSonidosAtacar = 5 ;    
 
}

//Destruyo la unidad
Soldado::~Soldado()
{    
}

//Selecciona la unidad y muestra las acciones disponibles para ésta
void
Soldado::Seleccionar()
{
    UnidadAgresiva::Seleccionar();
}    

 
//Actualiza el estado de la unidad dependiendo de la accion seleccionada
void
Soldado::Actualizar()
{
    if ( correrPulsado )
    {        
        if ( corriendo )
        {
            corriendo = false ;
        }
        else
        {
            corriendo = true ;
        }
        Api::GetInterfazPanel()->GetIntAcciones()->SetFijoBoton(8, corriendo);
        
        // Se actualiza el estado de todos los soldados seleccionados
        list<UnidadMultiSeleccionable *> unis = UnidadMultiSeleccionable::GetUnidadesSeleccionadas();
        for ( list<UnidadMultiSeleccionable *>::iterator i = unis.begin() ; i != unis.end() ; ++i )
        {
            ((Soldado *)(*i))->corriendo = corriendo ;
        }
    }
}

//Obtiene el tipo de unidad
int
Soldado::GetTipo()
{
    return Red::Protocolo::Unidad::Soldado ;
}

void
Soldado::PintarBotonesUnidad()
{
    Api::GetInterfazPanel()->GetIntAcciones()->SetVisible(8, true);
    Api::GetInterfazPanel()->GetIntAcciones()->CambiarImagen(8, UnidadSeleccionable::imagenesBotones[ UnidadSeleccionable::ImagenCorrerSoldado ]);  
    Api::GetInterfazPanel()->GetIntAcciones()->OnClickBoton(8, &correrPulsado);
    Api::GetInterfazPanel()->GetIntAcciones()->SetFijoBoton(8, corriendo);
    Api::GetInterfazPanel()->GetIntAcciones()->AsociarHotKey(8, KEY_C);
    Api::GetInterfazPanel()->GetIntAcciones()->SetTextoBoton(8, "C");
    Api::GetInterfazPanel()->GetIntAcciones()->SetTextoToolTip(8, "Correr");
}

void
Soldado::Inicializar()
{ 
    textura = Graficos::Api::CrearImagen("Unidades\\SoldadoRaso\\textura.bmp") ;
    for ( int i = 0 ; i < 8 ; i++ )
    {
        modeloHandler[i] = Graficos::Api::CrearModelo("Unidades\\SoldadoRaso\\modelo.3ds", textura, Jugador::GetColorRojo(i), Jugador::GetColorVerde(i), Jugador::GetColorAzul(i));
        
        Api::SetProgresoCarga( Api::GetProgresoCarga()+1.0 );
        Graficos::Api::SetPorcentajeBarraVida(Api::GetBarraProgresoCarga(), Api::GetProgresoCarga());
        Graficos::Api::Pintar();              
    }
    sonidosMover[0] = Graficos::Api::CrearSonido("Sonidos\\ahoramismo.wav");
    sonidosMover[1] = Graficos::Api::CrearSonido("Sonidos\\enseguida.wav");
    sonidosMover[2] = Graficos::Api::CrearSonido("Sonidos\\yavoy2.wav");     
    
    sonidosAtacar[0] = Graficos::Api::CrearSonido("Sonidos\\alataque.wav");
    sonidosAtacar[1] = Graficos::Api::CrearSonido("Sonidos\\ataque1.wav");
    sonidosAtacar[2] = Graficos::Api::CrearSonido("Sonidos\\ataque2.wav");
    sonidosAtacar[3] = Graficos::Api::CrearSonido("Sonidos\\eslahoradelastortas.wav");
    sonidosAtacar[4] = Graficos::Api::CrearSonido("Sonidos\\nohuyas.wav");                
}

void
Soldado::Reset()
{
    correrPulsado = false ;
}

}
