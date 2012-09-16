#include "iIntBarraSuperior.h"
#include "gApi.h"
#include "iApi.h"
#include "eExcepcionLocalizada.h"

#include <iostream>

using namespace std;

namespace Interfaz
{
   
IntBarraSuperior::IntBarraSuperior()
{
   try 
   {
      posXDinero = 930;
      posYDinero = 12;
      posXPoblacion = 832;
      posYPoblacion = 12;

      imgDinero = Graficos::Api::CrearImagen(0, "0");
      sprDinero = Graficos::Api::CrearSprite( imgDinero, posXDinero, posYDinero, 3 );

      imgPoblacion = Graficos::Api::CrearImagen(0, "0 / 0");
      sprPoblacion = Graficos::Api::CrearSprite(imgPoblacion, posXPoblacion, posYPoblacion, 3);

      imgBarraSuperior = Graficos::Api::CrearImagen( "Graficos//barraSuperior.bmp" );
      Graficos::Api::SetImagenMasked( imgBarraSuperior, true );
      sprBarraSuperior = Graficos::Api::CrearSprite( imgBarraSuperior, 0, 0, 2 );

      ultCantidadDinero = 0; 
      ultPoblacion = 0;
      ultPoblacionMax = 0;

      imgMenuFrente = Graficos::Api::CrearImagen("Graficos\\btnMenuFrente.bmp");
      Graficos::Api::SetImagenMasked(imgMenuFrente, true);
      imgMenuFondo = Graficos::Api::CrearImagen("Graficos\\btnMenuFondo.bmp");
      Graficos::Api::SetImagenMasked(imgMenuFondo, true);

      btnMenu = Graficos::Api::CrearBoton(imgMenuFondo, imgMenuFrente, 0, "Menu", 20, 5, 3);      
      Graficos::Api::OnClickBoton(btnMenu, &onClickMenu);
      onClickMenu = false;
    
   }
   catch ( Error::Excepcion &ex )
   {
      throw Error::ExcepcionLocalizada("Interfaz::IntBarraSuperior::BarraSuperior()").Combinar(ex);
   }
}

IntBarraSuperior::~IntBarraSuperior()
{
   try
   {
      Graficos::Api::DestruirSprite( sprBarraSuperior );
      Graficos::Api::DestruirSprite( sprDinero );
      Graficos::Api::DestruirSprite( sprPoblacion );
      Graficos::Api::DestruirImagen( imgBarraSuperior );
      Graficos::Api::DestruirImagen( imgDinero );
      Graficos::Api::DestruirImagen( imgPoblacion );
      
      Graficos::Api::DestruirImagen( imgMenuFrente );
      Graficos::Api::DestruirImagen( imgMenuFondo );
      Graficos::Api::DestruirBoton( btnMenu );
   }
   catch ( Error::Excepcion &ex )
   {
       throw Error::ExcepcionLocalizada("Interfaz::IntBarraSuperior::~IntBarraSuperior()").Combinar(ex);
   }
}

void
IntBarraSuperior::SetDineroActual(int dinero)
{
      char c_dinero[20];
 
      if ( dinero != ultCantidadDinero )
      {
          ultCantidadDinero = dinero ;

          sprintf(c_dinero,"%8d", dinero);
          
          
          
          string mensaje = c_dinero ;
          
          Graficos::Api::DestruirSprite(sprDinero);
          Graficos::Api::DestruirImagen(imgDinero);
          imgDinero = Graficos::Api::CrearImagen(0, mensaje);
          sprDinero = Graficos::Api::CrearSprite( imgDinero, posXDinero, posYDinero, 3 );   
    
          if(!Api::GetInterfazPanel()->EsVisible())
          {
             Graficos::Api::DesactivarSprite( sprDinero );
          }
      }
}

void
IntBarraSuperior::Actualizar()
{
    if( onClickMenu )
    {
        Api::GetJuego()->SalirPartida();
    }
}

void
IntBarraSuperior::Activar()
{
   Graficos::Api::ActivarSprite(sprBarraSuperior);
   Graficos::Api::ActivarSprite(sprDinero);
   Graficos::Api::ActivarSprite(sprPoblacion);
   Graficos::Api::ActivarBoton(btnMenu);   
}

void
IntBarraSuperior::Desactivar()
{
   Graficos::Api::DesactivarSprite(sprBarraSuperior);
   Graficos::Api::DesactivarSprite(sprDinero);
   Graficos::Api::DesactivarSprite(sprPoblacion);
   Graficos::Api::DesactivarBoton(btnMenu);
}

void
IntBarraSuperior::SetPoblacionActual(int poblacion, int max)
{
      char c_poblacion[20];
 
      if ( poblacion != ultPoblacion || max != ultPoblacionMax )
      {
          ultPoblacion = poblacion ;
          ultPoblacionMax = max ;

          sprintf(c_poblacion,"%d / %d", poblacion, max);
          
          Graficos::Api::DestruirSprite(sprPoblacion);
          Graficos::Api::DestruirImagen(imgPoblacion);
          imgPoblacion = Graficos::Api::CrearImagen(0, c_poblacion);
          sprPoblacion = Graficos::Api::CrearSprite( imgPoblacion, posXPoblacion, posYPoblacion, 3 );   
    
          if(!Api::GetInterfazPanel()->EsVisible())
          {
             Graficos::Api::DesactivarSprite( sprPoblacion );
          }
      }
}

}
