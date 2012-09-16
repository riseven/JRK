#include "lApi.h"
#include "lSistemaMotor.h"
#include "lUnidadDinamica.h"
#include "uUtils.h"

#include <cassert>


using std::string;
using std::vector;

using namespace Logica;
using namespace Util;


//------------------------- ctor -----------------------------------------
//
//------------------------------------------------------------------------
SistemaMotor::SistemaMotor(UnidadDinamica* agente):
                                  
             unidad(agente),
             banderas(ninguno),
             pesoSeparacion(8.0),
             pesoDeambular(1.0),
             pesoEvitarDurezas(3.0),
             distanciaVision(15.0),
             longitudSensores(25.0),
             sensores((int)(DIST_SENSORES*unidad->getRadio()*2)),
             deceleracion(normal),
             objetivoSecundario1(NULL),
             objetivoSecundario2(NULL),
             distanciaDeambular(DISTANCIA_DEAMBULAR),
             despDeambular(DESP_DEAMBULAR_POR_SEG),
             radioDeambular(RADIO_DEAMBULAR),
             pesoSeguir(0.5),
             pesoLlegar(1.0),
             hayCoherenciaEspacial(false)
             


{
  double theta = RandFloat() * TwoPi;

  objetivoDeambular = Vector2D(radioDeambular * cos(theta),
                              radioDeambular * sin(theta));

}

//---------------------------------dtor ----------------------------------
SistemaMotor::~SistemaMotor(){}


//----------------------- Calcular --------------------------------------
//
//  calcula la fuerza acumulada
//
//------------------------------------------------------------------------
Vector2D SistemaMotor::Calcular()
{ 
  //inicializar fuerza
  fuerzaFinal.Cero();


  fuerzaFinal = CalcularConPrioridad();

  return fuerzaFinal;
}


double SistemaMotor::ComponenteFrontal()
{
  return unidad->getDireccion().ProdEsc(fuerzaFinal);
}


double SistemaMotor::ComponenteLado()
{
  return unidad->getDireccion().Perp().ProdEsc(fuerzaFinal);
}


//--------------------- AccumularFuerza ----------------------------------
//
//  Acumula la fuerza nueva mientras no se llegue a su fuerza máxima
//
//------------------------------------------------------------------------
bool SistemaMotor::AcumularFuerza(Vector2D &total,
                                       Vector2D nueva)
{  
  //cuanta fuerza ha usado
  double tamFuerzaTemp = total.Longitud();

  //cuanta queda por poder aplicar
  double tamFuerzaRestante = unidad->getFuerzaMaxima() - tamFuerzaTemp;

  //si no se puede aplicar mas devuelve false
  if (tamFuerzaRestante <= 0.0) return false;

  //tamaño de la fuerza nueva a aplicar
  double tamNuevaFuerza = nueva.Longitud();
  
  //si se puede añadir la nueva, añadirla a la que ya habia
  if (tamNuevaFuerza < tamFuerzaRestante)
  {
    total += nueva;
  }

  else
  {
    tamNuevaFuerza = tamFuerzaRestante;

    //si no se puede añadir toda, añadir la correspondiente a la restante
    total += (Vec2DNormalizar(nueva) * tamNuevaFuerza); 
  }

  return true;
}



//---------------------- CalcularConPrioridad ----------------------------
//
//  Calcula en orden estricto de prioridad hasta que no se pueda aplicar mas
//  fuerza al objeto
//------------------------------------------------------------------------
Vector2D SistemaMotor::CalcularConPrioridad()
{       
  Vector2D fuerza;

  if (On(evitar_durezas))
  {
    fuerzaFinal = EvitarDurezas() * pesoEvitarDurezas;

    if (!AcumularFuerza(fuerza, fuerzaFinal)) return fuerza;
  }


    if (On(separacion))
    {
      fuerzaFinal = Separacion() * pesoSeparacion;

      if (!AcumularFuerza(fuerza, fuerzaFinal)) return fuerza;
    }


  if (On(seguir))
  {
    fuerzaFinal = Seguir(objetivo) * pesoSeguir;

    if (!AcumularFuerza(fuerza, fuerzaFinal)) return fuerza;
  }


  if (On(llegar))
  {
    fuerzaFinal = Llegar(objetivo, deceleracion) * pesoLlegar;

    if (!AcumularFuerza(fuerza, fuerzaFinal)) return fuerza;
  }

  if (On(deambular))
  {
    fuerzaFinal = Deambular() * pesoDeambular;

    if (!AcumularFuerza(fuerza, fuerzaFinal)) return fuerza;
  }


  return fuerza;
}



//------------------------------- Seguir -----------------------------------
//
//  Este comportamiento dirige una fuerza hacia el objetivo
//
//------------------------------------------------------------------------
Vector2D SistemaMotor::Seguir(const Vector2D &target)
{
 
  Vector2D velocidadDeseada = Vec2DNormalizar(target - unidad->getPosicion())
                            * unidad->getVelocidadMaxima();

  return (velocidadDeseada - unidad->getVelocidad());
}


