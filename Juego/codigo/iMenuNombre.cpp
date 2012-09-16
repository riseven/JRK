#include "iMenuNombre.h"
#include "iConfiguracion.h"
#include "gApi.h"
#include "eExcepcionFichero.h"
#include "eExcepcionParametro.h"
#include "eExcepcionLocalizada.h"


namespace Interfaz
{

MenuNombre::MenuNombre(Menu *menuPadre):
    Menu(menuPadre, "Nombre", "", "", "", "Aceptar", "Volver")
{
    try
    {
        aceptado = false ;
            
        // Creo el textbox
        imagenTextBox = Graficos::Api::CrearImagen("Graficos\\TextBoxNick.bmp");
        handlerTextBox = Graficos::Api::CrearTextBox(imagenTextBox, 0, Configuracion::GetNombreJugador(), (1024-200)/2, 250, 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::MenuNombre::MenuNombre(Menu *)").Combinar(ex);
    }
}

MenuNombre::~MenuNombre()
{
    try
    {
        aceptado = false ;
        Graficos::Api::DestruirImagen(imagenTextBox);
        Graficos::Api::DestruirTextBox(handlerTextBox);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::MenuNombre::~MenuNombre()").Combinar(ex);
    }
}

bool
MenuNombre::Aceptado()
{
    return aceptado ;
}

void
MenuNombre::ClickBoton4()
{
    try
    {
        Menu::ClickBoton4();        
        aceptado = true ;
        Configuracion::SetNombreJugador( Graficos::Api::GetTextoTextBox(handlerTextBox) );
        menuPadre->Continuar();
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::MenuNombre::ClickBoton4()").Combinar(ex);
    }
}

void
MenuNombre::ClickBoton5()
{
    try
    {
        Menu::ClickBoton5();        
        aceptado = false ;
        menuPadre->Continuar();
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::MenuNombre::ClickBoton5()").Combinar(ex);
    }
}

}
