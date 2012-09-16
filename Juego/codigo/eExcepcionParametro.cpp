#include "eExcepcionParametro.h"

namespace Error
{
    
ExcepcionParametro::ExcepcionParametro():
    ExcepcionLocalizada("<No especificado>", "Error en un parametro de entrada"), numParametro(0)
{
}

ExcepcionParametro::ExcepcionParametro(string lugarOrigen):
    ExcepcionLocalizada(lugarOrigen, "Error en un parametro de entrada"), numParametro(0)
{
}

ExcepcionParametro::ExcepcionParametro(string lugarOrigen, int numParametro):
    ExcepcionLocalizada(lugarOrigen, "Error en el parametro de entrada :"), numParametro(numParametro)
{
    string temp = GetMensajeDeError();
    temp += ((char)numParametro)+'0' ;
    SetMensajeDeError(temp);
}

ExcepcionParametro::ExcepcionParametro(string lugarOrigen, int numParametro, string mensajeDeError):
    ExcepcionLocalizada(lugarOrigen, mensajeDeError), numParametro(numParametro)
{
}

ExcepcionParametro::~ExcepcionParametro()
{
    numParametro = 0 ;
}

int
ExcepcionParametro::GetNumParametroExcepcion()
{
    return numParametro ;
}

void
ExcepcionParametro::SetNumParametroExcepcion(int numParametro)
{
    ExcepcionParametro::numParametro = numParametro ;
}

}
