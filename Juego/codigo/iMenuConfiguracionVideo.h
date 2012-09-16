#ifndef __I_MENUCONFIGURACIONVIDEO_H__
#define __I_MENUCONFIGURACIONVIDEO_H__

#include "iMenu.h"

namespace Interfaz
{

class MenuConfiguracionVideo : public Menu
{
private:
    int fondoBotonMultiTextura ;
    int selecBotonMultiTextura ;
    int botonMultiTextura ;
    int botonDiaNoche ;
    int botonEstaciones ;
    
    bool clickMultiTextura ;
    bool clickDiaNoche ;
    bool clickEstaciones ;
    
public:
    MenuConfiguracionVideo(Menu *menuPadre);
    ~MenuConfiguracionVideo();
    void ClickBoton5();
    void Actualizar();    
};

}

#include "iMenuPrincipal.h"

#endif
