#include "gSprite.h"
#include "eExcepcionParametro.h"

namespace Graficos
{

Sprite::Sprite(Imagen *ImagenPtr, int X, int Y, int Z):
    Entidad2D(X, Y, Z)
{
    if ( !ImagenPtr )
    {
        throw Error::ExcepcionParametro("Graficos::Sprite::Sprite(Imagen *, int, int, int)", 1);
    }
    imagen = ImagenPtr ;
    alpha = 255 ;
}

Sprite::~Sprite()
{
    imagen = NULL ;
    x = 0 ;
    y = 0 ; 
    z = 0 ;
}

int
Sprite::GetAncho()
{
    if (!imagen)
    {
        return 0 ;
    }
    return imagen->GetAncho();
}

int
Sprite::GetAlto()
{
    if (!imagen)
    {
        return 0;
    }
    return imagen->GetAlto();
}

void
Sprite::Pintar()
{
    try
    {
        if ( alpha == 255 )
        {
            imagen->Pintar(x, y);
        }
        else
        {
            imagen->PintarTransparente(x, y, alpha);
        }
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Sprite::Pintar()").Combinar(ex);
    }
}

}
