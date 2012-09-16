#include <cstdlib>
#include "iIntSeleccion.h"
#include "gApi.h"
#include "iApi.h"
#include "gBarraVida.h"
#include "eExcepcionLocalizada.h"
#include "iInterfazPanel.h"


#include <iostream>

using namespace std;

namespace Interfaz 
{
   
IntSeleccion::IntSeleccion() 
{   
    try 
    {         
       numRetratosActivos = 0;
       imagenBarraProgreso = Graficos::Api::CrearImagen("Graficos\\BarraProgreso.bmp");
       imagenCuadroProgreso = Graficos::Api::CrearImagen("Graficos\\CuadroProgreso.bmp");
      // imagenBarraVida = Graficos::Api::CrearImagen("Graficos\\BarraVida.bmp");
      // imagenCuadroVida = Graficos::Api::CrearImagen("Graficos\\CuadroVida.bmp");
       
       barraProgreso = Graficos::Api::CrearBarraProgreso(
           imagenBarraProgreso,
           imagenCuadroProgreso,
           1, 1, 261, 725, 15);
   
       xIni=526;
       yIni=605;
       //int imgSelecUnidadFondo = Graficos::Api::CrearImagen("Graficos\\selecUnidadFondo.bmp");
       int imgSelecUnidad = Graficos::Api::CrearImagen("Graficos\\selecUnidad.bmp");
       
       Graficos::Api::DesactivarBarraProgreso(barraProgreso);
      
       for(int i = 0; i < 12; i++)
       {
          btUnidades[i]=Graficos::Api::CrearBoton(imgSelecUnidad,imgSelecUnidad,0,"",xIni+(i%4)*52,yIni+(i/4)*51,2);         
          barraVida[i] =  Graficos::Api::CrearBarraVida(44, 5, 1, xIni + (i%4)*52 + 2, yIni + (i/4)*51 + 40, 5);
          Graficos::Api::DesactivarBoton(btUnidades[i]);
          Graficos::Api::DesactivarBarraVida(barraVida[i]);         
         
          onClicks[i] = false;
          OnClickBoton(i, &(onClicks[i]));
          imgUnidades[i] = -1;
       }
   }
   catch ( Error::Excepcion &ex )
   {
      throw Error::ExcepcionLocalizada("Interfaz::IntSeleccion::IntSeleccion()").Combinar(ex);
   }
}

IntSeleccion::~IntSeleccion()
{
    Graficos::Api::DestruirImagen(imagenBarraProgreso);
    Graficos::Api::DestruirImagen(imagenCuadroProgreso);
    Graficos::Api::DestruirBarraProgreso(barraProgreso);

    //Destruyo los 12 botones
    for(int i=0; i<12; i++)
    {
      Graficos::Api::DestruirBoton(btUnidades[i]);
      Graficos::Api::DestruirBarraVida(barraVida[i]);
      if ( imgUnidades[i] != -1 )
        Graficos::Api::DestruirImagen(imgUnidades[i]);
    }
}

void
IntSeleccion::SetVisibleBarraProgreso(bool visible)
{
    if ( visible )
    {
        Graficos::Api::ActivarBarraProgreso(barraProgreso);
    }
    else
    {
        Graficos::Api::DesactivarBarraProgreso(barraProgreso);
    }
}

void
IntSeleccion::SetPorcentajeBarraProgreso(int porcentaje)
{
    Graficos::Api::SetPorcentajeBarraProgreso(barraProgreso, porcentaje);
}

//Asocia cada boton con un booleano que controla su estado
void
IntSeleccion::OnClickBoton(int numBoton, bool *OnClickFlag)
{
    try
    {
        onClicks[numBoton] = OnClickFlag ;
        Graficos::Api::OnClickBoton(btUnidades[numBoton], OnClickFlag);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::IntSeleccion::OnClickBoton(int, bool *)").Combinar(ex);
    }
}


void
IntSeleccion::Actualizar()
{
      for( int i=0; i < 12; i++ )
      {
         if( onClicks[i] )
         {
            if( key[KEY_LSHIFT] || key[KEY_RSHIFT] )
              UnidadMultiSeleccionable::DeseleccionarUnidadOrden( i );
            else
              UnidadMultiSeleccionable::SeleccionarUnidadOrden( i );
            onClicks[i]=false;
            break;
         }
      }
}

void
IntSeleccion::ActualizarSeleccion(list <UnidadMultiSeleccionable *> listaUnidades)
{
      int j=0;
      
      for (list <UnidadMultiSeleccionable *>::iterator i = listaUnidades.begin(); i != listaUnidades.end() && (j < 12); ++i, j++ )
      {
         string strImagen =  (*i)->GetPathRecursos() + "minipreview.bmp";
         
         if( imgUnidades[j] != -1)
            Graficos::Api::DestruirImagen(imgUnidades[j]);

         imgUnidades[j] = Graficos::Api::CrearImagen(strImagen);
         Graficos::Api::CambiarFondoBoton(btUnidades[j], imgUnidades[j]);

         //Graficos::Api::SetPorcentajeBarraVida(barraVida[j], ... );

         Graficos::Api::ActivarBoton(btUnidades[j]);
         Graficos::Api::ActivarBarraVida(barraVida[j]);
      }
      
      numRetratosActivos = j;
      
      for(int i=j; i<12; i++)
      {
           Graficos::Api::DesactivarBoton(btUnidades[i]);
           Graficos::Api::DesactivarBarraVida(barraVida[i]);
           imgUnidades[i] = -1;
      }

      if(numRetratosActivos>0)
          Api::GetInterfazPanel()->GetIntUnidad()->CambiarImagenUnidad(listaUnidades.front());
}

void
IntSeleccion::Activar()
{
      for ( int i = 0; i < numRetratosActivos; i++ )
      {
         Graficos::Api::ActivarBoton(btUnidades[i]);     
         Graficos::Api::ActivarBarraVida(barraVida[i]);
      }
}

void
IntSeleccion::Desactivar()
{  
   for(int i = 0; i < 12; i++)
   {
      Graficos::Api::DesactivarBoton( btUnidades[i] );
      Graficos::Api::DesactivarBarraVida( barraVida[i] );
   }
}

}
