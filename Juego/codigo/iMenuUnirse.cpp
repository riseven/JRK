#include <Allegro.h>
#include "iMenuUnirse.h"
#include "iMenuPartida.h"
#include "iConfiguracion.h"
#include "gApi.h"
#include "rApi.h"
#include "iApi.h"
#include "eExcepcionLocalizada.h"
#include "lApi.h"


namespace Interfaz
{

MenuUnirse::MenuUnirse(Menu *menuPadre):
    Menu(menuPadre, "Unirse a partida", "", "", "", "Unirse", "Volver")
{
    // Creo el textbox
    imagenTextBox = Graficos::Api::CrearImagen("Graficos\\TextBoxNick.bmp");
    textBox = Graficos::Api::CrearTextBox(imagenTextBox, 0, Configuracion::GetIpServidor(), (1024-200)/2, 250, 1);
}

MenuUnirse::~MenuUnirse()
{
    Graficos::Api::DestruirTextBox(textBox);
    Graficos::Api::DestruirImagen(imagenTextBox);
}

void
MenuUnirse::Continuar()
{
    if ( !menuHijo || !menuPadre)
    {
        throw Error::ExcepcionLocalizada("Interfaz::MenuUnirse::Continuar()");
    }
    
    Logica::Api::Finalizar();
    Red::Api::Finalizar();
    
    delete menuHijo ;
    menuHijo = NULL ;
    menuPadre->Continuar();   
}

void
MenuUnirse::Activar()
{
    
}

void
MenuUnirse::Desactivar()
{
    Menu::Desactivar();
    Graficos::Api::DesactivarTextBox(textBox);
}

void
MenuUnirse::ClickBoton4()
{    
    Configuracion::SetIpServidor( Graficos::Api::GetTextoTextBox(textBox) );    
    Red::Api::Inicializar( Red::Api::MODO_CLIENTE );    
    Desactivar();
    Menu::ClickBoton4();
    menuHijo = new MenuPartida(this);
    Api::SetMenuActual(menuHijo);
}

void
MenuUnirse::ClickBoton5()
{
    Menu::ClickBoton5();
    menuPadre->Continuar();
}

}
