
#include "lIAUnidadOfensiva.h"
#include "lApi.h"
#include "lenum_mensajes.h"

using namespace Logica;

IAUnidadOfensiva::IAUnidadOfensiva(Unidad * uni):IAUnidadMovil(uni)
{
  //Si no hay unidad objetivo
  unidad_objetivo=-1;
  
  ultimo_enemigo_visto=-1;
} 

/*
 * Actualiza la IA de la unidad asociada un paso en el juego
 */
void
IAUnidadOfensiva::Actualizar()
{
 /*
  //Si se ha empezado una accion, traducirla al estado correspondiente
  //en este automata 
  if (nueva_accion)
  {
      //Aquí están todas las acciones gestionadas por el AFD
      //No se contempla el modo, por ahora se implementa el modo defensivo
      switch(getAccion())
      {
         case Api::ANDAR:
              //estado=ANDANDO; //Añadir estado andando
              estado=CORRIENDO;
              velocidad=1;
         break;
         
         case Api::CORRER:
              estado=CORRIENDO;
              velocidad=2;
         break;
         
         case Api::PARAR:
              estado=ESPERANDO;
              //Si se para a la unidad se recuerda la 
              //posición para volver luego
              posicion_inicial=unidad->getPosicion();

         break;
         
         case Api::ANDAR_AGRESIVO:
              estado=ESPERANDO;
         break;
         
         case Api::CORRER_AGRESIVO:
              estado=ESPERANDO;
         break;
         
         case Api::ATACAR:
              estado=ESPERANDO_A_ATACAR;
              if (unidad_objetivo!=-1)
              {
                //Atacar a una unidad
                ultima_posicion_enemigo=Api::GetPosUnidad(unidad_objetivo);
              }
         break;
         
         default:
              estado=Unidad::NEUTRO;
      };
      //Ya ha empezado
      nueva_accion=false;
  }
  
  //Autómata
  printf("Automata soldado\n");
  switch(estado)
  {
 
     case ESPERANDO:
          printf("-ESPERANDO\n");
          
          //Parar la unidad
          PasoParar();                 
          
          //Se anula el objetivo
          unidad_objetivo=-1;
          
          //Eventos de cambio de estado          
          
          if (eventoEnemigoVisible())
          {
             estado=MIRANDO;
             ultima_posicion_enemigo=Api::GetPosUnidad(ultimo_enemigo_visto);
          }

     break;
     
     //En este estado ya se ha visto un enemigo y se sabe
     //su posicion
     case MIRANDO:

          printf("-MIRANDO\n");
          
          //miramos a la ultima posicion donde se ha visto un enemigo
            dir=ultima_posicion_enemigo - unidad->getPosicion();

           cout << "Unidad mirando a:" << dir << endl;
           
          //Si el enemigo localizado esta cerca, pasar a dispararle
          if (eventoEnemigoCerca()) estado=DISPARANDO;
          else//Si se sigue viendo se mirara y guardara la posicion
          if (eventoEnemigoVisible())
          {
             //Si está visible miramos hacia él y guardamos la posicion
             ultima_posicion_enemigo=Api::GetPosUnidad(ultimo_enemigo_visto);
             dir=ultima_posicion_enemigo - unidad->getPosicion();
          }


     break;
     
     case DISPARANDO:

          printf("-DISPARANDO\n");

          //Si esta muerto, dejar de tenerlo en cuenta
          if (eventoEnemigoMuerto())
          {
            ultimo_enemigo_visto=-1;
            estado=ESPERANDO;
            break;
          }
          //Mirar y disparar
          dir=Api::GetPosUnidad(ultimo_enemigo_visto) - unidad->getPosicion();
          PasoDisparar(ultimo_enemigo_visto);
          printf("Vida de %d: %d\n",ultimo_enemigo_visto,Api::GetVida(ultimo_enemigo_visto));
          
          if (eventoEnemigoMuerto())
          {
             estado=ESPERANDO;
             ultimo_enemigo_visto=-1;
          }
          else
          if (!eventoEnemigoCerca()) estado=MIRANDO;
     break;
          
     case CORRIENDO:
         
          printf("-CORRIENDO\n");

          if (velocidad==1) PasoAndar();
          else PasoCorrer(); 
          
          //Comprobar eventos de cambio de estado
          
          //Si ha llegado, ordeno parar
          if (eventoHaLlegado()) estado=ESPERANDO;
          else
          if (eventoObstaculo()) estado=CORREGIR_CORRIENDO;
          
     break;
          
     case CORREGIR_CORRIENDO:

          printf("-CORREGIR_CORRIENDO\n");

          //Aplico fuerza correctora sumandola a la direccion 
          //normalizada de la unidad

          //Por hacer
          
          estado=CORRIENDO;
     break;
          
     //Empieza orden con unidad objetivo
     case ESPERANDO_A_ATACAR:

          printf("-ESPERANDO A ATACAR\n");
          
          if (eventoObjetivoVisible()) estado=CORRER_A_ENEMIGO;
     break;
     
     case CORRER_A_ENEMIGO:
          
          printf("-CORRER A ENEMIGO\n");

          //Debe tener una unidad objetivo
          dir=AuxMover(Api::GetPosUnidad(unidad_objetivo));
          velocidad=2;
          
          if (eventoObjetivoCerca()) estado=DISPARAR_CORRIENDO;
                    
     break;
          
     case CORREGIR_CORRIENDO_ATACANDO:
     break;
          
     case DISPARAR_CORRIENDO:
          
          //Parar y disparar y si deja de estar cerca
          //volver a correr

          printf("-DISPARAR CORRIENDO\n");

          velocidad=0;
          PasoDisparar(unidad_objetivo);
          printf("Vida enemigo %d:%d\n",unidad_objetivo,Api::GetVida(unidad_objetivo));
          
          if (!eventoObjetivoCerca()) estado=CORRER_A_ENEMIGO;
          else
          if (eventoObjetivoMuerto())
          {
            estado=ESPERANDO;
            
            //Destruir unidad sin vida
          }
          
     break;
          
     case HUIR:
     break;
          
     case PARAR_DE_HUIR:
     break;
          
     case DISPARAR_SIN_PARAR:
     break;
                   
  };

  // Actualización de resultados

  cout << "Direccion: " << dir <<endl;
  printf("Velocidad: %d\n",velocidad);
  printf("Angulo: %f\n",angulo);
  //unidad->setVelocidad(velocidad);
  
  //angulo=AuxCalcularAngulo( dir );
  //unidad->setAngulo(angulo);  
 */       
}


