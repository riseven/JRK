#ifndef __E_EXCEPCION_LOCALIZADA_H__
#define __E_EXCEPCION_LOCALIZADA_H__

#include <allegro.h>
#include <alleggl.h>

#include <string>

#include "eExcepcion.h"

using namespace std;

namespace Error
{
    
class ExcepcionLocalizada : public Excepcion
{
private:
    string origen ;
    
public:
    ExcepcionLocalizada() ;
    ExcepcionLocalizada(string lugarOrigen) ;
    ExcepcionLocalizada(string lugarOrigen, string mensajeDeError) ;
    ~ExcepcionLocalizada() ;
    // El constructor de copia y el operator= por defecto son validos
    string GetOrigenExcepcion() ;
    void SetOrigenExcepcion(string origen) ;
};

}
#endif
