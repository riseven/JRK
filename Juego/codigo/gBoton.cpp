#include "gApi.h"
#include "gBoton.h"
#include "eExcepcionParametro.h"

namespace Graficos
{

//------------------------------------------------------------------------------
//! Crea un nuevo objeto Boton a partir de una Imagen de fondo, una Imagen de
//! selección, una Fuente con la que se escribirá el texto, y un string con el
//! texto del boton. Además recibe los parámetros x, y, z que le pasará al
//! constructor de la clase base Entidad2D.
//------------------------------------------------------------------------------
Boton::Boton(Imagen *ImagenFondo, Imagen *ImagenSeleccion, Fuente *fuente, string texto, int X, int Y, int Z):
    Entidad2D(X, Y, Z)
{
    try
    {
        imagenFondo = ImagenFondo ;
        imagenSeleccion = ImagenSeleccion ;
        imagenTexto = NULL ;
     
        if ( ImagenFondo == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Boton::Boton(Imagen *, Imagen *, Fuente *, string, int, int, int)", 1);
        }   
        if ( fuente == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Boton::Boton(Imagen *, Imagen *, Fuente *, string, int, int, int)", 3);
        }
        
        if ( texto != "" )
        {
            imagenTexto = new Imagen(fuente, texto);
        }
        else
        {
            imagenTexto = NULL ;
        }
    
        alpha = 0 ;
        onClickFlag = NULL ;
        onMouseOver = NULL ;
        estabaPulsado = false ;
        estaFijo = false ;
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Boton::Boton(Imagen *, Imagen *, Fuente *, string, int, int, int)").Combinar(ex);
    }
}

Boton::~Boton()
{
    try
    {
        imagenFondo = NULL ;
        imagenSeleccion = NULL ;
        if ( imagenTexto )
        {
            delete imagenTexto ;
            imagenTexto = NULL ;
        }
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Boton::~Boton()").Combinar(ex);
    }
}

void
Boton::OnClick(bool *OnClickFlag)
{
    onClickFlag = OnClickFlag ;
}

void
Boton::OnMouseOver(bool *flag)
{
    onMouseOver = flag ;
}

void
Boton::CambiarImagenFondo(Imagen *ImagenFondo)
{
    if ( ImagenFondo == NULL )
    {
        throw Error::ExcepcionParametro("Graficos::Boton::CambiarImagenFondo(Imagen *)", 1);
    }
    imagenFondo = ImagenFondo ;
}

void
Boton::Pintar()
{
    try
    {
        imagenFondo->Pintar(x, y);
    
        if ( imagenTexto )
        {
            int tx = x + ( imagenFondo->GetAncho() - imagenTexto->GetAncho() )/2 ;
            int ty = y + ( imagenFondo->GetAlto() - imagenTexto->GetAlto() )/2 ;
            imagenTexto->Pintar(tx, ty);
        }
        
        if ( alpha > estaFijo?128:0 )
        {
            alpha -= 5 ;
            if ( alpha < estaFijo?128:0 )
            {
                alpha = estaFijo?128:0 ;
            }
        }
    
        if ( mouse_x >= x && mouse_x < x+imagenFondo->GetAncho() &&
            mouse_y >= y && mouse_y < y+imagenFondo->GetAlto() )
        {
            if ( onMouseOver )
            {
                *onMouseOver = true ;
            }
            
            if ( mouse_b & 1 )
            {
                estabaPulsado = true ;
                alpha = 196 ;
            }
            else
            {
                if ( onClickFlag )
                {
                    if ( estabaPulsado)
                    {
                        *onClickFlag = true ;
                        estabaPulsado = false ;
                    }
                    else
                    {
                        *onClickFlag = false ;
                    }
                }
                if ( alpha < 128 )
                {
                    alpha = 128 ;
                }
            }
        }
        else
        {   
            if ( onMouseOver )
            {
                *onMouseOver = false ;
            }
            
            if ( estaFijo )
            {
                alpha = 128 ;
            }
            
            if ( onClickFlag )
            {
                *onClickFlag = false ;
            }
            estabaPulsado = false ;
        }
    
        if ( imagenSeleccion && alpha != 0)
        {
            {
                imagenSeleccion->PintarTransparente(x, y, alpha);
            }   
        }
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Boton::Pintar()").Combinar(ex);
    }
}

}
