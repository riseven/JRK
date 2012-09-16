#ifndef __E_EXCEPCION_INTERNA_H__
#define __E_EXCEPCION_INTERNA_H__

#include <allegro.h>
#include <alleggl.h>

#include <string>

#include "eExcepcion.h"

using namespace std;

namespace Error
{
    
class ExcepcionInterna : public Excepcion
{
private:
    string origen ;
    
public:
    ExcepcionInterna() ;
    ExcepcionInterna(string lugarOrigen) ;
    ExcepcionInterna(string lugarOrigen, string mensajeDeError) ;
    ~ExcepcionInterna() ;
    // El constructor de copia y el operator= por defecto son validos
    string GetOrigenExcepcion() ;
    void SetOrigenExcepcion(string origen) ;
};

}
#endif