//Eventos

  /*
   * -Si no hay un ultimo enemigo visto o
   *  no es visible..
   * -Buscar enemigos
   * - Verificar que no hay obstaculos entre ambos
   *
   * -Si la distancia al enemigo es menor que un umbral
   * cercano.
   *
   * -Si la distancia al enemigo es menor que un umbral
   * lejano y además que el angúlo de la unidad con respecto
   * al enemigo sea frontal o tenga un ángulo límite con ella.
   *
   * -Si encuentra algún enemigo lo guarda en ultimo_enemigo_visto
   */
bool
IAUnidadOfensiva::eventoEnemigoVisible()
{
   //TO DO: Bajar la frecuencia de ejecucion con un FrecEv
   
   //Si no hay enemigo a la vista o el que habia ya no es visible buscar otro
   if (ultimo_enemigo_visto==-1 || AuxEsVisiblePorUnidad(ultimo_enemigo_visto))
   {
      
      //Buscar otro enemigo cercano
     ultimo_enemigo_visto=AuxBuscarMejorEnemigo(); 
     
     if (ultimo_enemigo_visto==-1) return false;
     else return true;
      
   } else
         return true;
}

bool
IAUnidadOfensiva::eventoObjetivoVisible()
{
  return AuxEsVisiblePorUnidad(unidad_objetivo);
}

bool
IAUnidadOfensiva::eventoEnemigoCerca()
{
  return ( (unidad->getPosicion() - Api::GetPosUnidad(ultimo_enemigo_visto)).Longitud() < 10 );
}

