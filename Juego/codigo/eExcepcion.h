#ifndef __E_EXCEPCION_H__
#define __E_EXCEPCION_H__

#include <allegro.h>
#include <alleggl.h>

#include <string>

using namespace std;

namespace Error
{
    
class Excepcion
{
private:
    string mensajeDeError ;
    
public:
    Excepcion();
    Excepcion(string mensajeDeError);
    virtual ~Excepcion();
    // Constructor de copia y operator= por defecto son validos para esta clase
    string GetMensajeDeError();
    void SetMensajeDeError(string mensajeDeError);
    Excepcion & Combinar(Excepcion &ex);
};

}

#endif
