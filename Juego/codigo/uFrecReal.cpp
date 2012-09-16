#include "ufrecreal.h"
#include <windows.h>
 
namespace Util
{

unsigned int FrecReal::tiempoActual ;

FrecReal::FrecReal()
{
    periodo=-1;
    ActualizarTiempo();
    ultimo = tiempoActual ;
}
        
FrecReal::FrecReal(float f)
{
    if (f<0) periodo=-1;
    else
        periodo=1000/f;
    ActualizarTiempo();
    ultimo=tiempoActual;      
}

/*
 * Devuelve true si es necesario comprobar 
*/

bool
FrecReal::comprobar()
{   
   //Si ya ha expirado, actualizar
   if (periodo<=(tiempoActual - ultimo))
   {
      ultimo+=periodo;
      return true;
      
   }else
      return false;
}


void
FrecReal::setFrec(float f)
{
    if (f<0) periodo=-1;
      else
        periodo=1000/f;
}

int
FrecReal::getTiempoSobrante()
{
    return (ultimo+periodo-tiempoActual) ;
    return ((ultimo+periodo-tiempoActual)*100)/periodo;
}

void
FrecReal::ActualizarTiempo()
{
    tiempoActual = GetTickCount();    
}

}
