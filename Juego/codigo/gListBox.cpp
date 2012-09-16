#include "gListBox.h"
#include "eExcepcionLocalizada.h"

namespace Graficos
{
    
ListBox::ListBox(Imagen *ImagenFondo, Imagen *ImagenSeleccion, Imagen *ImagenResaltado,
            Imagen *ImagenScroll, Imagen *ImagenSeleccionScroll, Fuente *fuente,
            int LeftClient, int TopClient, int BottomClient,
            int LeftScroll, int TopScroll, int BottomScroll,
            int X, int Y, int Z):
    Entidad2D(X, Y, Z)
{
    imagenFondo = ImagenFondo ;
    imagenSeleccion = ImagenSeleccion ;
    imagenResaltado = ImagenResaltado ;
    imagenScroll = ImagenScroll ;
    imagenSeleccionScroll = ImagenSeleccionScroll ;
    fuenteTexto = fuente ; 
    
    leftClient = LeftClient ;
    topClient = TopClient ;
    bottomClient = BottomClient ;
    
    leftScroll = LeftScroll ;
    topScroll = TopScroll ;
    bottomScroll = BottomScroll ;
    
    posicion = 0 ;
    altoLinea = imagenSeleccion->GetAlto();
    lineasCaben = (bottomClient-topClient)/altoLinea ;
    vector <Imagen *> temp(0);
    lineas = temp ;
    lineaSeleccionada = 0 ;
    rightClient = imagenSeleccion->GetAncho() + leftClient ;
    outCambio = NULL ;
    outSeleccionado = NULL ;
    
}

ListBox::~ListBox()
{
    try
    {
        for ( vector <Imagen *>::iterator i = lineas.begin(); i != lineas.end() ; ++i )
        {
            if ( (*i) )
            {
                delete (*i) ;
            }
            (*i) = NULL ;
        }
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::ListBox::~ListBox()").Combinar(ex);
    }
}

void
ListBox::AddLinea(string Texto)
{
    try
    {
        lineas.push_back(new Imagen(fuenteTexto, Texto));
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::ListBox::AddLinea(string").Combinar(ex);
    }
}

void
ListBox::OnCambio(bool *Cambio, int *Seleccionado )
{
    outCambio = Cambio ;
    outSeleccionado = Seleccionado ;
}

void
ListBox::Pintar()
{
    try
    {
        if ( imagenFondo )
        {
            imagenFondo->Pintar(x,y);
        } 
        
        // Pintamos las lineas
        for ( int i = posicion ; i < lineas.size() && i-posicion < lineasCaben ; i++ )
        {
            int py = topClient + (i-posicion)*imagenSeleccion->GetAlto() ;
            int px = leftClient ;
            if ( lineaSeleccionada == i )
            {
                imagenSeleccion->Pintar(x+px, y+py);
            }
            int temp = (imagenSeleccion->GetAlto() - text_height(fuenteTexto->GetFont()) )/2 ;        
            py += temp ;
            px += temp ;        
            lineas.at(i)->Pintar(x+px,y+py);
        }      
        
        if ( outCambio )
        {
            *outCambio = false ;
        }
        // Pintamos el resaltado si el raton esta en el client
        if ( mouse_x >= x+leftClient && mouse_x < x+rightClient &&
            mouse_y >= y+topClient && mouse_y < y+bottomClient )
        {
            // Calculamos en que linea está
            int pos = mouse_y - y - topClient ;
            pos /= altoLinea ;
            imagenResaltado->PintarTransparente(x+leftClient, y+topClient+ pos*altoLinea, 128);
            
            // Comprobamos si se pulsa el raton izquierdo
            if ( mouse_b & 1 )
            {
                // Comprobamos que haya algo en esa linea
                if (pos + posicion < lineas.size() )
                {
                    lineaSeleccionada = pos + posicion ;
                    if ( outCambio )
                    {
                        *outCambio = true ;
                    }
                    if ( outSeleccionado )
                    {
                        *outSeleccionado = lineaSeleccionada ;
                    }                    
                }
            }
        }
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::ListBox::Pintar()").Combinar(ex);
    }
}



}
