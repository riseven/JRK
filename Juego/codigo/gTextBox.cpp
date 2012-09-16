#include <allegro.h>

#include "gTextBox.h"
#include "eExcepcionLocalizada.h"

namespace Graficos
{

TextBox::TextBox( Imagen *Fondo, Fuente *fuente, string Texto, int X, int Y, int Z ):
    Entidad2D(X, Y, Z)
{
    imagenFondo = Fondo ;
    fuenteTexto = fuente ;
    texto = Texto ; 
    relY = ( Fondo->GetAlto() - text_height(fuente->GetFont()) )/2 ;
    estado = 0;
    imagenTexto = new Imagen(fuente, Texto) ;
    onEnter = NULL ;
}

TextBox::~TextBox()
{
    try
    {
        if ( imagenTexto )
        {
            delete imagenTexto ;
            imagenTexto = NULL ;
        }
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::TextBox::~TextBox()").Combinar(ex);
    }
}

void
TextBox::OnEnter(bool *flag)
{
    onEnter = flag ;
}

void
TextBox::Interactuar()
{
    try
    {
        switch (estado)
        {
        case 0: // No seleccionado, No overmouse
            if ( mouse_x >= x && mouse_x < x + imagenFondo->GetAncho() &&
                 mouse_y >= y && mouse_y < y + imagenFondo->GetAlto() )
            {
                if ( mouse_b & 1 ) // Boton izquierdo del raton pulsado
                {
                    estado = 2 ;
                }
                else
                {
                    estado = 1 ;
                }
                
                // Generamos un texto con el simbolo de edicion
                delete imagenTexto ;
                imagenTexto = new Imagen(fuenteTexto, texto + "_");
            }
            break ;
        case 1: // No seleccionado, overmouse
            if ( mouse_x >= x && mouse_x < x + imagenFondo->GetAncho() &&
                 mouse_y >= y && mouse_y < y + imagenFondo->GetAlto() )
            {
                if ( mouse_b & 1 ) // Boton izquierdo del raton pulsado
                {
                    estado = 2 ;
                }
            }
            else
            {
                estado = 0 ;
                
                // Quitamos el simbolo de edicion
                delete imagenTexto ;
                imagenTexto = new Imagen(fuenteTexto, texto);
            }
            break ;
        case 2: // Seleccionado
            // Si el raton está fuera del control, y se pulsa cualquier boton
            if ( !( mouse_x >= x && mouse_x < x + imagenFondo->GetAncho() &&
                 mouse_y >= y && mouse_y < y + imagenFondo->GetAlto() ) && mouse_b )
            {
                estado = 0 ;
                delete imagenTexto ;
                imagenTexto = new Imagen(fuenteTexto, texto);
            }
            else
            {
                bool stringCambiada = false ;
                if ( onEnter )
                {
                    *onEnter = false ;
                }
                
                // Leemos el teclado
                while ( keypressed() )
                {
                    stringCambiada = true ;
                    int temp = readkey() ;

                    // Filtramos casos especiales
                    switch (temp>>8)
                    {
                    case KEY_ENTER:
                        {
                            if ( onEnter )
                            {
                                *onEnter = true ;
                            }
                        }
                        break ;
                    case KEY_BACKSPACE:
                        {
                            if ( texto != "" )
                            {
                                string temp = texto ;
                                texto = "" ;
                                for ( int i = 0 ; i < temp.size()-1 ; i++ )
                                {
                                    texto += temp.at(i);
                                }
                            }
                        }
                        break ;
                    default:
                        texto += (char)temp ;
                    }
                }
                
                if ( stringCambiada )
                {
                    delete imagenTexto ;
                    imagenTexto = new Imagen(fuenteTexto, texto+"_");
                }
            }
            break;
        }
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::TextBox::Interactuar()").Combinar(ex);
    }
} 

void
TextBox::SetTexto(string texto)
{
    TextBox::texto = texto ;
    if ( imagenTexto )
    {
        delete imagenTexto ;
    }
    if ( estado == 0 )
    {
        imagenTexto = new Imagen(fuenteTexto, texto);
    }
    else
    {
        imagenTexto = new Imagen(fuenteTexto, texto + "_" );
    }
}

string
TextBox::GetTexto()
{
    return texto ;
}

void
TextBox::Pintar()
{
    try
    {
        Interactuar();
        
        if ( imagenFondo )
        {
            imagenFondo->Pintar(x, y);
        }
        
        if ( imagenTexto )
        {
            imagenTexto->Pintar(x+relY, y+relY);
        }
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::TextBox::Pintar()").Combinar(ex);
    }
}

}
