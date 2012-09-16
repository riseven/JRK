#include <allegro.h>
#include <alleggl.h>
#include <string>
#include "iApi.h"
#include "iIntMinimapa.h"
#include "gApi.h"
#include "iCamara.h"
#include "eExcepcionLocalizada.h"
#include "iUnidad.h"


namespace Interfaz 
{

/* Crea el interfaz del modulo de minimapa del panel
 * Este modulo se encarga de transformar la coordenada elegida a una coordenada
 * del mapa principal, centrando la cámara donde se haga click.
 */
IntMinimapa::IntMinimapa()
{
    posX=16; posY=560;     //Posicion del minimapa
    ancho=200; alto=200;   //Anchura y altura
    imgMinimapa=-1; btnMinimapa=-1;
    btnFlag=false;

    imgMinimapaTrans = -1;
    bdrPulsado = false ;
    
    for ( int i = 0 ; i < 4 ; i++ )
    {
        segmentosVision[i] = Graficos::Api::CrearSegmento(1,0,1,1,0,0,0,0,30);
        Graficos::Api::SetClippingSegmento(segmentosVision[i], 16, 560, 216, 760);
    }
}

//Destruye el minimapa
IntMinimapa::~IntMinimapa()
{
    try
    {
        Graficos::Api::DestruirBoton(btnMinimapa);
        Graficos::Api::DestruirImagen(imgMinimapa);
        Graficos::Api::DestruirImagen(imgMinimapaTrans);
        Graficos::Api::DestruirSprite(sprMinimapaTrans);
        imgMinimapa=-1; btnMinimapa=-1;
        for ( int i = 0 ; i < 4 ; i++ )
        {
            Graficos::Api::DestruirSegmento(segmentosVision[i]);
        }
        
        // Elimino las unidades pintadas en el ultimo frame.
        for(list <int >::iterator i = miniUnidades.begin(); i != miniUnidades.end(); ++i)
        Graficos::Api::DestruirRectangulo(*i);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::IntMinimapa::~IntMinimapa()").Combinar(ex);
    }
}

//Comprueba si se ha hecho click sobre el minimapa
//y si es el caso se transforma la coordenada
//y se mueve la cámara
void
IntMinimapa::Actualizar()
{
    try
    {   
        float x = mouse_x ;
        float y = mouse_y ;        
        TransCoorAPantalla(x, y);

        if ( mouse_x >= posX && mouse_y >= posY && mouse_x < posX+ancho && mouse_y < posY+alto )
        {
            if ( mouse_b & 1 )
            {
                //Muevo la camara a la nueva posicion
                Api::GetJuego()->GetCamara()->SetPosicionDeseada(x,y);     
                Api::GetJuego()->GetCamara()->ForzarPosicion() ;
                
            }
            if ( mouse_b & 2 && !bdrPulsado)
            {
                bdrPulsado = true ;
                float wx = x * Graficos::Api::GetEscalaTerreno();
                float wz = y * Graficos::Api::GetEscalaTerreno();
                UnidadMultiSeleccionable::RespuestaInteligenteTerreno(wx,wz);
            }
        }
        
        if ( !(mouse_b &2) )
        {
            bdrPulsado = false ;   
        }
        
        
        // Pinto el trapezoide de vision
        int coor[4];
        coor[0] = Graficos::Api::CoordenadasVentana::ArribaIzquierda ;
        coor[1] = Graficos::Api::CoordenadasVentana::ArribaDerecha ;
        coor[2] = Graficos::Api::CoordenadasVentana::AbajoDerecha ;
        coor[3] = Graficos::Api::CoordenadasVentana::AbajoIzquierda ;
        
        float vx[4], vy[4] ;
        
        // Obtengo las coordenadas de las esquinas
        for ( int i = 0 ; i < 4 ; i++ )
        {
            Util::Vector3D pos = Graficos::Api::GetCoordenadas2Dto3D(coor[i]);
            
            vx[i] = pos.GetX();
            vy[i] = pos.GetZ();
            
            vx[i] /= Graficos::Api::GetEscalaTerreno() ;
            vy[i] /= Graficos::Api::GetEscalaTerreno() ;
            
            TransCoorAMinimapa(vx[i], vy[i]);
        }
        
        // Pinto las lineas
        Graficos::Api::MoverSegmento(segmentosVision[0], vx[0], vy[0], vx[1], vy[1]);
        Graficos::Api::MoverSegmento(segmentosVision[1], vx[1], vy[1], vx[2], vy[2]);
        Graficos::Api::MoverSegmento(segmentosVision[2], vx[2], vy[2], vx[3], vy[3]);
        Graficos::Api::MoverSegmento(segmentosVision[3], vx[3], vy[3], vx[0], vy[0]); 
        
        // Pinto las unidades
        PintaUnidades();
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::IntMinimapa::Actualizar()").Combinar(ex);
    }
}


// Translada una coordenada del minimapa a la correspondiente en el mapa general
void 
IntMinimapa::TransCoorAPantalla(float &x, float &y)
{
    try
    {
        x -= posX; 
        y -= posY;
        
        x = (( x / (ancho*1.0) ) * Graficos::Api::GetAnchoTerreno()) ;
        y = (( y / (alto*1.0) ) * Graficos::Api::GetAltoTerreno()) ;
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::IntMinimapa::TrasladaCoordenada(int &, int &)").Combinar(ex);
    }
}

void
IntMinimapa::TransCoorAMinimapa(float &x, float &y)
{    
    x = (( x * (ancho*1.0) ) / Graficos::Api::GetAnchoTerreno()) ;
    y = (( y * (alto*1.0) ) / Graficos::Api::GetAltoTerreno()) ;

    x += posX; 
    y += posY;    
}


//Establece un minimapa
bool
IntMinimapa::SetMinimapa(string nombre)
{
    try
    {
        imgMinimapa = Graficos::Api::CrearImagen(nombre);
        imgMinimapaTrans = Graficos::Api::CrearImagen("Graficos//minimapaTrans.bmp");
        Graficos::Api::SetImagenMasked(imgMinimapaTrans, true);
        sprMinimapaTrans = Graficos::Api::CrearSprite(imgMinimapaTrans,posX,posY,55);

        btnMinimapa = Graficos::Api::CrearBoton(imgMinimapa,-1,0,"",posX,posY,2);   
        Graficos::Api::OnClickBoton(btnMinimapa, &btnFlag);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::IntMinimapa::SetMinimapa(string)").Combinar(ex);
    }
}

void
IntMinimapa::PintaUnidades()
{
   float x=-1,y=-1,z=-1;
   int tam;
 

      for(list <int >::iterator i = miniUnidades.begin(); i != miniUnidades.end(); ++i)
         Graficos::Api::DestruirRectangulo(*i);
         
      miniUnidades.clear();
      
       try
       {
           for ( vector <Unidad *>::iterator i = Api::unidades.begin() ; i != Api::unidades.end() ; ++i )
           {
                if ( (*i)->GetNumJugador() != -1 )
                {
                   (*i)->GetPos(x,y,z);
                   int color = Api::GetJugador((*i)->GetNumJugador())->GetColor();
                   
                   //El tamaño del rectangulo depende dl tipo d unidad
                   switch ((*i)->GetTipo())
                   {
                      case Red::Protocolo::Unidad::Barracones: tam=5; break;
                      default: tam=3;
                   }
                   
                   int num = Graficos::Api::CrearRectangulo(tam, tam, Jugador::GetColorRojo(color), Jugador::GetColorVerde(color), Jugador::GetColorAzul(color), 1, true, posX + (int)(x/Graficos::Api::GetEscalaTerreno()), posY + (int)(z/Graficos::Api::GetEscalaTerreno()), 4);
                   miniUnidades.push_back(num);
                }
           }
       }
       catch ( Error::Excepcion &ex )
       {
           throw Error::ExcepcionLocalizada("Interfaz::IntMinimapa::PintaUnidades()").Combinar(ex);
        }
}

void
IntMinimapa::Activar()
{
   Graficos::Api::ActivarSprite(sprMinimapaTrans);
   Graficos::Api::ActivarBoton(btnMinimapa);
   
   for(list <int >::iterator i = miniUnidades.begin(); i != miniUnidades.end(); ++i)
      Graficos::Api::ActivarRectangulo(*i);    
      
   for(int i = 0; i < 4; i++)
      Graficos::Api::ActivarSegmento(segmentosVision[i]);      
}


void
IntMinimapa::Desactivar()
{
   Graficos::Api::DesactivarSprite(sprMinimapaTrans);
   Graficos::Api::DesactivarBoton(btnMinimapa);   
   
   for(list <int >::iterator i = miniUnidades.begin(); i != miniUnidades.end(); ++i)
      Graficos::Api::DesactivarRectangulo(*i);
      
   for(int i = 0; i < 4; i++)
      Graficos::Api::DesactivarSegmento(segmentosVision[i]);
}

}
