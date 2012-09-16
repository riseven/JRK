#include "eExcepcionInterna.h"

namespace Error
{
    
ExcepcionInterna::ExcepcionInterna():
    Excepcion("Se ha producido un error interno del programa"), origen("<No especificado>")
{
}

ExcepcionInterna::ExcepcionInterna(string lugarOrigen):
    Excepcion(), origen(lugarOrigen)
{
    string temp("Se ha producido un error interno en: ");
    temp += origen ;
    SetMensajeDeError(temp);
}

ExcepcionInterna::ExcepcionInterna(string lugarOrigen, string mensajeDeError):
    Excepcion(mensajeDeError), origen(lugarOrigen)
{
}

ExcepcionInterna::~ExcepcionInterna()
{
    origen = "" ;
}

string
ExcepcionInterna::GetOrigenExcepcion()
{
    return origen ;
}

void
ExcepcionInterna::SetOrigenExcepcion(string lugarExcepcion)
{
    origen = lugarExcepcion ;
}

}