bool
IAUnidadOfensiva::eventoObjetivoCerca()
{
  return ( (unidad->getPosicion() - Api::GetPosUnidad(unidad_objetivo)).Longitud() < 10 );
}

bool
IAUnidadOfensiva::eventoEnemigoMuerto()
{
  return (Api::GetVida(ultimo_enemigo_visto)==0);
}

bool
IAUnidadOfensiva::eventoObjetivoMuerto()
{
  return (Api::GetVida(unidad_objetivo)==0.);
}

bool
IAUnidadOfensiva::eventoPocaVida()
{
   return unidad->getVida()<5.;
}


bool
IAUnidadOfensiva::eventoLejosInicio()
{
            
}

bool
IAUnidadOfensiva::eventoObstaculo()
{
   return false;
}


//Acciones primitivas

//Ataque a distancia
//Intervienen en la puntuación la distancia,
//la orientacion y un factor aleatorio
void
IAUnidadOfensiva::PasoDisparar(int uni)
{
  //Para pruebas
  Api::EnviarMensaje(0,
                  unidad->getID(),
                  uni,
                  MSG_TeHeDado,
                  (void*)5);
}

//Ataque cuerpo a cuerpo
void
IAUnidadOfensiva::PasoAtacar()
{

}

//Acciones auxiliares

/*
 * Busca el mejor enemigo(el mas alineado y cercano)
 * de todos los que puede haber en un radio alrededor
 * de la unidad
 */
int
IAUnidadOfensiva::AuxBuscarMejorEnemigo()
{
  int uni=-1,sel=-1;
  Vector2D p,p2,p3;
  float distp3,ang,score,min=999999999.;
  
  Api::BuscarUnidadesBegin(unidad,4);
  while(!Api::BuscarUnidadesEnd())
  {
      uni=Api::BuscarUnidadesNext();
      
      //Por ahora no se contemplan equipos
      if ( /*unidad->getEquipo() != Api::GetEquipo(uni) &&*/ AuxEsVisiblePorUnidad(uni))
      {
         p = Api::GetPosUnidad(uni);
         p2= unidad->getPosicion();
         p3= (p - p2);
         distp3 = p3.Longitud();
         //Angulo entre ellas
         ang = acos( (dir*p3) / (dir.Longitud()*distp3) );
      
      cout << "ang1:" << dir <<endl;
      cout << "ang2:" << p3 <<endl;
      printf("ang3:%f\n",RADTODEG(ang));
      printf("dist:%f\n",distp3);
      
         score = distp3*distp3*ang;
      
         if (score<min)
         {
           min=score;
           sel=uni;
         }
      }
  }    
  
  return sel;                                 
}

/*
 * AuxEsVisiblePorUnidad mira si no hay obstaculos y además
 * comprueba que la distancia y el ángulo permiten verlo
 */
 bool
 IAUnidadOfensiva::AuxEsVisiblePorUnidad(int uni)
 {
    return !AuxHayObstaculos(uni); // && Comprobar distancia y orientacion
 }

/*
 * Devuelve true si la unidad(enemiga) es visible
 * desde la posición de esta unidad 
 *
 * -Necesario comprobar si hay dureza entre el enemigo
 * y la unidad.
 * -Y si hay unidades en medio que oculten (edificios)
 */
bool
IAUnidadOfensiva::AuxHayObstaculos(int uni)
{
   Vector2D pOri,pDest;
   
   pOri=unidad->getPosicion();
   pDest=Api::GetPosUnidad(uni);
   
   return Api::HayDureza(pOri.getX(),pOri.getY(),pDest.getX(),pDest.getY())  || AuxHayEdificios(pOri.getX(),pOri.getY(),pDest.getX(),pDest.getY());
   
}

/*
 * Comprobara que no hay unidades grandes en medio
 */
bool
IAUnidadOfensiva::AuxHayEdificios(float x,float y,float x2,float y2)
{
  return false;
}
