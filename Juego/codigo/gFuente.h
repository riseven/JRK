#ifndef _G_FUENTE_H__
#define _G_FUENTE_H__

#include <allegro.h>
#include <alleggl.h>
#include <string>

using namespace std ;

namespace Graficos
{

class Fuente
{
private:
    FONT *fuente ;
    
public:
    Fuente() ;
    Fuente(string NombreFichero) ;
    ~Fuente() ;
    FONT *GetFont() ;    
};

} // namespace Graficos

#endif
