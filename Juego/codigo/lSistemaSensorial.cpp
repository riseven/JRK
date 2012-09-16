#include "uWinDefs.h"
#include "lApi.h"
#include "lSistemaSensorial.h"


using namespace Logica;
using namespace Util;

//------------------------------- ctor ----------------------------------------
//-----------------------------------------------------------------------------
 SistemaSensorial:: SistemaSensorial(Unidad* unidad,
                                           double tiempoMemoria):unidad(unidad),
                                                            tiempoMemoria(tiempoMemoria)
                                                          
{}

void SistemaSensorial::CrearNuevoRegistro(Unidad* enemigo)
{
  if (mapa.find(enemigo) == mapa.end())
  {
    mapa[enemigo] = Memoria();
  }
}


void SistemaSensorial::EliminarDeMemoria(Unidad* uni)
{
  MapaDeMemoria::iterator record = mapa.find(uni);
  
  if (record != mapa.end())
  {
    mapa.erase(record);
  }
}

void SistemaSensorial::Olvidar( Unidad* uni )
{
  MapaDeMemoria::iterator record = mapa.find(uni);
  
  if (record != mapa.end())
  {
    mapa[uni].tiempoRegistro=0;
  }
}
  

void SistemaSensorial::ActualizarFuenteDeSonido(Unidad* fuenteSonido)
{
  //si no somos nosotros mismos
  if (unidad != fuenteSonido)
  {
    //Crear un nuevo registro
    CrearNuevoRegistro(fuenteSonido);

    Memoria& info = mapa[fuenteSonido];

    //Comprobar si hay linea de disparo entre ambos
    if ( !Api::HayDureza(unidad->getPosicion(), fuenteSonido->getPosicion()) )
    {
      info.disparable = true;
      
     //actualizar posicion
      info.posicion = fuenteSonido->getPosicion();
    }
    else
    {
      info.disparable = false;
    }
    
    //Momento en el que se sintió
    info.tiempoRegistro = (double)Util::GetTiempoActual();
  }
}


void SistemaSensorial::ActualizarVision()
{
  
  //Buscamos todas las unidades, OPTIMIZAR para buscar sólo las que caigan en la vista del agente, utilizando un grid
  Api::BuscarUnidadesBegin(unidad,0);
  while(! Api::BuscarUnidadesEnd())
  {
      Unidad * actual = Api::GetUnidad(Api::BuscarUnidadesNext());
      
      //Hacerlo parte de la memoria, sino estaba ya
      CrearNuevoRegistro(actual);

      //datos de la unidad
      Memoria& info = mapa[actual];

      //Mirar si hay linea de tiro entre ambos
      if (!Api::HayDureza(unidad->getPosicion(), actual->getPosicion()))
      {
      	//Siempre que haya linea de tiro, aunque no sea visible, se actualiza esta variable
      	//para ahorrar cálculos
        info.disparable = true; 

        //Ahora comprobar si entra en el campo de visión y en el rango de vision
        //de la unidad (FOV) y (ROV)
        if (estaElSegundoEnFOVDelPrimero(unidad->getPosicion(),
                                 unidad->getDireccion(),
                                 actual->getPosicion(),
                                  unidad->getFOV())
            && Vec2DDistanciaCuad(unidad->getPosicion(),actual->getPosicion())<=unidad->getROVCuad() )
        {
        	//Como se ve, se actualiza el registro
          info.tiempoRegistro     = Util::GetTiempoActual();
          info.posicion = actual->getPosicion();
          info.tiempoUltimaVezVisto    = info.tiempoRegistro;

          //Si en la comprobación anterior anterior no era visible
          if (info.esVisible == false)
          {
            info.esVisible           = true;
            //Acaba de aparecer
            info.tiempoPrimeraVezVisto = info.tiempoRegistro;
            info.tiempoSiendoVisible    = 0;
          
          }
          else{ //Si sigue siendo visible actualizar el tiempo que lleva siendolo
          
             info.tiempoSiendoVisible = info.tiempoUltimaVezVisto - info.tiempoPrimeraVezVisto;
          }
          
        }

        else
        {
          info.esVisible = false;         
        }
      }
      else
      {
        info.disparable = false;
        info.esVisible = false;
      }
  }//Siguiente
}


// Lista de enemigos recordados
std::list<Unidad*> 
SistemaSensorial::GetListaRecordados()const
{
  std::list<Unidad*> enemigos;

  double tiempo = Util::GetTiempoActual();

  MapaDeMemoria::const_iterator registroActual = mapa.begin();
  for (registroActual; registroActual!=mapa.end(); ++registroActual)
  {

    //Si el tiempo en el que se guardó el registro no supera el tiempo que la unidad es capaz de memorizarlo,
    //recogerlo
    if ( (tiempo - registroActual->second.tiempoRegistro) <= tiempoMemoria)
    {
    	//Guardamos en la lista la clave del mapa, es decir, la referncia a la unidad
      enemigos.push_back(registroActual->first);
    }
  }

  return enemigos;
}

//Devuelve si la unidad enemiga es disparable, buscando en la memoria
bool SistemaSensorial::esDisparable(Unidad* enemigo)const
{
  MapaDeMemoria::const_iterator it = mapa.find(enemigo);
 
  if (it != mapa.end())
  {
    return it->second.disparable;
  }

  return false;
}

//Devuelve si el enemigo es visible, buscando en la memoria
bool  SistemaSensorial::esVisible(Unidad* enemigo)const
{
  MapaDeMemoria::const_iterator it = mapa.find(enemigo);
 
  if (it != mapa.end())
  {
    return it->second.esVisible;
  }

  return false;
}

//Devuelve la posicion donde se recuerda al enemigo
Vector2D  SistemaSensorial::GetPosicionRecordada(Unidad* enemigo)const
{
  MapaDeMemoria::const_iterator it = mapa.find(enemigo);
 
  if (it != mapa.end())
  {
    return it->second.posicion;
  }

  //throw std::runtime_error("< SistemaSensorial::GetLastRecordedPositionOfOpponent>: Attempting to get position of unrecorded bot");
}

//Devuelve el tiempo que fué visible una unidad
double  SistemaSensorial::GetTiempoSiendoVisible(Unidad* enemigo)const
{
  MapaDeMemoria::const_iterator it = mapa.find(enemigo);
 
  if (it != mapa.end())
  {
    return it->second.tiempoSiendoVisible;
  }

  return 0;
}

//Tiempo que un enemigo lleva sin aparecer
double SistemaSensorial::GetTiempoSinHaberSidoVisto(Unidad* enemigo)const
{
  MapaDeMemoria::const_iterator it = mapa.find(enemigo);
 
  if (it != mapa.end())
  {
    return Util::GetTiempoActual() - it->second.tiempoUltimaVezVisto;
  }

  return MaxDouble;
}

//Tiempo desde que se sintió por última vez
double  SistemaSensorial::GetTiempoDesdeQueSeSintio(Unidad* enemigo)const
{
  MapaDeMemoria::const_iterator it = mapa.find(enemigo);
 
  if (it != mapa.end() )//&& it->second.esVisible)
  {
    return Util::GetTiempoActual() - it->second.tiempoRegistro;
  }

  return 0;
}
