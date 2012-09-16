#ifndef __I_MENU_MAPA_H__
#define __I_MENU_MAPA_H__

#include <allegro.h>
#include <alleggl.h>

#include <vector>

#include "iInfoMapa.h"
#include "iMenu.h"

using namespace std;

namespace Interfaz
{

class MenuPrincipal;

class MenuMapa : public Menu
{
private:
    int imagenFondoListBox ;
    int imagenSeleccionListBox ;
    int imagenResaltadoListBox ;
    int handlerListBox ;
    bool cambioListBox ;
    
    int seleccionListBox ;
    vector <InfoMapa *> infoMapas ;
    int imagenPreview ;
    int spritePreview ;
    int imagenFondoPreview ;
    int spriteFondoPreview ;
    
public:
    MenuMapa(Menu *menuPadre);
    ~MenuMapa();
    void Actualizar();
    void Continuar();
    void Activar();
    void Desactivar();
    void ClickBoton3();
    void ClickBoton4();
    void ClickBoton5();
};

}
#endif
