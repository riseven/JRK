#ifndef SISTEMA_SENSORIAL_H
#define SISTEMA_SENSORIAL_H

//-----------------------------------------------------------------------------
//
//  Clase que se encarga de actualizar los sentidos de las unidades, periódicamente ira actuazlizando
//  la vista del agente que lo posea y eventualmente el sonido.
//
//-----------------------------------------------------------------------------
#include <map>
#include <list>
#include "uVector2d.h"
#include "lUnidad.h"


namespace Logica{

//Guarda un registro de memoria
class Memoria
{
public:
  
  //Tiempo en el que se guardo el registro
  double       tiempoRegistro;

  //Tiempo que un agente ha sido visible
  double       tiempoSiendoVisible;

  //Tiempo en el cual lo fue por última vez
  double       tiempoUltimaVezVisto;
  
  //Tiempo en el que se vió por primera vez, si deja de ser visible y vuelve a serlo, se inicializa el tiempo
  double       tiempoPrimeraVezVisto;

  //Posicion del agente dondé se sintió (visto o oido)
  Util::Vector2D    posicion;

  //Guarda si es visible actualmente por la unidad
  bool        esVisible;

  //Guarda si entre él y la unidad hay linea de disparo (no hay durezas) 
  bool        disparable;
  

  Memoria():tiempoRegistro(-999),
            tiempoSiendoVisible(-999),
            tiempoUltimaVezVisto(0),
            esVisible(false),
            disparable(false)
  {}
};


class SistemaSensorial
{
private:

  typedef std::map<Unidad*, Memoria> MapaDeMemoria;

private:
  
  //unidad que contiene este sistema
  Unidad* unidad;

  //Se guardara un registro por cada unidad que se sienta, y se actualizará si ya estaba en el mapa
  MapaDeMemoria  mapa;

  //Tiempo hasta el cual los registros son válidos, simula la memoria a corto plazo
  double      tiempoMemoria;

  //Crea un nuevo registro de la unidad si no estaba ya
  void       CrearNuevoRegistro(Unidad* uni);

public:

  //Crea el sistema a partir de una unidad origen y de un tiempo de recuerdo máximo
  SistemaSensorial(Unidad* uni, double tiempoMemoria);

  //Cuando alguna unidad hace algún sonido percibible por ésta unidad, se actualiza el mapa de memoria
  void     ActualizarFuenteDeSonido(Unidad* fuenteSonido);

  //Elimina a la unidad del mapa
  void     EliminarDeMemoria(Unidad* uni);
  
  void     Olvidar( Unidad* uni );
  
  //Actualiza la vista de la unidad
  void     ActualizarVision();

  bool     esDisparable(Unidad* enemigo)const;
  bool     esVisible(Unidad* enemigo)const;
  Util::Vector2D GetPosicionRecordada(Unidad* enemigo)const;
  double    GetTiempoSiendoVisible(Unidad* enemigo)const;
  double    GetTiempoSinHaberSidoVisto(Unidad* enemigo)const; 
  double    GetTiempoDesdeQueSeSintio(Unidad* enemigo)const;

  //Se devuelve una lista con todas las unidades que recuerda
  std::list<Unidad*> GetListaRecordados()const;

};


}

#endif

