#include <allegro.h>
#include "gAPI.h"
#include "iAPI.h"
#include "lAPI.h"
#include "rAPI.h"
#include "eExcepcionFichero.h"
#include "uFrecReal.h"
#include "uVector2D.h"
#include "lenum_mensajes.h"
#include "rProtocolo.h"
#include "lSistemaObjetivos.h"
#include "uWinDefs.h"
#include <iostream>


using namespace std;
using namespace Util;

BITMAP * bunidad;
Logica::Unidad* seleccionado = NULL;
vector<Logica::Unidad*> multiseleccionados;
int numUnidades=0;


void init() {
	int depth, res;
	allegro_init();
	depth = desktop_color_depth();
	if (depth == 0) depth = 32;
	set_color_depth(depth);
//	if (res != 0) {
//		allegro_message(allegro_error);
//		exit(-1);
//	}

	install_timer();
	install_keyboard();
	install_mouse();
	/* add other initializations here */
}

void deinit() {
	clear_keybuf();
	/* add other deinitializations here */
}

void PintarUnidades(BITMAP* bmp)
{
     vector<Logica::Unidad*> lista = Logica::Api::GetListaUnidades();
     
     for (int i=0;i<lista.size();i++)
     {
         Vector2D v = lista[i]->getPosicion();
         
         //masked_blit (bunidad,bmp,0,0,v.getX(),v.getY(),bunidad->w,bunidad->h );
         Vector2D pr(1,1);
         rotate_sprite(bmp, bunidad, v.getX()-bunidad->w/2.0, v.getY()-bunidad->h/2.0,  ftofix(lista[i]->getDireccion().AnguloDEG()*0.71111) );
         if (seleccionado==lista[i]) circle(bmp, v.getX(), v.getY(), 11, makecol(0,0,0));

         list<Logica::Unidad *> lrec = lista[i]->getIA()->GetMemoria()->GetListaRecordados();
         
         list<Logica::Unidad *>::const_iterator itactual = lrec.begin();
         for (itactual; itactual!=lrec.end(); ++itactual)
         {
             Vector2D ps = lista[i]->getIA()->GetMemoria()->GetPosicionRecordada(*itactual);
             //if ( lista[i]->getIA()->GetMemoria()->esDisparable(*itactual) )
             //line(bmp,v.getX(), v.getY(),  ps.getX(), ps.getY(), makecol(255,255,0));
         }
         
         Logica::SistemaObjetivos * obj=lista[i]->getIA()->GetObjetivo();
         Vector2D pos = obj->GetPosicionObjetivo();
         
         if ( obj->esObjetivoVisible() )
           line(bmp,v.getX(), v.getY(),  pos.getX(), pos.getY(), makecol(255,0,0));    
         
     }
}

Logica::Unidad* Seleccionar()
{
     vector<Logica::Unidad*> lista = Logica::Api::GetListaUnidades();
     
     Vector2D v2(mouse_x,mouse_y);
     
     for (int i=0;i<lista.size();i++)
     {
         Vector2D v = lista[i]->getPosicion();
         
         if (v.Distancia(v2)< 6.0)
           return lista[i];
     }
     return NULL;
}

