#include <allegro.h>
#include <alleggl.h>
#include "gBarraProgreso.h"

#include "gApi.h"
#include "eExcepcionParametro.h"

namespace Graficos
{

//------------------------------------------------------------------------------
//! Crea un nuevo objeto Boton a partir de una Imagen de fondo, una Imagen de
//! selección, una Fuente con la que se escribirá el texto, y un string con el
//! texto del boton. Además recibe los parámetros x, y, z que le pasará al
//! constructor de la clase base Entidad2D.
//------------------------------------------------------------------------------
BarraProgreso::BarraProgreso(Imagen *imagenFondo, Imagen *imagenProgreso, int bordeLateral, int separacion, int X, int Y, int Z):
    Entidad2D(X, Y, Z)
{
    try
    {
        BarraProgreso::imagenFondo = imagenFondo ;
        BarraProgreso::imagenProgreso = imagenProgreso ;
        BarraProgreso::bordeLateral = bordeLateral ;
        BarraProgreso::separacion = separacion ;
        
        numImagenesProgreso = (imagenFondo->GetAncho()-bordeLateral*2+separacion)/imagenProgreso->GetAncho() ;
        porcentaje = 0 ;
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::BarraProgreso::BarraProgreso(Imagen *, Imagen *, int, int, int, int, int)").Combinar(ex);
    }
}

BarraProgreso::~BarraProgreso()
{
    try
    {
        imagenFondo = NULL ;
        imagenProgreso = NULL ;
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::BarraProgreso::~BarraProgreso()").Combinar(ex);
    }
}

void
BarraProgreso::SetPorcentaje(int porcentaje)
{
    BarraProgreso::porcentaje = porcentaje ;
}

void
BarraProgreso::Pintar()
{
    
    try
    {

        imagenFondo->Pintar(x, y);

        int cantidad = (porcentaje * numImagenesProgreso)/100 ;
        if ( cantidad < numImagenesProgreso )
        {
            cantidad++ ;
        }
        
        for ( int i = 0 ; i < cantidad ; i++ )
        {
            imagenProgreso->Pintar(x + bordeLateral + (imagenProgreso->GetAncho()+separacion)*i, y+bordeLateral);
        }
        
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Boton::Pintar()").Combinar(ex);
    }
}

}
