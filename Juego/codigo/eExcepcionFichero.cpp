#include "eExcepcionFichero.h"

namespace Error
{

ExcepcionFichero::ExcepcionFichero():
    ExcepcionLocalizada("<No especificado>", "Error de fichero")
{
}
    
ExcepcionFichero::ExcepcionFichero(string lugarOrigen):
    ExcepcionLocalizada(lugarOrigen, "Error de fichero"), nombreFichero("<No especificado>")
{
}

ExcepcionFichero::ExcepcionFichero(string lugarOrigen, string nombreFichero):
    ExcepcionLocalizada(lugarOrigen, "Error de fichero: "), nombreFichero(nombreFichero)
{
    string temp = GetMensajeDeError();
    temp += nombreFichero ;
    SetMensajeDeError(temp);
}

ExcepcionFichero::ExcepcionFichero(string lugarOrigen, string nombreFichero, string mensajeDeError):
    ExcepcionLocalizada(lugarOrigen, mensajeDeError), nombreFichero(nombreFichero)
{
}

ExcepcionFichero::~ExcepcionFichero()
{
    nombreFichero = "" ;
}

string
ExcepcionFichero::GetNombreFichero()
{
    return nombreFichero ;
}

void
ExcepcionFichero::SetNombreFichero(string nombreFichero)
{
    ExcepcionFichero::nombreFichero = nombreFichero ;
}

}
