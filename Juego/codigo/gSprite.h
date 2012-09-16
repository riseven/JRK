#ifndef __G_SPRITE_H__
#define __G_SPRITE_H__

#include "gEntidad2D.h"
#include "gImagen.h"

namespace Graficos
{

class Sprite : public Entidad2D
{
private:
    Imagen *imagen ;
    int alpha ;
public:
    Sprite(Imagen *ImagenPtr, int X, int Y, int Z);
    ~Sprite();
    int GetAncho();
    int GetAlto();
    void SetAlpha(int a){alpha = a;} ;
    void Pintar();
};

}

#endif
