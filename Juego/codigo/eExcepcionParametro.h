#ifndef __E_EXCEPCION_PARAMETRO_H__
#define __E_EXCEPCION_PARAMETRO_H__

#include <allegro.h>
#include <alleggl.h>

#include <string>

#include "eExcepcionLocalizada.h"

using namespace std ;

namespace Error
{

class ExcepcionParametro : public ExcepcionLocalizada
{
private:
    int numParametro ;
    
public:
    ExcepcionParametro();
    ExcepcionParametro(string lugarOrigen);
    ExcepcionParametro(string lugarOrigen, int numParametro);
    ExcepcionParametro(string lugarOrigen, int numParametro, string mensajeDeError);
    ~ExcepcionParametro();
    // El constructor de copia y el operator= por defecto son validos
    int GetNumParametroExcepcion() ;
    void SetNumParametroExcepcion(int numParametro);
};
  
}

#endif
