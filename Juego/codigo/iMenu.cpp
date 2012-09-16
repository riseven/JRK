#include "iMenu.h"
#include "gApi.h"
#include "eExcepcion.h"
#include "eExcepcionLocalizada.h"
#include "eExcepcionParametro.h"
#include "eExcepcionFichero.h"


namespace Interfaz
{

int Menu::handlerImagenBoton[5];
int Menu::handlerImagenSeleccionBoton[5];
int Menu::posBoton[5][2];
int Menu::sonidoClick = -1 ;
int Menu::musica = -1 ;
int Menu::imagenFondoMenu = -1 ;

// Construye un menu de 5 botones pasandole como parametros el 
// menu padre, el titulo del menu, y el texto de cada uno de los botones
Menu::Menu(Menu *menuPadre, string textoTitulo, string texto1, string texto2, string texto3, string texto4, string texto5):
    textoTitulo(textoTitulo), menuPadre(menuPadre)
{
    try
    {
        imagenTitulo = -1 ;
        spriteTitulo = -1 ; 
        
        //Creo un sprite con el titulo del menu
        if ( textoTitulo != "" )
        {
            imagenTitulo = Graficos::Api::CrearImagen(0, textoTitulo) ;
            spriteTitulo = Graficos::Api::CrearSprite(imagenTitulo, 50, 50, 1) ;
        }
        
        textoBoton[0] = texto1 ;
        textoBoton[1] = texto2 ;
        textoBoton[2] = texto3 ;
        textoBoton[3] = texto4 ;
        textoBoton[4] = texto5 ; 
        
        //Creo los botones con el texto especificado
        for ( int i = 0 ; i < 5 ; i++ )
        {
            handlerBoton[i] = -1 ;
            clickBoton[i] = false ;
            if ( textoBoton[i] != "" )
            {
                handlerBoton[i] = Graficos::Api::CrearBoton(handlerImagenBoton[i], handlerImagenSeleccionBoton[i], 0, textoBoton[i], posBoton[i][0], posBoton[i][1], 1);
                Graficos::Api::OnClickBoton(handlerBoton[i], &clickBoton[i]);
            }
        }
        
        spriteFondoMenu = Graficos::Api::CrearSprite(imagenFondoMenu, 0, 0, 0);
        Graficos::Api::SetAlphaSprite(spriteFondoMenu, 64);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::Menu::Menu(string)").Combinar(ex);
    }
}

//Destruye el menu
Menu::~Menu()
{
    try
    {
        //Destruto sprite titulo
        if ( imagenTitulo != -1 )
        {
            Graficos::Api::DestruirImagen(imagenTitulo) ;
        }
        if ( spriteTitulo != -1 )
        {
            Graficos::Api::DestruirSprite(spriteTitulo) ;
        }
        //Destruyo botones creados
        for ( int i = 0 ; i < 5 ; i++ )
        {
            if ( handlerBoton[i] != -1 )
            {
                Graficos::Api::DestruirBoton(handlerBoton[i]);
            }
        }
        
        Graficos::Api::DestruirSprite(spriteFondoMenu);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::Menu::~Menu()").Combinar(ex);
    }
}

void
Menu::Inicializar()
{
    try
    {   
        //Creo imagenes botones
        handlerImagenBoton[0] = Graficos::Api::CrearImagen("Graficos\\BotonPrincipal1.bmp");
        handlerImagenBoton[1] = Graficos::Api::CrearImagen("Graficos\\BotonPrincipal2.bmp");
        handlerImagenBoton[2] = Graficos::Api::CrearImagen("Graficos\\BotonPrincipal3.bmp");
        handlerImagenBoton[3] = Graficos::Api::CrearImagen("Graficos\\BotonPrincipal4.bmp");
        handlerImagenBoton[4] = Graficos::Api::CrearImagen("Graficos\\BotonPrincipal5.bmp");
        
        //Creo imagenes seleccion botones
        handlerImagenSeleccionBoton[0] = Graficos::Api::CrearImagen("Graficos\\BotonPrincipalSel1.bmp");
        handlerImagenSeleccionBoton[1] = Graficos::Api::CrearImagen("Graficos\\BotonPrincipalSel2.bmp");
        handlerImagenSeleccionBoton[2] = Graficos::Api::CrearImagen("Graficos\\BotonPrincipalSel3.bmp");
        handlerImagenSeleccionBoton[3] = Graficos::Api::CrearImagen("Graficos\\BotonPrincipalSel4.bmp");
        handlerImagenSeleccionBoton[4] = Graficos::Api::CrearImagen("Graficos\\BotonPrincipalSel5.bmp");                                
        
        //Aplico mascara transparencia a las imagenes de los botones
        for ( int i = 0 ; i < 5 ; i++ )
        {
            Graficos::Api::SetImagenMasked( handlerImagenBoton[i], true );
            Graficos::Api::SetImagenMasked( handlerImagenSeleccionBoton[i], true );
        }
        
        //Posicion de cada boton en pantalla
        posBoton[0][0] = 81 ;
        posBoton[0][1] = 176 ;
        posBoton[1][0] = 112 ;
        posBoton[1][1] = 270 ;
        posBoton[2][0] = 113;
        posBoton[2][1] = 363 ;
        posBoton[3][0] = 93 ;
        posBoton[3][1] = 457 ;
        posBoton[4][0] = 55 ;
        posBoton[4][1] = 551 ;
        
        sonidoClick = Graficos::Api::CrearSonido("Sonidos\\Click.wav");
        
        musica = Graficos::Api::CrearSonido("Musica\\Track6.wav");
        Graficos::Api::ReproducirLoopSonido(musica);
        
        imagenFondoMenu = Graficos::Api::CrearImagen("Graficos\\Presentacion.bmp");
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::Menu::Inicializar()").Combinar(ex);
    }
}


//Comprueba si se ha hecho click sobre alguno de los botones
//del menu ejecutando la accion apropiada
void
Menu::Actualizar()
{
    if ( clickBoton[0] )
    {
        ClickBoton1();
        return ;
    }
    if ( clickBoton[1] )
    {
        ClickBoton2();
        return ;
    }
    if ( clickBoton[2] )
    {
        ClickBoton3();
        return ;
    }
    if ( clickBoton[3] )
    {
        ClickBoton4();
        return ;
    }
    if ( clickBoton[4] )
    {
        ClickBoton5();
        return ;
    }
}

void
Menu::Continuar()
{
}

//Activa los botones del menu
void
Menu::Activar()
{
    try
    {
        Graficos::Api::ActivarSprite(spriteTitulo) ;
        for ( int i = 0 ; i < 5 ; i++ )
        {
            if ( handlerBoton[i] != -1 )
            {
                Graficos::Api::ActivarBoton(handlerBoton[i]);
            }
        }
        Graficos::Api::ActivarSprite(spriteFondoMenu);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::Menu::Activar()").Combinar(ex);
    }
}

//Desactiva los botones del menu
void
Menu::Desactivar()
{
    try
    {
        Graficos::Api::DesactivarSprite(spriteTitulo) ;
        for ( int i = 0 ; i < 5 ; i++ )
        {
            if ( handlerBoton[i] != -1 )
            {
                Graficos::Api::DesactivarBoton(handlerBoton[i]);
            }
        }
        Graficos::Api::DesactivarSprite(spriteFondoMenu);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::Menu::Desactivar()").Combinar(ex);
    }
}


//Guarda el boton pulsado

void
Menu::ClickBoton1()
{
    botonPulsado = 1 ;
    Graficos::Api::ReproducirSonido(sonidoClick);
}

void
Menu::ClickBoton2() 
{
    botonPulsado = 2 ;
    Graficos::Api::ReproducirSonido(sonidoClick);    
}

void
Menu::ClickBoton3()
{
    botonPulsado = 3 ;
    Graficos::Api::ReproducirSonido(sonidoClick);    
}

void
Menu::ClickBoton4()
{
    botonPulsado = 4 ;
    Graficos::Api::ReproducirSonido(sonidoClick);
}

void
Menu::ClickBoton5()
{
    botonPulsado = 5 ;
    Graficos::Api::ReproducirSonido(sonidoClick);    
}

}
