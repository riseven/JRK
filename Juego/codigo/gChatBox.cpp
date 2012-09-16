#include <allegro.h>
#include <alleggl.h>

#include "gChatBox.h"
#include "eExcepcionLocalizada.h"

namespace Graficos
{

ChatBox::ChatBox( Imagen *Fondo, Fuente *fuente, int ancho, int alto, int borde, int X, int Y, int Z ):
    Entidad2D(X, Y, Z)
{
    imagenFondo = Fondo ;
    fuenteTexto = fuente ;
    ChatBox::ancho = ancho ;
    ChatBox::alto = alto ;
    ChatBox::borde = borde ;
    numLineasVisibles = (alto-2*borde) / text_height(fuente->GetFont()) ;    
    relY = Y + alto - borde - text_height(fuente->GetFont()) ;
    
    linea.clear();
}

ChatBox::~ChatBox()
{
    try
    {
        linea.clear();
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::ChatBox::~ChatBox()").Combinar(ex);
    }
}

void
ChatBox::AddLinea(string texto, int r, int g, int b)
{
    Imagen *imagen = new Imagen(fuenteTexto, texto, r, g, b);
    
    Linea l ;
    l.alpha = 255 ;
    l.tiempo = 0 ;
    l.imagenTexto = imagen ;
    
    linea.push_front(l);
    
    while ( linea.size() > numLineasVisibles )
    {
        delete linea.back().imagenTexto ;
        linea.pop_back() ;
    }
}

void
ChatBox::Pintar()
{
    try
    {
        // Actualizamos
        for ( list<Linea>::iterator i = linea.begin() ; i != linea.end() ; ++i )
        {
            (*i).tiempo++ ;
            if ( (*i).tiempo > 200 )
            {
                if ( (*i).alpha > 0 )
                {
                    (*i).alpha-- ;
                }
            }
        }
        
        if ( imagenFondo )
        {
            imagenFondo->Pintar(x, y);
        }
        
        // Pintamos las lineas
        int posy = relY ;
        for ( list<Linea>::iterator i = linea.begin() ; i!= linea.end() ; ++i )
        {
            (*i).imagenTexto->PintarTransparente(x+borde, posy, (*i).alpha);
            posy -= text_height(fuenteTexto->GetFont());
        }
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::ChatBox::Pintar()").Combinar(ex);
    }
}

}
