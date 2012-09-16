#include "iMenuPrincipal.h"
#include "gApi.h"
#include "iApi.h"
#include "iConfiguracion.h"
#include "iJuego.h"
#include "eExcepcionLocalizada.h"
#include "iMenuNombre.h"
#include "iMenuConfiguracion.h"
#include "iMenuMapa.h"
#include "iMenuUnirse.h"

namespace Interfaz
{ 

MenuPrincipal::MenuPrincipal():
    Menu(NULL, "Menu Principal", "Crear partida", "Unirse a partida", "Configuracion", "Creditos", "Salir")
{
}

MenuPrincipal::~MenuPrincipal()
{
}

//Entra en el menu Nombre
void
MenuPrincipal::ClickBoton1()
{
    try
    {
        //Desactivo menu actual
        Desactivar();
        //Pulso boton 1
        Menu::ClickBoton1();
        //Guardo como menu hijo el MenuNombre
        menuHijo = new MenuNombre(this);
        //Lo establezco como menu actual
        Api::SetMenuActual(menuHijo);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::MenuPrincipal::ClickBoton1()").Combinar(ex);
    }
}

//Entra en el menu Nombre
void
MenuPrincipal::ClickBoton2()
{
    try
    {
        Desactivar();
        Menu::ClickBoton2();
        menuHijo = new MenuNombre(this);
        Api::SetMenuActual(menuHijo);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::MenuPrincipal::ClickBoton2()").Combinar(ex);
    }
}

//Entra en el menu de configuracion
void
MenuPrincipal::ClickBoton3()
{
    try
    {
        Desactivar();
        Menu::ClickBoton3();
        menuHijo = new MenuConfiguracion(this);
        Api::SetMenuActual(menuHijo);    
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::MenuPrincipal::ClickBoton3()").Combinar(ex);
    }
}

// Volver
void
MenuPrincipal::ClickBoton5()
{
    try
    {
        Api::SetMenuActual(NULL);
        delete this ;
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::MenuPrincipal::ClickBoton5()").Combinar(ex);
    }
}

//Ejecuta el menu hijo
void
MenuPrincipal::Continuar()
{
    try
    {
        if ( menuHijo == NULL )
        {
            throw Error::ExcepcionLocalizada("Interfaz::MenuPrincipal::Continuar()");
        }
        //Segun el boton pusado entramos en otro submenu
        switch (botonPulsado)
        {
            case 1: //Si se ha elegido Aceptar en el menu hijo de la opcion 1 se carga el Menu de Mapas
                if ( ((MenuNombre *) menuHijo)->Aceptado() )
                {
                    delete menuHijo ;
                    menuHijo = new MenuMapa(this);
                    Api::SetMenuActual(menuHijo);
                    botonPulsado = 6 ;
                }
                else // Sino se carga el menu actual
                {
                    delete menuHijo ;
                    menuHijo = NULL ;
                    Activar();
                    Api::SetMenuActual(this);
                }
                break;
            case 2:
                if ( ((MenuNombre *) menuHijo)->Aceptado() )
                {
                    delete menuHijo ;
                    menuHijo = new MenuUnirse(this);
                    Api::SetMenuActual(menuHijo);
                    botonPulsado = 7 ; 
                }
                else
                {
                    delete menuHijo ;
                    menuHijo = NULL ;
                    Activar();
                    Api::SetMenuActual(this);
                }
                break ;                
            case 3:
                delete menuHijo ;
                menuHijo = NULL ;
                Activar();
                Api::SetMenuActual(this);            
                break ;
            case 6:
                delete menuHijo ;
                menuHijo = NULL ;
                Activar();
                Api::SetMenuActual(this);
                break ;
            case 7:
                delete menuHijo ;
                menuHijo = NULL ;
                Activar();
                Api::SetMenuActual(this);
                break ;
            default:
                throw Error::ExcepcionLocalizada("Interfaz::MenuPrincipal::Continuar()");            
        }
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::MenuPrincipal::Continuar()").Combinar(ex);
    }
}

}
