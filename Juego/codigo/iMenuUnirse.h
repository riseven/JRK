#ifndef __I_MENUUNIRSE_H__
#define __I_MENUUNIRSE_H__

#include <allegro.h>
#include <alleggl.h>

#include "iMenu.h"

namespace Interfaz
{

class MenuPrincipal;

class MenuUnirse : public Menu
{
private:
    int imagenTextBox ;
    int textBox ;

public:
    MenuUnirse(Menu *menuPadre);
    ~MenuUnirse();
    void Continuar();
    void Activar();
    void Desactivar();
    void ClickBoton4();
    void ClickBoton5(); 
};

}

#include "iMenuPrincipal.h"

#endif
