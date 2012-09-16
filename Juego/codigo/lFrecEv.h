/*
 * Esta clase guarda las frecuencias asociadas a los eventos
 * para saber si es necesario comprobarlo 
 *
*/
#ifndef __L_FRECEV_H__
#define __L_FRECEV_H__

namespace Logica
{
          
class FrecEv
{
      public:
             FrecEv();
             FrecEv(float r);
             bool comprobar();
             void setFrec(float v);
      
      private:
              float frec,actual;
};

}

#endif
