#ifndef __G_TEXTBOX_H__
#define __G_TEXTBOX_H__

#include <string>

#include "gImagen.h"
#include "gFuente.h"
#include "gEntidad2D.h"

using namespace std ;

namespace Graficos
{

class TextBox : public Entidad2D
{
private:
    Imagen *imagenFondo ;
    Fuente *fuenteTexto ;
    string texto ;
    int relY ;
    Imagen *imagenTexto ;
    int estado ;
    
    bool *onEnter ;
    
    void Interactuar();
    
public:
    TextBox( Imagen *Fondo, Fuente *fuente, string Texto, int X, int Y, int Z);
    ~TextBox();
    void SetTexto(string texto);
    string GetTexto();
    void OnEnter(bool *flag);
    void Pintar();
}; 
    
} // namespace Graficos

#endif // __G_TEXTBOX_H__
