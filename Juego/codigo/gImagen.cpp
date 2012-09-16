#include "gImagen.h"
#include "gApi.h"
#include "iConfiguracion.h"
#include "eExcepcionFichero.h"
#include "eExcepcionLocalizada.h"
#include "eExcepcionParametro.h"

#include <iostream>

namespace Graficos
{

BITMAP *
Imagen::CargarBitmap(string nombreFichero)
{
    BITMAP *bitmap = NULL ;;
    char buf[256] ;
    replace_filename(buf, Interfaz::Configuracion::GetNombreDirectorio().c_str(), nombreFichero.c_str(), sizeof(buf));
    bitmap = load_bitmap(buf, NULL);
    
    if (!bitmap)
    {
        throw Error::ExcepcionFichero(nombreFichero);
    }
    return bitmap ;
}

//------------------------------------------------------------------------------
//! Crea una nueva Imagen a partir de un fichero de imagen. Se debe comprobar si
//! ha sido posible crear la imagen llamando al metodo ErrorEnCreacion()
//------------------------------------------------------------------------------
Imagen::Imagen(string nombreFichero)
{
    bitmap = CargarBitmap(nombreFichero);
    masked = false ;
    
    // Creamos la textura
    textura = allegro_gl_make_texture_ex(AGL_TEXTURE_MIPMAP | AGL_TEXTURE_MASKED | AGL_TEXTURE_FLIP | AGL_TEXTURE_RESCALE, bitmap, -1);
    
    w = bitmap->w ;
    h = bitmap->h ;    
    
    destroy_bitmap(bitmap);
    bitmap = NULL ;    
}

//------------------------------------------------------------------------------
//! Crea una nueva imagen a partir de una fuente y un texto. El tamaño de la 
//! imagen será la justa para que quepa el texto. Se debe comprobar si ha sido 
//! posible crear la imagen llamando al metodo ErrorEnCreacion()
//------------------------------------------------------------------------------
Imagen::Imagen(Fuente *fuente, string texto)
{
    if ( !fuente )
    {
        bitmap = NULL ;
        throw Error::ExcepcionParametro("Graficos::Imagen::Imagen(Fuente *, string)", 1);
    }
    
    // Creamos el bitmap con el tamaño apropiado
    bitmap = create_bitmap( text_length(fuente->GetFont(), texto.c_str()), text_height(fuente->GetFont()) );
    if (!bitmap)
    {
        throw Error::ExcepcionLocalizada("Graficos::Imagen::Imagen(Fuente *, string)", "No se pudo crear el bitmap");
    }
    
    // Pintamos el texto en el bitmap creado, con el color que trae de "fabrica"
    // y con el color de fondo trasparente
    textout_ex(bitmap, fuente->GetFont(), texto.c_str(), 0, 0, -1, makecol(255,0,255));
    
    // Por defecto una imagen no se pinta con mascara trasparente, pero una
    // imagen generada por una fuente si que lo hace por defecto.
    masked = true ;
    
    textura = allegro_gl_make_masked_texture(bitmap);
    
    w = bitmap->w ;
    h = bitmap->h ;
    
    destroy_bitmap(bitmap);
    bitmap = NULL ;    
}

Imagen::Imagen(Fuente *fuente, string texto, int r, int g, int b)
{
    if ( !fuente )
    {
        bitmap = NULL ;
        throw Error::ExcepcionParametro("Graficos::Imagen::Imagen(Fuente *, string)", 1);
    }
    
    // Creamos el bitmap con el tamaño apropiado
    bitmap = create_bitmap( text_length(fuente->GetFont(), texto.c_str()), text_height(fuente->GetFont()) );
    if (!bitmap)
    {
        throw Error::ExcepcionLocalizada("Graficos::Imagen::Imagen(Fuente *, string)", "No se pudo crear el bitmap");
    }
    
    // Pintamos el texto en el bitmap creado, con el color que trae de "fabrica"
    // y con el color de fondo trasparente
    textout_ex(bitmap, fuente->GetFont(), texto.c_str(), 0, 0, makecol(r,g,b), makecol(255,0,255));
    
    // Por defecto una imagen no se pinta con mascara trasparente, pero una
    // imagen generada por una fuente si que lo hace por defecto.
    masked = true ;
    
    textura = allegro_gl_make_masked_texture(bitmap);
    
    w = bitmap->w ;
    h = bitmap->h ;    
    
    destroy_bitmap(bitmap);
    bitmap = NULL ;
}    

Imagen::~Imagen()
{
    if ( bitmap )
    {
        destroy_bitmap(bitmap);
    }
    glDeleteTextures(1, &textura);
}

BITMAP *
Imagen::GetBitmap()
{
    throw Error::ExcepcionLocalizada("Graficos::Imagen::GetBitmap()", "Esto ya no se permite");
}

void
Imagen::SetMasked(bool Masked)
{
    masked = Masked ;
}

bool
Imagen::GetMasked()
{
    return masked ;
}

int
Imagen::GetAncho()
{
    return w ;
}

int
Imagen::GetAlto()
{
    return h ;
}

void
Imagen::Pintar(int x, int y)
{
    try
    {        
        if ( masked )
        {
            Api::MaskedBlit(textura, 0, 0, x, y, w, h);
        }
        else
        {
            Api::Blit(textura, 0, 0, x, y, w, h);
        }
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Imagen::Pintar(int, int)").Combinar(ex);
    }
}

void
Imagen::PintarTransparente(int x, int y, int alpha)
{
    try
    {    
        Api::DrawTransSprite(textura, 0, 0, x, y, w, h, alpha);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::PintarTransparente(int, int, int)").Combinar(ex);
    }
}

GLuint
Imagen::GetTextura()
{
    return textura ;
}


void 
Imagen::PintarPixel(int x, int y, char r, char g, char b)
{
    throw Error::ExcepcionLocalizada("Graficos::Imagen::PintarPixel(int, int, char, char, char)", "Esto ya no se permite");
}


void
Imagen::AplicarMascara()
{
    throw Error::ExcepcionLocalizada("Graficos::Imagen::AplicarMascara()", "Esto ya no se permite");
}

}
