#ifndef __E_EXCEPCION_FICHERO_H__
#define __E_EXCEPCION_FICHERO_H__

#include <allegro.h>
#include <alleggl.h>

#include <string>

#include "eExcepcionLocalizada.h"

using namespace std ;

namespace Error
{

class ExcepcionFichero : public ExcepcionLocalizada
{
private:
    string nombreFichero ;
    
public:
    ExcepcionFichero() ;
    ExcepcionFichero(string lugarOrigen);
    ExcepcionFichero(string lugarOrigen, string nombreFichero);
    ExcepcionFichero(string lugarOrigen, string nombreFichero, string mensajeDeError);
    ~ExcepcionFichero() ;
    // El constructor de copia ye el operator= por defecto son validos
    string GetNombreFichero() ;
    void SetNombreFichero(string nombreFichero) ;
};

}

#endif