//--------------------------- Llegar -------------------------------------
//
//  Es como seguir, pero intentando llegar parado al destino
//
//------------------------------------------------------------------------
Vector2D SistemaMotor::Llegar(const Vector2D    &target,
                                const Deceleracion deceleracion)
{
  Vector2D ToTarget = target - unidad->getPosicion();

  //Distancia al objetivo
  double dist = ToTarget.Longitud();

  if (dist > 0)
  {
    //Multipicador de decelaracion
    const double multDeceleracion = 0.3;

    //Calcular la velocidad necesaria para llegar al objetivo
    double speed =  dist / ((double)deceleracion * multDeceleracion);     

    //la velocidad no debe exceder el maximo
    speed = MinOf(speed, unidad->getVelocidadMaxima());


    //Velocidad aplicada al vector
    Vector2D VelocidadDeseada =  ToTarget * speed / dist;

    //Fuerza resultante para alcanzarla
    return (VelocidadDeseada - unidad->getVelocidad());
  }

  return Vector2D(0,0);
}



//--------------------------- Deambular -------------------------------------
//
//  This behavior makes the agent wander about randomly
//------------------------------------------------------------------------
Vector2D SistemaMotor::Deambular()
{ /*
  //first, add a small random vector to the target's position
  objetivoDeambular += Vector2D(RandomClamped() * despDeambular,
                              RandomClamped() * despDeambular);

  //reproject this new vector back on to a unit circle
  objetivoDeambular.Normalizar();

  //increase the length of the vector to the same as the radius
  //of the wander circle
  objetivoDeambular *= RadioDeambular;

  //move the target into a position DeambularDist in front of the agent
  Vector2D objetivo = objetivoDeambular + Vector2D(distanciaDeambular, 0);

  //project the target into world space
  Vector2D Target = PointToWorldSpace(objetivo,
                                       unidad->Direccion(),
                                       unidad->Direccion().Perp(), 
                                       unidad->getPosicion());

  //and steer towards it
  return Target - unidad->getPosicion(); */
}


//--------------------------- EvitarDurezas --------------------------------
//
//  Este comoportamiento creará una fuerza para evitar colisionar
//  con las durezas del mapa
//
//------------------------------------------------------------------------
Vector2D SistemaMotor::EvitarDurezas()
{

  Vector2D fuerza;
  double tamVel = unidad->getVelocidad().Longitud();
  double tamSensor = tamVel;
  double mas_grande=0;

  CrearSensores();
  
  //Para todos los sensores
  for (int i=0; i<sensores.size(); i++)
  {  
      //Se calcula si hay dureza
      Vector2D final = sensores[i] + tamSensor*unidad->getDireccion();
      if ( Api::HayDureza(sensores[i], final ) )
      {
           //Calcular la fuerza a aplicar teniendo en cuenta el tamaño del sensor que esta
           //dentro de la pared
           double dentro = tamSensor - Api::UltimaColision();
           
           //Se escoje la colision que mas cerca este del objeto, es decir, 
           //la que tenga mas parte dentro de la dureza.
           if (mas_grande < dentro )
           {
              mas_grande=dentro;
              
              fuerza = Vec2DNormalizar(unidad->getPosicion() - final) * dentro;
           }
           
      }
  }
  
  return fuerza;
}


void SistemaMotor::CrearSensores()
{
  //Se guardará los puntos donde comienza cada sensor

  //Obtengo los vectores perpendiculares a la direccion (deben estar normalizados)
  Vector2D perpI = unidad->getDireccion().Perp();
  Vector2D perpD = perpI*(-1);
  
  int ind=0;
  for (int i=DIST_SENSORES;i<=unidad->getRadio();i+=DIST_SENSORES) 
  {
      //Sensores derecha
    sensores[ind++]= unidad->getPosicion() + i*perpD;
      //Sensores izquierda
    sensores[ind++]= unidad->getPosicion() + i*perpI;    
  }
  

}


//---------------------------- Separacion --------------------------------
//Calcula una fuerza que separa a la unidad de sus vecinos
//------------------------------------------------------------------------
Vector2D SistemaMotor::Separacion()
{  
  //iterate through all the neighbors and calculate the vector from the
  Vector2D SteeringForce;

  Api::BuscarUnidadesBegin(unidad,2);
  while (!Api::BuscarUnidadesEnd())
  {

      Vector2D ToAgent = unidad->getPosicion() - Api::GetPosUnidad(Api::BuscarUnidadesNext());

      //scale the fuerzaFinal inversely proportional to the agents distance  
      //from its neighbor.
      SteeringForce += Vec2DNormalizar(ToAgent)/ToAgent.Longitud();
  }

  return SteeringForce;
}























