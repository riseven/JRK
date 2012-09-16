#include "eExcepcion.h"

namespace Error
{
    
Excepcion::Excepcion():
    mensajeDeError()
{
}

Excepcion::Excepcion(string mensajeDeError):
    mensajeDeError(mensajeDeError)
{
}

Excepcion::~Excepcion()
{
    mensajeDeError = "" ;
}

string
Excepcion::GetMensajeDeError()
{
    return mensajeDeError ;
}

void
Excepcion::SetMensajeDeError(string mensajeDeError)
{
    Excepcion::mensajeDeError = mensajeDeError ;
}

Excepcion &
Excepcion::Combinar(Excepcion &ex)
{
    string temp = ex.GetMensajeDeError() ;
    temp += "\n" ;
    temp += GetMensajeDeError();
    mensajeDeError = temp ;
    return *this;
}

}
