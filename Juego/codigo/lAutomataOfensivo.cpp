#include "lAutomataOfensivo.h"
#include "lUnidadDinamica.h"
#include "lIAUnidad.h"
#include "uVector2D.h"
#include "lSistemaObjetivos.h"
#include "lenum_mensajes.h"
#include "rProtocolo.h"

using namespace Logica;
using namespace Util;


//
//Estado Global
//


//Método de código que es necesario ejecutar en cada iteración del autómata
void
EstadoGlobal_Ofensivo::Ejecutar(UnidadDinamica* uni)                                     
{
    
}


bool
EstadoGlobal_Ofensivo::RecibirMensaje(UnidadDinamica * uni, const Mensaje & msg)
{
    switch (msg.msg)
    {
           case   AC_MoverUnidad:     //Vector2D(float x, float z);
              //Establecer un objetivo en el mapa
              uni->getIA()->GetMovimiento()->SetObjetivo(*((Vector2D*)msg.datos));
              //Cambiar al estado correspondiente
              uni->getIA()->GetAFD()->CambiarEstado(EstadoMover_Ofensivo::This());
           return true;
           
           case   AC_MoverAgresivo:   //Vector2D(float x, float z); 
           
           case   AC_DetenerUnidad:   //(); 

              uni->getIA()->GetAFD()->CambiarEstado(EstadoDetener_Ofensivo::This());
           return true;
           
           //Solo aplicable a enemigos
           case   AC_AtacarUnidad:    //(int handlerObjetivo);
           //Solo aplicable a aliados
           case   AC_SeguirUnidad:    //(int handlerObjetivo); 
           case   AC_AtacarZona:      //Vector2D(float x, float z); 
           case   AC_CambiarArma:      //(int idArma )
           break;
    };
    
    return false;
}


//
// Estado Mover
//

void
EstadoMover_Ofensivo::Entrar(UnidadDinamica* uni)
{
  //Cuando entra por primera vez al estado mover asegurar que puede hacerlo
  uni->getIA()->GetMovimiento()->SeguirOn();
}

void
EstadoMover_Ofensivo::Ejecutar(UnidadDinamica* uni)                                     
{
  
  //Por ahora comprobar sólo si ha llegado
  if ( uni->HaLlegado( uni->getIA()->GetMovimiento()->GetObjetivo() ) )//uni->getIA()->getObjetivoIA() ) )
    uni->getIA()->GetAFD()->CambiarEstado( EstadoDetener_Ofensivo::This() );
    
}

void
EstadoMover_Ofensivo::Salir(UnidadDinamica* uni)
{
  //Ya no debe moverse
  uni->getIA()->GetMovimiento()->SeguirOff();
  
  //Si tenia algun objetivo, olvidarlo
  uni->getIA()->GetMemoria()->Olvidar(uni->getIA()->GetObjetivo()->GetObjetivo());
}




//
// Estado Detener
//

void
EstadoDetener_Ofensivo::Entrar(UnidadDinamica* uni)
{

}

void
EstadoDetener_Ofensivo::Ejecutar(UnidadDinamica* uni)                                     
{

   //Establecer nueva posicion y direccion de reposo para la unidad
   uni->getIA()->GuardarReposo();

    switch ( uni->getIA()->getModo() )
    {
           case Red::Protocolo::ModoUnidad::Pasivo:
               
               uni->getIA()->GetAFD()->CambiarEstado( ModoPasivo_Ofensivo::This() );
                
           break;
           
           case Red::Protocolo::ModoUnidad::Defensivo:
                
                uni->getIA()->GetAFD()->CambiarEstado( ModoDefensivo_Ofensivo::This() );
                
           break;
           
           case Red::Protocolo::ModoUnidad::Agresivo:
                
                uni->getIA()->GetAFD()->CambiarEstado( ModoAgresivo_Ofensivo::This() );
                
           break;
           
           case Red::Protocolo::ModoUnidad::MantenerPosicion:
                
                
           break;
    };
   
}

void
EstadoDetener_Ofensivo::Salir(UnidadDinamica* uni)
{


}


//
// Estado Seguir
//

void
EstadoSeguir_Ofensivo::Entrar(UnidadDinamica* uni)
{

}

void
EstadoSeguir_Ofensivo::Ejecutar(UnidadDinamica* uni)                                     
{

}

void
EstadoSeguir_Ofensivo::Salir(UnidadDinamica* uni)
{


}


//
// Modo Pasivo
//

void
ModoPasivo_Ofensivo::Entrar( UnidadDinamica* uni )
{
  uni->getIA()->ElegirObjetivo(false);
}

void
ModoPasivo_Ofensivo::Salir( UnidadDinamica* uni )
{
  uni->getIA()->ElegirObjetivo(true);                            
}


