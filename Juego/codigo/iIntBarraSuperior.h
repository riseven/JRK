#ifndef __I_INTERFAZ_BARRASUPERIOR_H__
#define __I_INTERFAZ_BARRASUPERIOR_H__

namespace Interfaz
{

class IntBarraSuperior
{
   private:
      int btnMenu;
      bool onClickMenu;
      int imgMenuFondo;
      int imgMenuFrente;
      int imgBarraSuperior;
      int sprBarraSuperior;
      int imgDinero;
      int sprDinero;
      int imgPoblacion;
      int sprPoblacion;
      int posXDinero,posYDinero;
      int posXPoblacion,posYPoblacion;
      int ultCantidadDinero;  //Ultima Cantidad de dinero
      int ultPoblacion, ultPoblacionMax;
   public:
      IntBarraSuperior();
      ~IntBarraSuperior();
      void SetDineroActual(int dinero);
      void SetPoblacionActual(int poblacion, int max);
      void Actualizar();
      void Activar();
      void Desactivar();
};

}

#endif
