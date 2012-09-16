#include <allegro.h>
#include <alleggl.h>

#include "gSonido.h"
#include "eExcepcionParametro.h"
#include "eExcepcionLocalizada.h"
#include "iConfiguracion.h"

namespace Graficos
{

Sonido::Sonido(string nombreFichero)
{
    sample = NULL ;
    char buf[256] ;
    replace_filename(buf, Interfaz::Configuracion::GetNombreDirectorio().c_str(), nombreFichero.c_str(), sizeof(buf));
    sample = load_sample(buf);
    
    if ( !sample )
    {
        throw Error::ExcepcionParametro("Graficos::Sonido::Sonido(string)", 1);
    }
}    

Sonido::~Sonido()
{
    if ( sample )
    {
        destroy_sample(sample);
    }
}

void
Sonido::Play()
{    
    if ( !sample )
    {
        throw Error::ExcepcionLocalizada("Graficos::Sonido::Play()");
    }
    play_sample(sample, 255, 128, 1000, 0);
}

void
Sonido::PlayLoop()
{
    if ( !sample )
    {
        throw Error::ExcepcionLocalizada("Graficos::Sonido::PlayLoop()");
    }
    play_sample(sample, 255, 128, 1000, 1);
}

}
