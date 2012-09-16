#ifndef __G_SEGMENTO_H__
#define __G_SEGMENTO_H__

#include "gEntidad2D.h"

namespace Graficos
{

class Segmento : public Entidad2D
{
private:
    bool clip ;
    int xmin, xmax ;
    int ymin, ymax ;
    
    int finX, finY ;
    float color[4] ;
public:
    Segmento(float r, float g, float b, float a, int X, int Y, int finX, int finY, int Z);
    ~Segmento();
    void SetClipping(int x0, int y0, int x1, int y1);
    void SetFinalSegmento(int finX, int finY);
    void Pintar();
};

}

#endif
