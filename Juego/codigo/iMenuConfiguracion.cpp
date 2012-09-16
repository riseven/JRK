#include "iMenuConfiguracion.h"
#include "gApi.h"
#include "iApi.h"
#include "eExcepcionLocalizada.h"
#include "iMenuConfiguracionVideo.h"

namespace Interfaz
{

//Crea menu de configuracion
MenuConfiguracion::MenuConfiguracion(Menu *menuPadre):
    Menu(menuPadre, "Configuracion", "Video", "Audio", "Juego", "", "Volver")
{

}

MenuConfiguracion::~MenuConfiguracion()
{
}

void
MenuConfiguracion::ClickBoton1()
{
    Desactivar();
    Menu::ClickBoton1();
    menuHijo = new MenuConfiguracionVideo(this);    
    Api::SetMenuActual(menuHijo);
}

void
MenuConfiguracion::ClickBoton5()
{
    try
    {
        Menu::ClickBoton5();
        menuPadre->Continuar();
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::MenuConfiguracion::ClickBoton5()").Combinar(ex);
    }
}

void
MenuConfiguracion::Continuar()
{
    if ( menuHijo == NULL )
    {
        throw Error::ExcepcionLocalizada("Interfaz::MenuPrincipal::Continuar()");
    }
    //Segun el boton pusado entramos en otro submenu
    switch (botonPulsado)
    {
        case 1:    
            delete menuHijo ;
            menuHijo = NULL ;
            Activar();
            Api::SetMenuActual(this);
            break ;
    }
}



}
