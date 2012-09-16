#include "gFuente.h"

namespace Graficos
{

//------------------------------------------------------------------------------
//! Crea una Fuente para utilizar con la fuente estandar de allegro
//------------------------------------------------------------------------------
Fuente::Fuente()
{
    fuente = font ;
}

//------------------------------------------------------------------------------
//! Crea una Fuente cargandola de un fichero externo
//------------------------------------------------------------------------------
Fuente::Fuente(string NombreFichero)
{
    // TO DO
    fuente = NULL ;
}

//------------------------------------------------------------------------------
//! Destructor de la clase fuente. Libera la FONT * interna a menos que se trate
//! de la Fuente que encapsula a la FONT *font (fuente estandar de allegro)
//------------------------------------------------------------------------------
Fuente::~Fuente()
{
    if ( fuente != font && fuente)
    {
        allegro_gl_destroy_font(fuente) ;
    }
}

//------------------------------------------------------------------------------
//! Devuelve la FONT * interna a esta Fuente, para su uso por otras clases del
//! módulo gráfico.
//------------------------------------------------------------------------------
FONT *
Fuente::GetFont()
{
    return fuente ;
}

} // namespace Graficos
