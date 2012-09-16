#ifndef __G_BARRA_PROGRESO_H__
#define __G_BARRA_PROGRESO_H__

#include <allegro.h>
#include <alleggl.h>

#include <string>

#include "gImagen.h"
#include "gEntidad2D.h"

using namespace std ;

namespace Graficos
{

class BarraProgreso : public Entidad2D
{
private:
    Imagen *imagenFondo ;
    Imagen *imagenProgreso ;
    int bordeLateral ;
    int separacion ;
    int porcentaje ;
    
    int numImagenesProgreso ;
    
public:
    BarraProgreso(Imagen *ImagenFondo, Imagen *ImagenProgreso, int bordeLateral, int separacion, int X, int Y, int Z);
    ~BarraProgreso();
    void SetPorcentaje(int porcentaje);
    void Pintar();   
};

}

#endif
