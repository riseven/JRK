#ifndef __G_CHATBOX_H__
#define __G_CHATBOX_H__

#include <string>
#include <list>

#include "gImagen.h"
#include "gFuente.h"
#include "gEntidad2D.h"

using namespace std ;

namespace Graficos
{

class ChatBox : public Entidad2D
{
private:
    typedef struct Linea
    {
        int alpha ;
        int tiempo ;
        Imagen *imagenTexto ;
    };
    
    Imagen *imagenFondo ;
    Fuente *fuenteTexto ;
    list<Linea> linea ;
    int ancho, alto, borde ;
    
    int numLineasVisibles ;
    int relY ;
    
public:
    ChatBox( Imagen *Fondo, Fuente *fuente, int ancho, int alto, int borde, int X, int Y, int Z);
    ~ChatBox();
    void AddLinea(string texto, int r, int g, int b);
    void Pintar();
}; 
    
} // namespace Graficos

#endif // __G_CHATBOX_H__