//
// Modo Defensivo
//

/*
 * En este modo la unidad buscara objetivos y los seguirá y 
 * atacará mientras no salga de su radio de defensa
 */

void
ModoDefensivo_Ofensivo::Entrar(UnidadDinamica* uni)
{
    //La unidad debe poder elegir objetivos
    uni->getIA()->ElegirObjetivo(true);                                               
}

void
ModoDefensivo_Ofensivo::Ejecutar(UnidadDinamica* uni)                                     
{
   SistemaObjetivos * sObj = uni->getIA()->GetObjetivo();
   IAUnidad * ia = uni->getIA();
   
   //Si estamos viendo a un objetivo en este momento
   if ( sObj->hayObjetivo() && sObj->esObjetivoVisible() )
   {
        //comprobar que con el rango del arma actual se puede disparar
        //sin moverse, sino perseguirlo siempre que no se sobrepase el
        //limite ni se acerque demasiado al objetivo
        float distCuad = Vec2DDistanciaCuad( ia->getPosicionReposo(), uni->getPosicion() );
        float distDefCuad = uni->getDistanciaDefensa()*uni->getDistanciaDefensa();
        float rad1 = uni->getRadio();
        float rad2 = sObj->GetObjetivo()->getRadio();
        float rad = rad1 + rad2;
        float radCuad = rad*rad;
        
        if ( distDefCuad > distCuad && 
             Vec2DDistanciaCuad( uni->getPosicion(), sObj->GetObjetivo()->getPosicion() ) > radCuad*2 )
        {
           ia->GetMovimiento()->SetObjetivo( sObj->GetPosicionObjetivo() );
           ia->GetMovimiento()->SeguirOn();
        }else
        {
           ia->GetMovimiento()->SeguirOff();
           uni->MirarA( sObj->GetObjetivo()->getPosicion() );
        }
        
        
        
   }else //Si no hay objetivos o no son visibles, volver a la posicion de reposo
   {
         if ( !uni->HaLlegado( ia->getPosicionReposo() ) )
         {
           ia->GetMovimiento()->SetObjetivo( ia->getPosicionReposo() );
           ia->GetMovimiento()->SeguirOn();
         }else
         {
             ia->GetMovimiento()->SeguirOff();
             //Lo orientamos conforme estaba antes
             uni->setDireccion( ia->getDireccionReposo() );
             Vector2D cero;
             uni->setVelocidad(cero);
         }
   }
}

void
ModoDefensivo_Ofensivo::Salir(UnidadDinamica* uni)
{

}


//
// Modo Agresivo
//

/*
 * En este modo la unidad buscara objetivos y los seguirá y 
 * atacará sin limite de distancia
 */

void
ModoAgresivo_Ofensivo::Entrar(UnidadDinamica* uni)
{
    //La unidad debe poder elegir objetivos
    uni->getIA()->ElegirObjetivo(true);                                               
}

void
ModoAgresivo_Ofensivo::Ejecutar(UnidadDinamica* uni)                                     
{
   SistemaObjetivos * sObj = uni->getIA()->GetObjetivo();
   IAUnidad * ia = uni->getIA();
   
   //Si estamos viendo a un objetivo en este momento
   if ( sObj->hayObjetivo() && sObj->esObjetivoVisible() )
   {
        //comprobar que con el rango del arma actual se puede disparar
        //sin moverse, sino perseguirlo siempre que no se acerque 
        //demasiado al objetivo
        
        float rad1 = uni->getRadio();
        float rad2 = sObj->GetObjetivo()->getRadio();
        float rad = rad1 + rad2;
        float radCuad = rad*rad;
        
        if ( Vec2DDistanciaCuad( uni->getPosicion(), sObj->GetObjetivo()->getPosicion() ) > radCuad*2 )
        {
           ia->GetMovimiento()->SetObjetivo( sObj->GetPosicionObjetivo() );
           ia->GetMovimiento()->SeguirOn();
        }else
        {
           ia->GetMovimiento()->SeguirOff();
           uni->MirarA( sObj->GetObjetivo()->getPosicion() );
        }
        
        
        
   }else //Si el objetivo ya no es visible, ir a la ultima posicion recordada
   {
         if ( sObj->hayObjetivo() )
         {
             if ( !uni->HaLlegado( sObj->GetPosicionObjetivo() ) )
             {
               ia->GetMovimiento()->SetObjetivo( sObj->GetPosicionObjetivo() );
               ia->GetMovimiento()->SeguirOn();
             }else
             {
                 ia->GetMovimiento()->SeguirOff();

             }
         }else
         {
             ia->GetMovimiento()->SeguirOff(); 
          }
   }
}

void
ModoAgresivo_Ofensivo::Salir(UnidadDinamica* uni)
{

}
