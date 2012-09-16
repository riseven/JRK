#include "iMenuConfiguracionVideo.h"
#include "gApi.h"
#include "iApi.h"
#include "eExcepcionLocalizada.h"
#include "iConfiguracion.h"

namespace Interfaz
{

//Crea menu de configuracion
MenuConfiguracionVideo::MenuConfiguracionVideo(Menu *menuPadre):
    Menu(menuPadre, "Configuracion de video", "", "", "", "", "Volver")
{
    fondoBotonMultiTextura = Graficos::Api::CrearImagen("Graficos\\FondoBotonOpcion.bmp");
    selecBotonMultiTextura = Graficos::Api::CrearImagen("Graficos\\SeleccionBotonOpcion.bmp");
    botonMultiTextura = Graficos::Api::CrearBoton(fondoBotonMultiTextura, selecBotonMultiTextura, 0, "Multitexturizado", 500, 200, 1);
    Graficos::Api::OnClickBoton(botonMultiTextura, &clickMultiTextura);
    Graficos::Api::SetFijoBoton(botonMultiTextura, Configuracion::GetMultiTextura() );
    
    botonDiaNoche = Graficos::Api::CrearBoton(fondoBotonMultiTextura, selecBotonMultiTextura, 0, "Dia / Noche", 500, 250, 1);
    Graficos::Api::OnClickBoton(botonDiaNoche, &clickDiaNoche);
    Graficos::Api::SetFijoBoton(botonDiaNoche, Configuracion::GetNoche() );

    botonEstaciones = Graficos::Api::CrearBoton(fondoBotonMultiTextura, selecBotonMultiTextura, 0, "Estaciones", 500, 300, 1);
    Graficos::Api::OnClickBoton(botonEstaciones, &clickEstaciones);
    Graficos::Api::SetFijoBoton(botonEstaciones, Configuracion::GetEstaciones() );


}

MenuConfiguracionVideo::~MenuConfiguracionVideo()
{
    Graficos::Api::DestruirImagen(fondoBotonMultiTextura);
    Graficos::Api::DestruirImagen(selecBotonMultiTextura);
    Graficos::Api::DestruirBoton(botonMultiTextura);
    
    Graficos::Api::DestruirBoton(botonDiaNoche);
    Graficos::Api::DestruirBoton(botonEstaciones);
}

void
MenuConfiguracionVideo::ClickBoton5()
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
MenuConfiguracionVideo::Actualizar()
{
    if ( clickMultiTextura )
    {
        Configuracion::SetMultiTextura( !Configuracion::GetMultiTextura() );
        Graficos::Api::SetFijoBoton(botonMultiTextura, Configuracion::GetMultiTextura() );
    }
    
    if ( clickDiaNoche )
    {
        Configuracion::SetNoche( !Configuracion::GetNoche() );
        Graficos::Api::SetFijoBoton(botonDiaNoche, Configuracion::GetNoche() );
    }
    
    if ( clickEstaciones )
    {
        Configuracion::SetEstaciones( !Configuracion::GetEstaciones() );
        Graficos::Api::SetFijoBoton(botonEstaciones, Configuracion::GetEstaciones() );
    }
    
    
    Menu::Actualizar();
}

}
