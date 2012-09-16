#include "eExcepcionLocalizada.h"

namespace Error
{
    
ExcepcionLocalizada::ExcepcionLocalizada():
    Excepcion(), origen("<No especificado>")
{
    string temp(origen);
    temp += ": " ;
    temp += "Se ha producido un error interno del programa" ;
    SetMensajeDeError(temp);
}

ExcepcionLocalizada::ExcepcionLocalizada(string lugarOrigen):
    Excepcion(), origen(lugarOrigen)
{
    string temp(origen);
    temp += ": " ;
    temp += "Se ha producido un error interno del programa" ;
    SetMensajeDeError(temp);    
}

ExcepcionLocalizada::ExcepcionLocalizada(string lugarOrigen, string mensajeDeError):
    Excepcion(), origen(lugarOrigen)
{
    string temp(origen);
    temp += ": " ;
    temp += mensajeDeError;
    SetMensajeDeError(temp);
}

ExcepcionLocalizada::~ExcepcionLocalizada()
{
    origen = "" ;
}

string
ExcepcionLocalizada::GetOrigenExcepcion()
{
    return origen ;
}

void
ExcepcionLocalizada::SetOrigenExcepcion(string lugarExcepcion)
{
    origen = lugarExcepcion ;
}

}
