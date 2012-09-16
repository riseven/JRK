#ifndef __G_RECTANGULO_H__
#define __G_RECTANGULO_H__

#include "gEntidad2D.h"

namespace Graficos
{

class Rectangulo : public Entidad2D
{
private:
    bool relleno ;
    int ancho, alto ;
    float color[4] ;
public:
    Rectangulo(int ancho, int alto, float r, float g, float b, float a, bool relleno, int X, int Y, int Z);
    ~Rectangulo();
//    bool operator >=(const Sprite& sprite);
    void SetRelleno(bool relleno);
    void SetColor(float r, float g, float b, float a);
    void SetAncho(int ancho);
    void SetAlto(int alto);
    void Pintar();
};

}

#endif
