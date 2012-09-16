#include "gBarraVida.h"
#include "eExcepcionParametro.h"

namespace Graficos
{

BarraVida::BarraVida(int ancho, int alto, int borde, int X, int Y, int Z):
    Entidad2D(X, Y, Z)
{
    BarraVida::ancho = ancho ;
    BarraVida::alto = alto ;
    BarraVida::borde = borde ;
    anchoInt = ancho - 2 * borde ;
    altoInt = alto - 2 * borde ;
    porcentaje = 100 ;
}

BarraVida::~BarraVida()
{
    ancho = 0 ;
    alto = 0 ;
    borde = 0 ;
    anchoInt = 0 ;
    altoInt = 0 ;
}

int
BarraVida::GetAncho()
{
    return ancho ;
}

int
BarraVida::GetAlto()
{
    return alto ;
}

void
BarraVida::SetPorcentaje(float porcentaje)
{
    BarraVida::porcentaje = porcentaje ;
}

void
BarraVida::Pintar()
{
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
    glBegin(GL_QUADS);
        glColor4f(0,0,0,0.5);
        glVertex2f(x,y);
        glVertex2f(x,y+alto);
        glVertex2f(x+ancho,y+alto);
        glVertex2f(x+ancho,y);
        
        glColor4f(porcentaje>50?(2-porcentaje/50.0):1,porcentaje>50?1:(porcentaje/50.0),0,1);
        glVertex2f(x+borde,y+borde);
        glVertex2f(x+borde,y+borde+altoInt);
        glVertex2f(x+borde+anchoInt*porcentaje/100.0, y+borde+altoInt);
        glVertex2f(x+borde+anchoInt*porcentaje/100.0, y+borde);        
    glEnd();
    
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}

}
