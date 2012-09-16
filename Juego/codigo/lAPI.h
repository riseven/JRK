/*
 * Interfaz para utilizar la lógica, contiene funciones
 * que utilizan internamente las clases de la Lógica 
 * del juego y funciones útiles
 *
 */
 
#ifndef __lAPI_H_
#define __lAPI_H_

#include <string>
#include <vector>
#include <list>

#include "logica.h"
#include "uVector2D.h"
#include "lUnidad.h"
#include "lIAUnidad.h"
#include "lJugador.h"
#include "lCentroDeMensajes.h"

namespace Logica
{
    
class Api
{
      public:
      
     //-- Acciones de la Lógica --
     
           static void UnirsePartida(int numJugador, string nombreJugador);
    
    //! Indica a la lógica que un jugador ha abandonado la partida
           //static void AbandonarPartida();
    
    //! Indica a la lógica que un jugador quiere cambiar de color de equipo
    //! (Solo tiene efecto en la pantalla de preparación de la partida)
           static void CambiarColor(int numJugador);
    
    //! Indica a la lógica que un jugador quiere cambiar de bando
    //! (Solo tiene efecto en la pantalla de preparación de la partida)    
           static void CambiarBando();
    
    //! Indica a la lógica que el servidor de la partida ha pulsado el boton
    //! Empezar partida
           static void EmpezarPartida();
    
    
            static void CaidaJugador(int jugador);
    //--------------
      
      
      
           //Multiplicador para controlar la velocidad del juego
            static const float VEL_JUEGO=0.05;
      
      
            static bool logicaActiva ;
            static void Inicializar();
            static void Finalizar();
            
            static Jugador * GetJugador(int numJugador);
      //API de unidades
      
            //Bucle principal que recorre las unidades y actualiza sus acciones
            //Notificar de cambios de posicion
            static void Actualizar();
      
            //Crea y destruye unidades, notificando a Red y Gráfico
            static int CrearUnidad(int tipo, int numJugador, float x, float z);
            static bool DestruirUnidad(int identificador);
            
            // Crea un nuevo edificio (pero pendiente de construir)
            static int ConstruirEdificio(int tipo, int numJugador, float x, float z);
            
            //Para debug
            static void ImprimirUnidades();
            
            //Métodos de comunicación con unidades
            static Util::Vector2D GetPosUnidad(int unidad) { return unidades[unidad]->getPosicion(); }
            //static Cubo getDimsUnidad(int unidad);
            static int GetVida(int unidad) { unidades[unidad]->getVida(); }
            static int GetEquipo(int unidad) { unidades[unidad]->getEquipo(); }
            
            //Funciones para buscar unidades en el radio que se indica
            //alrededor de la unidad de origen
            //Se aplicarán optimizaciones en la busqueda, organizando
            //el terreno en una rejilla, que permitirá muchas unidades
            static void BuscarUnidadesBegin(int unidad,float radio) { BuscarUnidadesBegin( unidades[unidad], radio ); }
            static void BuscarUnidadesBegin(const Unidad * unidad,float radio);
            static int BuscarUnidadesNext();
            static bool BuscarUnidadesEnd();
            
            static Unidad* GetUnidad(int u) { return unidades[u]; }
            
            //METODO PARA PRUEBAS
            static vector<Unidad*> GetListaUnidades()
            {
                   vector<Unidad*> uni;
                   for (int i=0;i<unidades.size();i++) 
                     if (unidades[i]!=NULL)
                       uni.push_back(unidades[i]);
                       
                   return uni; 
            }
            
            static std::vector<int> unidades_encontradas;
            
      //----------------
      
      //API de IA
            //Funciones de terreno
            static void CargarMapaAlturas();
            static void CargarMapaDurezas();
            static int GetAlturaMapa(float x,float y);
            static bool HayDureza(float x,float y);
            static bool HayDureza(float x,float y,float x2,float y2);
            static bool HayDureza(const Util::Vector2D &a,const Util::Vector2D &b)
            {
                   return HayDureza(a.getX(),a.getY(),b.getX(),b.getY());
            }
            static float  UltimaColision();
            static void RestringirPenetracionConUnidades( Unidad* );
      //----------------
      
      //API de mensajes
        static bool EnviarMensaje(double      retardo,
                   int         origen,
                   int         destino,
                   int         msg,
                   void*       datos)
        {
                   return mensajeria->EnviarMensaje(retardo,origen,destino,msg,datos);
        }
      //--------------
      
      
      private:
              //Datos de la lógica
              static int numCiclo ;
              static const int ciclosDia = 1200 ;
              static const int ciclosAnyo = 6000 ;              
              
              static std::vector<Unidad *> unidades;
                          
              static vector <Jugador *> jugadores ;
              
              static bool partidaEmpezada ;
              
              static vector <unsigned char> mapa_durezas;
              static int ancho_mapa_durezas;
              static int alto_mapa_durezas;
              static vector <unsigned char> mapa_alturas;
              static int ancho_mapa_alturas;
              static int alto_mapa_alturas;       
              
              //Utilizado por HayDureza para guardarse las iteraciones que 
              //ha llevado la ultima deteccion de colision, consultable por
              //UltimaColision()
              static int ultima_distancia_colision;
              
              
              //Centro de mensajes
              static CentroDeMensajes * mensajeria;
      
};

}

#endif
