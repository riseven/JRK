#ifndef __I_MENUNOMBRE_H__
#define __I_MENUNOMBRE_H__

#include <allegro.h>
#include <alleggl.h>

#include <string>

#include "iMenu.h"

using namespace std; 

namespace Interfaz
{

class MenuNombre : public Menu
{
private:
    bool aceptado ;
    int imagenTextBox ;
    int handlerTextBox ;
public:
    MenuNombre(Menu *menuPadre);
    ~MenuNombre();
    bool Aceptado();
    void ClickBoton4();
    void ClickBoton5();
};

}

#include "iMenuPrincipal.h"

#endif
