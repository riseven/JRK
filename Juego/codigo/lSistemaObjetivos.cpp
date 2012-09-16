#include "lSistemaObjetivos.h"
#include "lUnidad.h"
#include "lIAUnidad.h"
#include "lSistemaSensorial.h"

using namespace Logica;

//-------------------------------- ctor ---------------------------------------
//-----------------------------------------------------------------------------
SistemaObjetivos::SistemaObjetivos(Unidad* owner):unidad(owner),
                                                               objetivoActual(0)
{}



//----------------------------- Actualizar ----------------------------------------
// Se elige el enemigo mas cercano
// MEJORAS:
//  - Elegir el objetivo que este más cerca y mas alineado
//  - Dependiendo de la vida de los enemigos elegir el más débil
//-----------------------------------------------------------------------------
void SistemaObjetivos::Actualizar()
{
  double masCercano = MaxDouble;
  objetivoActual       = 0;


  std::list<Unidad*> memorizados;
  memorizados = unidad->getIA()->GetMemoria()->GetListaRecordados();
  
  std::list<Unidad*>::const_iterator actual = memorizados.begin();
  for (actual; actual != memorizados.end(); ++actual)
  {
    //Elegimos sólo de entre los que esten vivos y no sean la propia unidad, además de no ser ni del mismo equipo ni del equipo neutral
    if ((*actual)->estaVivo() && (*actual != unidad) && (*actual)->getEquipo()!=unidad->getEquipo() && (*actual)->getEquipo()!=-1 )
    {
      double dist = Vec2DDistanciaCuad((*actual)->getPosicion(), unidad->getPosicion());

      if (dist < masCercano)
      {
        masCercano = dist;
        objetivoActual = *actual;
      }
    }
  }
}




bool SistemaObjetivos::esObjetivoVisible()const
{
  return unidad->getIA()->GetMemoria()->esVisible(objetivoActual);
}

bool SistemaObjetivos::esObjetivoDisparable()const
{
  return unidad->getIA()->GetMemoria()->esDisparable(objetivoActual);
}

Util::Vector2D SistemaObjetivos::GetPosicionObjetivo()const
{
  return unidad->getIA()->GetMemoria()->GetPosicionRecordada(objetivoActual);
}

double SistemaObjetivos::GetTiempoObjetivoHaSidoVisible()const
{
  return unidad->getIA()->GetMemoria()->GetTiempoSiendoVisible(objetivoActual);
}

double SistemaObjetivos::GetTiempoSinVerObjetivo()const
{
  return unidad->getIA()->GetMemoria()->GetTiempoSinHaberSidoVisto(objetivoActual);
}
