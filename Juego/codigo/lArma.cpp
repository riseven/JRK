#include "lArma.h"
#include "uWinDefs.h"
#include "gApi.h"
#include "lApi.h"


using namespace Logica;

Arma::Arma(unsigned int Tipo,
                             double       frecuencia,
                             double       distancia,
                             double       velocidad,
                             double       tiempoCarga,
                             Unidad*      uni):

                                 tipo(Tipo),
                                 unidad(uni),
                                 frecuenciaDisparo(frecuencia),
                                 maximaDistancia(distancia),
                                 velocidadMaximaProyectiles(velocidad),
                                 tiempoDeCarga(tiempoCarga)
{  
                                 tiempoEstaraDisponible = Util::GetTiempoActual();
}



//Calcula si el disparo desde el origen hasta la unidad destino estará ocluido
//por terreno o por edificios ( se supone altura fija de 2)
bool     Arma::estaDisparoOcluido( const Unidad* uniDestino )
{
         Util::Vector2D origen2D = unidad->getPosicion();
         Util::Vector3D origen3D(origen2D.getX(),origen2D.getY(),Api::GetAlturaMapa(origen2D.getX(),origen2D.getY()));
         
         Util::Vector2D destino2D = uniDestino->getPosicion();
         Util::Vector3D destino3D(destino2D.getX(),destino2D.getY(),Api::GetAlturaMapa(destino2D.getX(),destino2D.getY()));
         
         Util::Vector3D vect = destino3D - origen3D;
         
         vect.Normalizar();
         
         vect = vect * (Graficos::Api::GetEscalaTerreno()/2.0);
         
         //Voy avanzando en linea recta a pequeños saltos
         for ( Util::Vector3D inicio ; inicio < destino3D ; inicio += vect )
         {
             //Buscamos colisiones con edificios y con terreno
             ;
         }
         
}
