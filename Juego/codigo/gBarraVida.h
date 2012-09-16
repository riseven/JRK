#ifndef __G_BARRAVIDA_H__
#define __G_BARRAVIDA_H__

#include "gEntidad2D.h"

namespace Graficos
{

class BarraVida : public Entidad2D
{
private:
    int ancho, alto ;
    int borde ;
    float porcentaje ;
    
    int anchoInt, altoInt ;
public:
    BarraVida(int ancho, int alto, int borde, int X, int Y, int Z);
    ~BarraVida();
    int GetAncho();
    int GetAlto();
    void SetPorcentaje(float porcentaje);
    void Pintar();
};

}

#endif
