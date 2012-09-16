#include "gRectangulo.h"
#include "eExcepcionParametro.h"

namespace Graficos
{

Rectangulo::Rectangulo(int ancho, int alto, float r, float g, float b, float a, bool relleno, int X, int Y, int Z):
    Entidad2D(X, Y, Z)
{
    Rectangulo::ancho = ancho ;
    Rectangulo::alto = alto ;
    Rectangulo::relleno = relleno ;
    color[0] = r ;
    color[1] = g ;
    color[2] = b ;
    color[3] = a ;
}

Rectangulo::~Rectangulo()
{
    color[0] = 0 ;
    color[1] = 0 ;
    color[2] = 0 ;
    color[3] = 0 ;
    relleno = false ;
}

void
Rectangulo::SetRelleno(bool relleno)
{
    Rectangulo::relleno = relleno ;
}

void
Rectangulo::SetColor(float r, float g, float b, float a)
{
    color[0] = r ;
    color[1] = g ;
    color[2] = b ;
    color[3] = a ;
}    

void
Rectangulo::SetAncho(int ancho)
{
    Rectangulo::ancho = ancho ;
}

void
Rectangulo::SetAlto(int alto)
{
    Rectangulo::alto = alto ;
}

void
Rectangulo::Pintar()
{
    glColor4fv(color);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    
    if ( !relleno )
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(2.0);
    }
    
    glBegin(GL_QUADS);
        glVertex2f(x,y);
        glVertex2f(x,y+alto);
        glVertex2f(x+ancho,y+alto);
        glVertex2f(x+ancho,y);
    glEnd();
    
    if ( !relleno )
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glLineWidth(1.0);
    }
    
    glEnable(GL_TEXTURE_2D);
}

}
