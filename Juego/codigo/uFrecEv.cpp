#include "ufrecev.h"

using namespace Util;
 
FrecEv::FrecEv()
{
  frec=actual=0.;
}
        
FrecEv::FrecEv(float r)
{
  if (r<0) frec=0.;
  else
      if (r>1) frec=1.;
  else
      frec=r;
      
  actual=0.;
}

/*
 * Devuelve true si es necesario comprobar el evento
*/
bool
FrecEv::comprobar()
{
   actual+=frec;
   
   //Si se ha llegado a 1, el evento se debe producir
   if (actual>=1)
   {
      actual-=1;//Se quita 1 para guardar el exceso
      return true;
   }else
      return false;
}

void
FrecEv::setFrec(float v)
{
    if (v<0) frec=0.;
  else
      if (v>1) frec=1.;
  else
      frec=v;
}
