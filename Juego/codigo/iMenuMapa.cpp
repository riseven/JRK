#include <allegro.h>
#include <alleggl.h>
#include <stdexcept>
#include "iMenuMapa.h"
#include "iMenuPartida.h"
#include "gApi.h"
#include "iApi.h"
#include "lApi.h"
#include "rApi.h"
#include "eExcepcionFichero.h"
#include "eExcepcionParametro.h"
#include "eExcepcionLocalizada.h"



namespace Interfaz
{

MenuMapa::MenuMapa(Menu *menuPadre):
    Menu(menuPadre, "Seleccionar mapa", "", "", "Partida local", "Partida multijugador", "Volver")
{
    try
    {
        // Leemos la informacion de los mapas
        InfoMapa *temp;
        temp = new InfoMapa("MapaDePrueba.map") ;
        if ( temp->Correcto() )
        {
            infoMapas.push_back( temp );
        }
        temp = new InfoMapa("ElOtro.map") ;    
        if ( temp->Correcto() )
        {
            infoMapas.push_back( temp );
        }
        
        // Creo el listbox de mapas
        imagenFondoListBox =        Graficos::Api::CrearImagen("Graficos\\FondoListBox.bmp");
        imagenSeleccionListBox =    Graficos::Api::CrearImagen("Graficos\\SeleccionListBox.bmp");
        imagenResaltadoListBox =    Graficos::Api::CrearImagen("Graficos\\ResaltadoListBox.bmp");
        Graficos::Api::SetImagenMasked(imagenFondoListBox, true);
        handlerListBox = Graficos::Api::CrearListBox(
            imagenFondoListBox, imagenSeleccionListBox, imagenResaltadoListBox,
            0, 0, 0,
            4, 13, 487,  
            0, 0, 0,
            500, 105, 1 );
            
        // Añadimos las lineas del listbox
        for ( vector <InfoMapa *>::iterator i = infoMapas.begin() ; i != infoMapas.end() ; ++i )
        {
            Graficos::Api::AddLineaListBox(handlerListBox, (*i)->GetNombre() + " (" +  ((char) ( (*i)->GetMaxJugadores() + '0' )) +")" );
        }
        
        // Añadimos el evento de cambio de listbox
        Graficos::Api::OnCambioListBox(handlerListBox, &cambioListBox, &seleccionListBox);
        cambioListBox = false ;
        seleccionListBox = 0 ;
        
        // Mostramos el fondo de la imagen de preview
        imagenFondoPreview = Graficos::Api::CrearImagen( "Graficos\\FondoPreview.bmp" );
        spriteFondoPreview = Graficos::Api::CrearSprite( imagenFondoPreview, 95, 100, 1 );
        
        // Mostramos el primer mapa
        if ( infoMapas.size() > 0 )
        {
            imagenPreview = -1 ;
            spritePreview = -1 ;
            try
            {
                imagenPreview = Graficos::Api::CrearImagen( infoMapas.at(seleccionListBox)->GetFicheroPreview() );
                spritePreview = Graficos::Api::CrearSprite( imagenPreview, 100, 105, 2 ) ;
            }
            catch ( Error::ExcepcionFichero &ex )
            {
            }
        }
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::MenuMapa::MenuMapa(Menu *)");
    }
    catch ( Error::Excepcion &ex)
    {
        throw Error::ExcepcionLocalizada("Interfaz::MenuMapa::MenuMapa(Menu *)").Combinar(ex);
    }
}

MenuMapa::~MenuMapa()
{
    try
    {
        Graficos::Api::DestruirListBox(handlerListBox);
        Graficos::Api::DestruirImagen(imagenFondoListBox);
        Graficos::Api::DestruirImagen(imagenSeleccionListBox);
        Graficos::Api::DestruirImagen(imagenResaltadoListBox);
        
        for ( vector <InfoMapa *>::iterator i = infoMapas.begin() ; i != infoMapas.end() ; ++i )
        {
            if ( *i )
            {
                delete (*i) ;
                *i = NULL ;
            }
        }
        infoMapas.clear();
        
        Graficos::Api::DestruirSprite(spritePreview);
        Graficos::Api::DestruirImagen(imagenPreview);
        Graficos::Api::DestruirSprite(spriteFondoPreview);
        Graficos::Api::DestruirImagen(imagenFondoPreview);
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::MenuMapa::~MenuMapa()");
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::MenuMapa::~MenuMapa()").Combinar(ex);
    }   
}

void
MenuMapa::Actualizar()
{
    try
    {
        // Comprobamos si se ha cambiado el mapa seleccionado en el listbox
        if ( cambioListBox )
        {
            if ( spritePreview != -1 )
            {
                Graficos::Api::DestruirSprite(spritePreview);
            }
            if ( imagenPreview != -1 )
            {
                Graficos::Api::DestruirImagen(imagenPreview);
            }
            try
            {
                imagenPreview = -1 ;
                spritePreview = -1 ;
                imagenPreview = Graficos::Api::CrearImagen( infoMapas.at(seleccionListBox)->GetFicheroPreview() );
                spritePreview = Graficos::Api::CrearSprite( imagenPreview, 100, 105, 2 ) ;                
            }
            catch ( Error::ExcepcionFichero &ex )
            {
            }
        }
        Menu::Actualizar();
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::MenuMapa::Actualizar()").Combinar(ex);
    }
}

void
MenuMapa::Continuar()
{
    try
    {
        if ( !menuHijo || !menuPadre)
        {
            throw Error::ExcepcionLocalizada("Interfaz::MenuMapa::Continuar()");
        }
        
        Logica::Api::Finalizar();
        Red::Api::Finalizar();
        
        delete menuHijo ;
        menuHijo = NULL ;
        menuPadre->Continuar();        
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::MenuMapa::Continuar()").Combinar(ex);
    }
}

void
MenuMapa::Activar()
{
    try
    {
        Menu::Activar();
        Graficos::Api::ActivarListBox(handlerListBox);
        Graficos::Api::ActivarSprite(spritePreview);
        Graficos::Api::ActivarSprite(spriteFondoPreview);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::MenuMapa::Activar()").Combinar(ex);
    }
}

void
MenuMapa::Desactivar()
{
    try
    {
        Menu::Desactivar();
        Graficos::Api::DesactivarListBox(handlerListBox);
        Graficos::Api::DesactivarSprite(spritePreview);
        Graficos::Api::DesactivarSprite(spriteFondoPreview);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::MenuMapa::Desactivar()").Combinar(ex);
    }
}
    

void
MenuMapa::ClickBoton3()
{
    try
    {
        // Inicializamos la red
        Interfaz::Api::SetMapa(infoMapas.at(seleccionListBox)->GetNombreFichero());
        Red::Api::Inicializar( Red::Api::MODO_LOCAL );
        Logica::Api::Inicializar();
        
        Menu::ClickBoton4();
        menuHijo = new MenuPartida(this);
        Api::SetMenuActual(menuHijo);
        Desactivar();
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::MenuMapa::ClickBoton4()").Combinar(ex);
    }
}

void
MenuMapa::ClickBoton4()
{
    try
    {
        // Inicializamos la red
        Interfaz::Api::SetMapa(infoMapas.at(seleccionListBox)->GetNombreFichero());
        Red::Api::Inicializar( Red::Api::MODO_SERVIDOR );
        Logica::Api::Inicializar();
        
        Menu::ClickBoton4();
        menuHijo = new MenuPartida(this);
        Api::SetMenuActual(menuHijo);
        Desactivar();
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::MenuMapa::ClickBoton4()").Combinar(ex);
    }
}

void
MenuMapa::ClickBoton5()
{
    try
    {
        Menu::ClickBoton5();
        menuPadre->Continuar();
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::MenuMapa::ClickBoton5()").Combinar(ex);
    }    
}

}
