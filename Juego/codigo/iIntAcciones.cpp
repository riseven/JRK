#include "iIntAcciones.h"
#include "gApi.h"
#include "eExcepcionLocalizada.h"
#include "iApi.h"

namespace Interfaz {

// Construye el interfaz de las acciones usando 12 botones
// y situandolos en el panel
IntAcciones::IntAcciones()
{
    try
    {   
        xIni=780;
        yIni=591;
                                 
        imagenSeleccion=Graficos::Api::CrearImagen("Graficos\\accionFondo.bmp");
        for(int i=0; i<12; i++)
        {   
            imgTexto[i] = sprTexto[i] = -1;
            //Creamos el boton correspondiente con las imagenes anteriores
            btAcciones[i]=Graficos::Api::CrearBoton(imagenSeleccion,imagenSeleccion,0,"",xIni+(i%4)*56,yIni+(i/4)*56,2);
                                   
            //Inicializo hotkeys
            hotKey[i] = -1 ;
            hotKeyPulsada[i] = 0 ;
            onClicks[i] = NULL ;
            accionesActivas[i] = false;

            //ToolTips
            onMouseOver[i] = false; 
            imgToolTip[i] = sprToolTip[i] = -1;
            Graficos::Api::OnMouseOver(btAcciones[i], &onMouseOver[i]);

            SetVisible(i, false);
        }
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::IntAcciones::IntAcciones()").Combinar(ex);
    }
}

// Destruye los 12 botones de las acciones
IntAcciones::~IntAcciones()
{
    try
    {    
        Graficos::Api::DestruirImagen(imagenSeleccion);
        for(int i=0;i<12;i++)
        {
            Graficos::Api::DestruirBoton(btAcciones[i]);
            if( sprTexto[i] != -1 )
                Graficos::Api::DestruirSprite(sprTexto[i]);
            if( imgTexto[i] != -1 )
                Graficos::Api::DestruirImagen(imgTexto[i]);
            if( sprToolTip[i] != -1 )
                Graficos::Api::DestruirSprite(sprToolTip[i]);
            if( imgToolTip[i] != -1 )
                Graficos::Api::DestruirImagen(imgToolTip[i]);
        }
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::IntAcciones::~IntAcciones()").Combinar(ex);
    }
}

//Hace visible o invisible en el panel el boton indicado
void
IntAcciones::SetVisible(int numBoton, bool visible)
{
    try
    {
        if ( visible )
        {
            Graficos::Api::ActivarBoton(btAcciones[numBoton]);
        }
        else
        {
            Graficos::Api::DesactivarBoton(btAcciones[numBoton]);
            Graficos::Api::OnClickBoton(btAcciones[numBoton], NULL);
            Graficos::Api::SetFijoBoton(btAcciones[numBoton], false);
            hotKey[numBoton] = -1 ;

            if( imgTexto[numBoton] != -1 )
            {
                Graficos::Api::DestruirImagen(imgTexto[numBoton]);                
                imgTexto[numBoton] = -1;
            }

            if( sprTexto[numBoton] != -1 )
            {
                Graficos::Api::DestruirSprite(sprTexto[numBoton]);
                sprTexto[numBoton] = -1;
            }

            if( imgToolTip[numBoton] != -1 )
            {
                Graficos::Api::DestruirImagen(imgToolTip[numBoton]);
                imgToolTip[numBoton] = -1;
            }

            if( sprToolTip[numBoton] != -1 )
            {
                Graficos::Api::DestruirSprite(sprToolTip[numBoton]);
                sprToolTip[numBoton] = -1;
            }

        }
        
        accionesActivas[numBoton] = visible;
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::IntAcciones::SetVisible(int, bool)").Combinar(ex);
    }
}

// Cambia la imagen del boton indicado
void
IntAcciones::CambiarImagen(int numBoton, int handlerImagen)
{
    try
    {
        Graficos::Api::CambiarFondoBoton(btAcciones[numBoton], handlerImagen);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::IntAcciones::CambiarImagen(int, int)").Combinar(ex);
    }
}

//Asocia cada boton con un booleano que controla su estado
void
IntAcciones::OnClickBoton(int numBoton, bool *OnClickFlag)
{
    try
    {
        onClicks[numBoton] = OnClickFlag ;    
        Graficos::Api::OnClickBoton(btAcciones[numBoton], OnClickFlag);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::IntAcciones::OnClickBoton(int, bool *)").Combinar(ex);
    }
}

//Asocia una tecla con una accion
void
IntAcciones::AsociarHotKey(int numBoton, int codigoTecla)
{
    hotKey[numBoton] = codigoTecla ;
}

void
IntAcciones::SetFijoBoton(int numBoton, bool fijo)
{
    Graficos::Api::SetFijoBoton(btAcciones[numBoton], fijo);
}

//Comprueba el estado de cada tecla asociada
void
IntAcciones::Actualizar()
{
    if ( !Api::GetJuego()->Escribiendo() )
    {
        for ( int i = 0 ; i < 12 ; i++ )
        {
            if ( hotKey[i] != -1 )
            {
                //Si no esta pulsada
                if ( hotKeyPulsada[i] == 2 )
                {
                    hotKeyPulsada[i] = 0 ;
                    *(onClicks[i]) = false ;
                }
                //Si esta pulsada
                if ( hotKeyPulsada[i] == 1 )
                {
                    if ( !key[hotKey[i]] )
                    {
                        hotKeyPulsada[i] = 2 ;
                        *(onClicks[i]) = true;
                    }
                }
    
                if ( hotKeyPulsada[i] == 0 && key[hotKey[i]] )
                {
                    hotKeyPulsada[i] = 1 ;
                }
            }
            
            //Comprobamos si se muestra algun toolTip
            if ( sprToolTip[i] != -1 )
            {
                if ( onMouseOver[i] )
                    Graficos::Api::ActivarSprite(sprToolTip[i]);
                else
                    Graficos::Api::DesactivarSprite(sprToolTip[i]);            
            }
        }
    }
}

void
IntAcciones::Activar()
{
  for(int i = 0; i < 12; i++)
      if(accionesActivas[i])
         Graficos::Api::ActivarBoton(btAcciones[i]);
}

void
IntAcciones::Desactivar()
{
   for(int i = 0; i < 12; i++)
      Graficos::Api::DesactivarBoton(btAcciones[i]);
}

void 
IntAcciones::SetTextoBoton(int numBoton, string texto)
{
    if ( imgTexto[numBoton] != -1 )
    {
        Graficos::Api::DestruirSprite(imgTexto[numBoton]);
    }
    if ( sprTexto[numBoton] != -1 )
    {
        Graficos::Api::DestruirSprite(sprTexto[numBoton]);
    }
    imgTexto[numBoton] = Graficos::Api::CrearImagen(0,texto, 128, 128, 255);
    sprTexto[numBoton] = Graficos::Api::CrearSprite(imgTexto[numBoton], xIni+(numBoton%4)*56 + 4, yIni+(numBoton/4)*56 + 38, 3);
}

void
IntAcciones::SetTextoToolTip(int toolTip, string texto)
{
    imgToolTip[toolTip] = Graficos::Api::CrearImagen(0,texto, 128, 128, 255);
    sprToolTip[toolTip] = Graficos::Api::CrearSprite(imgToolTip[toolTip], xIni, yIni , 3);
    Graficos::Api::MoverSprite(sprToolTip[toolTip], SCREEN_W - Graficos::Api::GetAnchoSprite(sprToolTip[toolTip]) - 5, yIni - Graficos::Api::GetAltoSprite(sprToolTip[toolTip]) - 20);
    Graficos::Api::DesactivarSprite(sprToolTip[toolTip]);
}

}