int main(int argc, char *argv[])
{
    srand(Util::GetTiempoActual());
    #define __DEBUG_LOGICA
    init();
    
//    Interfaz::Api::Inicializar();
//    Interfaz::Unidad::Inicializar();
    Interfaz::Api::SetInfoMapa(new Interfaz::InfoMapa("MapaDePrueba.map")); 
    Logica::Api::Inicializar();
//    Red::Api::Inicializar(Red::Api::MODO_LOCAL);
    
    char opc;
    float x,y;
    int id,com;
    int * ent;
    float escala = Graficos::Api::GetEscalaTerreno();
    int cont=0;
    Util::FrecReal fr(25);//5 veces por segundo
      
    // Teclas
    bool creando=false;   
    
    
      
    bunidad = load_bitmap("bitmap_unidad.bmp",NULL);
    BITMAP * raton = load_bitmap("Graficos/Mouse.bmp",NULL);
    BITMAP * imagen = load_bitmap(Interfaz::Api::GetInfoMapa()->GetFicheroPreview().c_str(),NULL);
    BITMAP * grande = create_bitmap(imagen->w*escala,imagen->h*escala);
    BITMAP * doble_buf = create_bitmap(imagen->w*escala,imagen->h*escala);
    stretch_blit(imagen, grande, 0, 0, imagen->w, imagen->h, 0, 0, grande->w, grande->h);
    destroy_bitmap(imagen);

  	set_gfx_mode(GFX_AUTODETECT_WINDOWED, grande->w, grande->h, 0, 0);


    while(!key[KEY_ESC])
    { 
      
      if (key[KEY_1])
      {
        if (!creando)
        {
          float x=rand()%SCREEN_W,y=rand()%SCREEN_H;
          while (Logica::Api::HayDureza(x,y))
          {
                x=rand()%SCREEN_W;
                y=rand()%SCREEN_H;
          }
          printf("Nueva unidad, posicion %f %f\n",x,y);
          Logica::Api::CrearUnidad(Red::Protocolo::Unidad::Soldado,x,y);
          numUnidades++;
        }
        creando=true;
      }else
        creando=false;
        
      if (key[KEY_2] && numUnidades>0)
      {
        if (Logica::Api::DestruirUnidad(rand()%numUnidades) )
              numUnidades--;
      }
      
      if (key[KEY_3])
      {
          float x=rand()%SCREEN_W,y=rand()%SCREEN_H;
          while (Logica::Api::HayDureza(x,y))
          {
                x=rand()%SCREEN_W;
                y=rand()%SCREEN_H;
          }
            Vector2D v(x,y);
            vector<Logica::Unidad*> vec=Logica::Api::GetListaUnidades();
            for (int i=0;i<vec.size();i++)
            {
             if (vec[i])
             {
              vec[i]->getIA()->GetMovimiento()->SetObjetivo(v);     
              vec[i]->getIA()->GetMovimiento()->SeguirOn();
              vec[i]->getIA()->GetMovimiento()->SeparacionOn();
              vec[i]->getIA()->GetMovimiento()->EvitarDurezasOn();                    
              }
            }
              
      }
        
        
      if (mouse_b & 1)
      {
        Logica::Unidad * temp=Seleccionar();
    //    if ( !key[KEY_LCONTROL] )
    //    {
            if (temp)// && temp!=seleccionado) 
            {
                             seleccionado=temp;
                              seleccionado->getIA()->GetMovimiento()->SeguirOff();
                         //     seleccionado->getIA()->GetMovimiento()->SeparacionOff();
    
            }//else
        //       multiseleccionados.clear();
   /*     }
        else
        {
            if (temp)// && temp!=seleccionado) 
            {
                             seleccionado=temp;
                              seleccionado->getIA()->GetMovimiento()->SeguirOff();
                         //     seleccionado->getIA()->GetMovimiento()->SeparacionOff();
                         multiseleccionados.push_back(seleccionado);
    
            }
            
        }*/
      }
      
      if ((mouse_b & 2) && seleccionado!=NULL)
      {
              Vector2D v(mouse_x,mouse_y);   
              //seleccionado->setPosicion(v);  
              seleccionado->getIA()->GetMovimiento()->SetObjetivo(v);     
              seleccionado->getIA()->GetMovimiento()->SeguirOn();
              seleccionado->getIA()->GetMovimiento()->SeparacionOn();
              seleccionado->getIA()->GetMovimiento()->EvitarDurezasOn();
              
/*              for (int i=0;i<multiseleccionados.size()-1;i++)
              {
                  multiseleccionados[i]->getIA()->GetMovimiento()->SetObjetivo(v);     
                  multiseleccionados[i]->getIA()->GetMovimiento()->SeguirOn();
                  multiseleccionados[i]->getIA()->GetMovimiento()->SeparacionOn();
                  multiseleccionados[i]->getIA()->GetMovimiento()->EvitarDurezasOn();                  
              }*/
              
      }
    
      blit(grande,doble_buf,0,0,0,0,grande->w,grande->h);
      PintarUnidades(doble_buf); 
      
      masked_blit(raton,doble_buf,0,0,mouse_x,mouse_y,raton->w,raton->h);
      blit(doble_buf,screen,0,0,0,0,grande->w,grande->h);
      
      
      if (fr.comprobar()) Logica::Api::Actualizar();
      
 //     fflush( stdin );
/*      printf("1.Crear unidad\n");
      printf("2.Destruir unidad\n");
      printf("3.Listar unidades\n");
      printf("4.Enviar comando\n");
      printf("5.Actualizar\n");
      printf("6.Actualizar n veces\n");
      printf("Opc?");
      opc=getchar();
      
      try{
      
      switch(opc)
      {
                 case '1':
                     printf("Posicion de unidad soldado nueva( x y ): ");
                     scanf("%f %f",&x,&y);
                     printf("ID:%d\n",Logica::Api::CrearUnidad(Red::Protocolo::Unidad::Soldado,x,y));
                     
                 break;
                 
                 case '2':
                     printf("Id de unidad:");
                     scanf("%d",&id);
                     Logica::Api::DestruirUnidad(id);
                 break;
                 
                 case '3':
                      Logica::Api::ImprimirUnidades();
                 break;
                 
                 case '4':
                      printf("Unidad destino:");
                      scanf("%d",&id);
                      printf("Comando ANDAR(%d) PARAR(%d) ATACAR(%d):",Logica::AC_MoverUnidad,Logica::AC_DetenerUnidad,Logica::AC_AtacarUnidad);
                      scanf("%d",&com);
                      

                      switch (com)
                      {
                             case Logica::AC_MoverUnidad:
                                    printf("Parametros x y:");
                                    scanf("%f %f",&x,&y);
                                    Logica::Api::EnviarMensaje( SIN_RETARDO,
                                                                ORIGEN_INNECESARIO,
                                                                id,
                                                                com,
                                                                new Vector2D(x,y) );
                             break;
                             
                             case Logica::AC_AtacarUnidad:
                                    printf("Parametro unidad:");
                                    scanf("%f",&x);
                                    ent=new int;
                                    *ent=(int)x;
                                    Logica::Api::EnviarMensaje( SIN_RETARDO,
                                                                ORIGEN_INNECESARIO,
                                                                id,
                                                                com,
                                                                ent );
                             break;
                             
                             case Logica::AC_DetenerUnidad:
                                    Logica::Api::EnviarMensaje( SIN_RETARDO,
                                                                ORIGEN_INNECESARIO,
                                                                id,
                                                                com,
                                                                NULL );
                             break;
                      }

                 break;
                 
                 case '5':
                      Logica::Api::Actualizar();
                      printf("Despues de actualizar\n");
                      Logica::Api::ImprimirUnidades();
                 break;
                 
                 case '6':
                      printf("Veces:");
                      int v;
                      scanf("%d",&v);
                      for (int i=0;i<v;i++)
                          Logica::Api::Actualizar();
                          
                      Logica::Api::ImprimirUnidades();
                 break;
      };
           
    }
    catch ( Error::ExcepcionFichero ex )
    {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("%s\n", ex.GetMensajeDeError().c_str() );
    }
    catch ( exception &ex )
    {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("%s\n", ex.what() );
    }   
    catch (...)
    {   
    }*/
       
    }
         deinit();
    return 0;    
}
END_OF_MAIN();
