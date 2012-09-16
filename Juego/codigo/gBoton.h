#ifndef __G_BOTON_H__
#define __G_BOTON_H__

#include <string>

#include "gImagen.h"
#include "gEntidad2D.h"

using namespace std ;

namespace Graficos
{

class Boton : public Entidad2D
{
private:
    Imagen *imagenFondo ;
    Imagen *imagenSeleccion ;
    Imagen *imagenTexto ;
    //BITMAP *bitmapTexto ;
    string texto ;
    int alpha ;
    bool *onClickFlag ;
    bool *onMouseOver ;
    bool estabaPulsado ;
    bool estaFijo ;
    
public:
    Boton(Imagen *ImagenFondo, Imagen *ImagenSeleccion, Fuente *fuente, string Texto, int X, int Y, int Z);
    ~Boton();
    void OnClick(bool *OnClickFlag);
    void OnMouseOver(bool *flag);
    void CambiarImagenFondo(Imagen *ImagenFondo);    
    bool GetFijo(){return estaFijo;}
    void SetFijo(bool fijo){estaFijo = fijo;}
    void Pintar();   
};

}

#endif
