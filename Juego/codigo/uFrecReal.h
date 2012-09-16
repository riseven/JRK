/*
 * Esta clase guarda frecuencias de ejecución
 * medidas con funciones de tiempo, para independizar
 * las animaciones de la velocidad de la CPU
 *
 * Se llamara a comprobar() y sólo devolverá
 * true con la frecuencia definida en la clase
 * 
 *
*/
#ifndef __L_FRECREAL_H__
#define __L_FRECREAL_H__

namespace Util
{
          
class FrecReal
{
      public:
             FrecReal();
             FrecReal(float f);
             bool comprobar();
             void setFrec(float f);
             int getTiempoSobrante();
             
             static unsigned int tiempoActual ;
             
             static void ActualizarTiempo();
      
      private:
              float periodo;//Guarda milisegundos
              unsigned int ultimo;
};

}

#endif
