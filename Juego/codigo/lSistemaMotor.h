#ifndef __SISTEMA_MOTOR_H__
#define __SISTEMA_MOTOR_H__

#include <vector>
#include "uWinDefs.h"
#include <string>
#include <list>
#include "uVector2D.h"
//#include "constants.h"

namespace Logica{


class UnidadDinamica;


//--------------------------- Constants ----------------------------------

//the radius of the constraining circle for the wander behavior
const double RADIO_DEAMBULAR    = 1.2;
//distance the wander circle is projected in front of the agent
const double DISTANCIA_DEAMBULAR   = 2.0;
//the maximum amount of displacement along the circle each frame
const double DESP_DEAMBULAR_POR_SEG = 40.0;

const int DIST_SENSORES=2; //Distancia entre sensores

                                          



//------------------------------------------------------------------------

class SistemaMotor
{
public:


private:

  enum Comportamiento
  {
    ninguno               = 0x00000,
    seguir               = 0x00002,
    llegar             = 0x00008,
    deambular          = 0x00010,
    separacion         = 0x00040,
    evitar_durezas     = 0x00200,
  };

private:

  
  //a pointer to the owner of this instance
  UnidadDinamica*     unidad; 
  
  //the steering force created by the combined effect of all
  //the selected behaviors
  Util::Vector2D       fuerzaFinal;
 
  //these can be used to keep track of friends, pursuers, or prey
  UnidadDinamica*     objetivoSecundario1;
  UnidadDinamica*     objetivoSecundario2;

  //the current target
  Util::Vector2D    objetivo;


  //a vertex buffer to contain the feelers rqd for wall avoidance  
  std::vector<Util::Vector2D> sensores;
  
  //the length of the 'feeler/s' used in wall detection
  double                 longitudSensores;


  //the current position on the wander circle the agent is
  //attempting to steer towards
  Util::Vector2D     objetivoDeambular; 

  //explained above
  double        despDeambular;
  double        radioDeambular;
  double        distanciaDeambular;


  //Multiplicadores para los comportamientos.
  double        pesoSeparacion;
  double        pesoDeambular;
  double        pesoEvitarDurezas;
  double        pesoSeguir;
  double        pesoLlegar;


  double        distanciaVision;

  //binary flags to indicate whether or not a behavior should be active
  int           banderas;

  
  //Arrive makes use of these to determine how quickly a UnidadDinamica
  //should decelerate to its target
  enum Deceleracion{lenta = 3, normal = 2, rapida = 1};

  //default
  Deceleracion deceleracion;

  //is cell space partitioning to be used or not?
  bool          hayCoherenciaEspacial;


  //Pregunta si hay un comportamiento especifico activado
  bool      On(Comportamiento comp){return (banderas & comp) == comp;}

  bool      AcumularFuerza(Util::Vector2D &suma, Util::Vector2D fuerzaNueva);

  //Crea 3 antenas
  void      CrearSensores();



   /* .......................................................

                          COMPORTAMIENTOS

      .......................................................*/


  //Movimiento directo hacia un objetivo
  Util::Vector2D Seguir(const Util::Vector2D &obj);

  //Igual que Seguir pero llegando parado
  Util::Vector2D Llegar(const Util::Vector2D    &obj,
                  const Deceleracion deceleracion);

  //Movimiento aleatorio restringido
  Util::Vector2D Deambular();

  Util::Vector2D EvitarDurezas();

  //Calcula una fuerza de separacion con la lista de vecinos
  Util::Vector2D Separacion();





  Util::Vector2D CalcularConPrioridad();

  
public:

  SistemaMotor( UnidadDinamica* agent);

  virtual ~SistemaMotor();


  Util::Vector2D Calcular();

 
  double    ComponenteFrontal();

 
  double    ComponenteLado();


  void      SetObjetivo(Util::Vector2D t){objetivo = t;}
  Util::Vector2D  GetObjetivo()const{return objetivo;}

  void      SetObjetivoSecundario1(UnidadDinamica* uni){ objetivoSecundario1 = uni;}
  void      SetObjetivoSecundario2(UnidadDinamica* uni){ objetivoSecundario2 = uni;}


  Util::Vector2D  FuerzaFinal()const{return fuerzaFinal;}


  void SeguirOn(){banderas |= seguir;}
  void LlegarOn(){banderas |= llegar;}
  void DeambularOn(){banderas |= deambular;}
  void SeparacionOn(){banderas |= separacion;}
  void EvitarDurezasOn(){banderas |= evitar_durezas;}

  void SeguirOff()  {if(On(seguir))   banderas ^=seguir;}
  void LlegarOff(){if(On(llegar)) banderas ^=llegar;}
  void DeambularOff(){if(On(deambular)) banderas ^=deambular;}
  void SeparacionOff(){if(On(separacion)) banderas ^=separacion;}
  void EvitarDurezasOff(){if(On(evitar_durezas)) banderas ^=evitar_durezas;}

  bool EstaSeguirOn(){return On(seguir);}
  bool EstaLlegarOn(){return On(llegar);}
  bool EstaDeambularOn(){return On(deambular);}
  bool EstaSeparacionOn(){return On(separacion);}
  bool EstaEvitarDurezasOn(){return On(evitar_durezas);}

  const std::vector<Util::Vector2D>& GetSensores()const{return sensores;}
  
  double getDespDeambular()const{return despDeambular;}
  double getDistanciaDeambular()const{return distanciaDeambular;}
  double gtRadioDeambular()const{return radioDeambular;}

  double GetPesoSeparacion()const{return pesoSeparacion;}

};


}

#endif
