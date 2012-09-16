#include "iIntUnidad.h"
#include "rProtocolo.h"
#include "iUnidadUniSeleccionable.h"
#include "gApi.h"
#include "iApi.h"
#include "iUnidad.h"
#include "eExcepcionLocalizada.h"
#include <string>
#include "iBarracones.h"

namespace Interfaz {

IntUnidad::IntUnidad()
{
   try
   {
      posX = 315;
      posY = 615;
      imgUnidad = -1;
      btnUnidad = -1;
      onClick = false ;
      imgFrente = Graficos::Api::CrearImagen("Graficos\\FrenteUnidad.bmp");
      Graficos::Api::SetImagenMasked(imgFrente,true);
      btnUnidad = Graficos::Api::CrearBoton(imgFrente, imgFrente, 0, "", posX, posY, 3);     
      Graficos::Api::OnClickBoton( btnUnidad, &onClick);
      Graficos::Api::DesactivarBoton( btnUnidad );

   }
   catch ( Error::Excepcion &ex )
   {
      throw Error::ExcepcionLocalizada("Interfaz::IntUnidad::IntUnidad()").Combinar(ex);
   }
   
}

IntUnidad::~IntUnidad()
{
   if(imgUnidad != -1)
      Graficos::Api::DestruirImagen( imgUnidad );
            
   if(btnUnidad != -1)
      Graficos::Api::DestruirBoton( btnUnidad );
      
   Graficos::Api::DestruirImagen( imgFrente );
}

void
IntUnidad::CambiarImagenUnidad(UnidadSeleccionable *unidad)
{
   try
   {  
      string strImagen =  unidad->GetPathRecursos() + "preview.bmp";
      cout<<"->"<<strImagen<<"<-"<<endl;
      if(imgUnidad != -1)
         Graficos::Api::DestruirImagen(imgUnidad);
      
      imgUnidad = Graficos::Api::CrearImagen( strImagen );

      Graficos::Api::CambiarFondoBoton(btnUnidad, imgUnidad);

      Graficos::Api::ActivarBoton(btnUnidad);   
          
      ultUnidad = unidad;  
   }
   catch ( Error::Excepcion &ex )
   {
      throw Error::ExcepcionLocalizada("Interfaz::IntUnidad::CambiarImagenUnidad()").Combinar(ex);
   }
}

void
IntUnidad::Activar()
{
   if(btnUnidad != -1)
      Graficos::Api::ActivarBoton(btnUnidad);
}

void
IntUnidad::Desactivar()
{     
   if(btnUnidad != -1)
      Graficos::Api::DesactivarBoton(btnUnidad);
}

void
IntUnidad::Actualizar()
{
   if( onClick )
   {
      float px, py, pz;
      ultUnidad->GetPos(px, py, pz);      
      ultUnidad->Seleccionar();
      Api::GetJuego()->GetCamara()->SetPosicionDeseada(px / Graficos::Api::GetEscalaTerreno(), pz / Graficos::Api::GetEscalaTerreno());
      Api::GetJuego()->GetCamara()->ForzarPosicion();
   }
}

}
