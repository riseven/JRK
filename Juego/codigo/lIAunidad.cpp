
#include "lAPI.h"
#include "lUnidad.h"
#include "lUnidadDinamica.h"
#include "lIAUnidad.h"
#include "lAutomataOfensivo.h"
#include "lenum_mensajes.h"


using namespace Logica;
using namespace Util;


IAUnidad::IAUnidad(Unidad * uni)
{
                    estado=0;//Unidad::NEUTRO;
                    
                   memoria = new SistemaSensorial ( uni, 5000 );
                   objetivo = new SistemaObjetivos ( uni );
                    
                   unidad = (UnidadDinamica*) uni;
                   
                   movimiento = new SistemaMotor ( unidad );
                   
                   frecVision.setFrec(1);
                   frecObjetivo.setFrec(0.5);
                   frecprueba.setFrec(0.05);
                   
                   elegir_objetivo=true;
                   
                   //Por defecto evitar durezas y unidades
                   movimiento->SeparacionOn();
                   movimiento->EvitarDurezasOn(); 
                   
                   afd = new MaquinaDeEstados<UnidadDinamica>(unidad);
                   
                   afd->SetEstadoGlobal( EstadoGlobal_Ofensivo::This() );
                   afd->SetEstadoActivo( EstadoDetener_Ofensivo::This() );
                   
                   modo = MODO_PASIVO;
}


IAUnidad::~IAUnidad()
{
                     
   if (memoria) delete memoria;
   if (objetivo) delete objetivo;
   if (movimiento) delete movimiento;
   if (afd) delete afd;
}


void
IAUnidad::Actualizar() 
{

   afd->Actualizar();
   
   if (frecVision.comprobar()) memoria->ActualizarVision();
   
   if ( elegir_objetivo && frecObjetivo.comprobar()) objetivo->Actualizar();

  Vector2D fuerza = movimiento->Calcular();

  //Si no hay fuerza nueva, frenar la unidad
  if (movimiento->FuerzaFinal().esCero())
  {
        //const double potenciaFrenada = 0.01; 
        //unidad->setVelocidad( unidad->getVelocidad() * potenciaFrenada);                                     
        Vector2D cero;
        unidad->setVelocidad( cero );                                         
  }else
  {

      //Aceleracion
      Vector2D acel = fuerza / unidad->getMasa();
    
      //Nueva Velocidad
      Vector2D vel = unidad->getVelocidad() + acel;
      //Truncar si supera su máxima velocidad
      vel.Truncar ( unidad->getVelocidadMaxima() );
      
      unidad->setVelocidad ( vel );
    
      //Nueva posicion
      unidad->setPosicion( (unidad->getPosicion() + vel) );
    
      //Actualizar vectores de direccion
      if (!vel.esCero())
      {    
        unidad->setDireccion( Vec2DNormalizar( vel ) );    
        //m_vSide = m_vHeading.Perp();
      }
  }

}


bool
IAUnidad::RecibirMensaje(const Mensaje &msg)
{
   
   switch ( msg.msg )
   {
          case AC_CambiarModo:
               modo = *(int*)msg.datos;
               printf("Cambiando modo:%d\n",modo);
          return true;
   };
    
   return ( afd && afd->RecibirMensaje( msg ) );
}
