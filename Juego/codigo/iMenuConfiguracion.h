#ifndef __I_MENUCONFIGURACION_H__
#define __I_MENUCONFIGURACION_H__

#include "iMenu.h"

namespace Interfaz
{

class MenuConfiguracion : public Menu
{
public:
    MenuConfiguracion(Menu *menuPadre);
    ~MenuConfiguracion();
    void ClickBoton1();
    void ClickBoton5();
    void Continuar();
};

}

#include "iMenuPrincipal.h"

#endif
