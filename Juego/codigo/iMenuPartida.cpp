#include "iMenuPartida.h"
#include "gApi.h"
#include "rApi.h"
#include "iApi.h"
#include "iConfiguracion.h"
#include "iJuego.h"
#include "eExcepcionLocalizada.h"
#include "eExcepcionParametro.h"

namespace Interfaz
{
    
MenuPartida::MenuPartida(Menu *menuPadre):
    Menu(menuPadre, "Partida", "", "", "", "Empezar partida", "Volver")
{    
    try
    {
        for (int i = 0 ; i < 16 ; i++ )
        {
            imagenNombre[i] = -1 ;
            spriteNombre[i] = -1 ;
            rectanguloColor[i] = -1 ;
        }
        
        // Mostramos el chatbox
        imagenChatBox = Graficos::Api::CrearImagen("Graficos\\ChatBox.bmp");
        chatBox = Graficos::Api::CrearChatBox(imagenChatBox, 0, 600, 200, 10, 350, 450, 1);
        
        // Mostramos el textbox del chat
        imagenTextBox = Graficos::Api::CrearImagen("Graficos\\TextBoxChat.bmp");
        textBox = Graficos::Api::CrearTextBox(imagenTextBox, 0, "", 350, 660, 1);
        Graficos::Api::OnEnterTextBox(textBox, &enterTextBox);
        
        // Mostramos el boton de cambio de color
        imagenBotonColor = Graficos::Api::CrearImagen("Graficos\\BotonColor.bmp");
        imagenBotonColorSeleccion = Graficos::Api::CrearImagen("Graficos\\BotonColorSeleccion.bmp");
        botonColor = Graficos::Api::CrearBoton(imagenBotonColor, imagenBotonColorSeleccion, 0, "", 295, 135, 1);
        Graficos::Api::OnClickBoton(botonColor, &clickBotonColor);
        
        // Nos conectamos
        Api::SetMenuPartida(this);
        Red::Api::GetApiLogica()->UnirsePartida( 0, Configuracion::GetNombreJugador() );
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::MenuPartida::MenuPartida(Menu *)").Combinar(ex);
    }
}

MenuPartida::~MenuPartida()
{
    try
    {
        for ( int i = 0 ; i < 16 ; i++ )
        {
            if ( imagenNombre[i] != -1 )
            {
                Graficos::Api::DestruirImagen(imagenNombre[i]);
            }
            if ( spriteNombre[i] != -1 )
            {
                Graficos::Api::DestruirSprite(spriteNombre[i]);
            }
            if ( rectanguloColor[i] != -1 )
            {
                Graficos::Api::DestruirRectangulo(rectanguloColor[i]);
            }
        }
        
        Graficos::Api::DestruirImagen(imagenChatBox);
        Graficos::Api::DestruirChatBox(chatBox); 
        
        Graficos::Api::DestruirImagen(imagenTextBox);
        Graficos::Api::DestruirTextBox(textBox);
        
        Graficos::Api::DestruirImagen(imagenBotonColor);
        Graficos::Api::DestruirImagen(imagenBotonColorSeleccion);
        Graficos::Api::DestruirBoton(botonColor);     
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::MenuPartida::~MenuPartida()").Combinar(ex);
    }
}

void
MenuPartida::Actualizar()
{
    if ( enterTextBox )
    {
        string texto = Graficos::Api::GetTextoTextBox(textBox);
        if ( texto != "" )
        {
            // Enviar el mensaje
            Red::Api::GetApiLogica()->MensajeChatMenu(0, texto);
        }
        Graficos::Api::SetTextoTextBox(textBox, "");
    }
    
    if ( clickBotonColor )
    {
        Red::Api::GetApiLogica()->CambiarColor(0);
    }
    
    // Actualizamos el color de los jugadores
    for ( int i = 0 ; i < 16 ; i++ )
    {
        if ( rectanguloColor[i] != -1 )
        {
            int color = Api::GetJugador(i)->GetColor() ;
            Graficos::Api::SetColorRectangulo(rectanguloColor[i],
                                              Jugador::GetColorRojo(color),
                                              Jugador::GetColorVerde(color),
                                              Jugador::GetColorAzul(color),
                                              1.0);
        }
    }
    
    // Actualizo la posicion de los botones
    Graficos::Api::MoverBoton(botonColor, 295, 135+ Jugador::GetJugadorPropio()*50);
    
    Menu::Actualizar();    
}

void
MenuPartida::Continuar()
{
    try
    {
        if ( menuHijo == NULL || menuPadre == NULL)
        {
            throw Error::ExcepcionLocalizada("Interfaz::MenuPartida::Continuar()");
        }
        delete menuHijo ;
        menuHijo = NULL ;
        menuPadre->Continuar();
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::MenuPartida::Continuar()").Combinar(ex);
    }
}

void
MenuPartida::Activar()
{
    try
    {
        Menu::Activar();
        for ( int i = 0 ; i < 16 ; i++ )
        {
            if ( spriteNombre[i] != -1 )
            {
                Graficos::Api::ActivarSprite(spriteNombre[i]);
            }
            if ( rectanguloColor[i] != -1 )
            {
                Graficos::Api::ActivarRectangulo(rectanguloColor[i]);
            }
        }
        Graficos::Api::ActivarBoton(botonColor);
        Graficos::Api::ActivarChatBox(chatBox);
        Graficos::Api::ActivarTextBox(textBox);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::MenuPartida::Activar()").Combinar(ex);
    }
}

void
MenuPartida::Desactivar()
{
    try
    {
        Menu::Desactivar();
        for ( int i = 0 ; i < 16 ; i++ )
        {
            if ( spriteNombre[i] != -1 )
            {
                Graficos::Api::DesactivarSprite(spriteNombre[i]);
            }
            if ( rectanguloColor[i] != -1 )
            {
                Graficos::Api::DesactivarRectangulo(rectanguloColor[i]);
            }
        }
        Graficos::Api::DesactivarBoton(botonColor);
        Graficos::Api::DesactivarChatBox(chatBox);
        Graficos::Api::DesactivarTextBox(textBox);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::MenuPartida::Desactivar()").Combinar(ex);
    }
}


void
MenuPartida::ClickBoton4()
{
    try
    {
        Red::Api::GetApiLogica()->EmpezarPartida();
        /*
        Menu::ClickBoton4();
        menuHijo = new Juego(this);
        Api::SetMenuActual(menuHijo);
        Desactivar();
        */
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::MenuPartida::ClickBoton4()").Combinar(ex);
    }
}

void
MenuPartida::ClickBoton5()
{
    try
    {
        Menu::ClickBoton5();        
        menuPadre->Continuar();
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::MenuPartida::ClickBoton5()").Combinar(ex);
    }
} 

void
MenuPartida::NuevoJugador(int numJugador, string nombreJugador)
{
    try
    {
        imagenNombre[numJugador] = Graficos::Api::CrearImagen(0, nombreJugador);
        spriteNombre[numJugador] = Graficos::Api::CrearSprite(imagenNombre[numJugador], 100, 150+numJugador*50, 1);    
        int color = Api::GetJugador(numJugador)->GetColor() ;
        rectanguloColor[numJugador] = Graficos::Api::CrearRectangulo(30, 20, 
                                                                     Jugador::GetColorRojo(color)*255.0, 
                                                                     Jugador::GetColorVerde(color)*255.0, 
                                                                     Jugador::GetColorAzul(color)*255.0,
                                                                     255,
                                                                     true,
                                                                     300, 150+numJugador*50-10, 2);

        // Notificamos en el chatbox el nuevo jugador
        string texto = "" ;
        texto += nombreJugador ;
        texto += " se ha unido a la partida." ;
        Graficos::Api::AddLineaChatBox(chatBox, texto, 255, 255, 255);        
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::MenuPartida::NuevoJugador(int, string)").Combinar(ex);
    }
}    

void
MenuPartida::AbandonoJugador(int numJugador)
{
    if ( imagenNombre[numJugador] != -1 )
    {
        Graficos::Api::DestruirImagen( imagenNombre[numJugador] );
        imagenNombre[numJugador] = -1 ;
    }
    
    if ( spriteNombre[numJugador] != -1 )
    {
        Graficos::Api::DestruirSprite( spriteNombre[numJugador] );
        spriteNombre[numJugador] = -1 ;
    }
    
    if ( rectanguloColor[numJugador] != -1 )
    {
        Graficos::Api::DestruirRectangulo( rectanguloColor[numJugador] );
        rectanguloColor[numJugador] = -1 ;
    }
    
    string texto = "" ;
    texto += Api::GetJugador(numJugador)->GetNombre() ;
    texto += " ha abandonado la partida." ;
    Graficos::Api::AddLineaChatBox(chatBox, texto, 255, 255, 255);
}

void
MenuPartida::MensajeChat(int numJugador, string texto)
{
    string textoLinea = Api::GetJugador(numJugador)->GetNombre() ;
    textoLinea += ": " ;
    textoLinea += texto ;
    int color = Api::GetJugador(numJugador)->GetColor() ;
    Graficos::Api::AddLineaChatBox(chatBox, textoLinea, 
                                   Jugador::GetColorRojo(color)*255.0, 
                                   Jugador::GetColorVerde(color)*255.0, 
                                   Jugador::GetColorAzul(color)*255.0);
}

void
MenuPartida::EmpezarPartida()
{
    menuHijo = new Juego(this);
    Api::SetMenuActual(menuHijo);
    Desactivar();
}

}

