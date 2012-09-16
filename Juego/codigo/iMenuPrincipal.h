#ifndef __I_MENUPRINCIPAL_H__
#define __I_MENUPRINCIPAL_H__

#include "iMenu.h"

namespace Interfaz
{

class MenuPrincipal : public Menu
{
private:
    int ImagenFondo ;
    int ImagenRaton ;
    int SpriteFondo ;
    int ImagenTitulo ;
    int SpriteTitulo ;
    int ImagenesBotones[5] ;
    int ImagenesBotonesSel[5] ;
    int Botones[5] ;
    bool ClickBoton[5] ;
    
public:
    MenuPrincipal();
    ~MenuPrincipal();
    void ClickBoton1();
    void ClickBoton2();
    void ClickBoton3();
    void ClickBoton5();
    void Continuar();
};

}

#endif
