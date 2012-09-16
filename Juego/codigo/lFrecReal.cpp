#include "lfrecreal.h"
#include <windows.h>

using namespace Logica;
 
FrecReal::FrecReal()
{
  periodo=-1;
  ultimo=0;
}
        
FrecReal::FrecReal(float f)
{
  if (f<0) periodo=-1;
  else
      periodo=1000/f;
      
}

/*
 * Devuelve true si es necesario comprobar 
*/
bool
FrecReal::comprobar()
{
   unsigned int temp=GetTickCount();
   
   //Si ya ha expirado, actualizar
   if (periodo<=(temp - ultimo))
   {
      ultimo=temp;
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

