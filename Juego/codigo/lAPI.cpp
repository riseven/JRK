
#include "lUnidad.h"
#include "lUnidadSoldado.h"
#include "lCuartelGeneral.h"
#include "lBarracones.h"
#include "gApi.h"
#include "iApi.h"
#include "rAPI.h"
#include "lAPI.h"
#include "eExcepcionFichero.h"
#include "eExcepcionParametro.h"
#include "eExcepcionLocalizada.h"
#include "rProtocolo.h"
#include "lPetroleo.h"
#include "lPozoDeExtraccion.h"

using namespace Logica;
using namespace Util;

std::vector<Unidad *> Api::unidades;
std::vector<int> Api::unidades_encontradas;

vector <Jugador *> Api::jugadores ;

bool Api::logicaActiva = false ;
bool Api::partidaEmpezada = false ;

vector <unsigned char> Api::mapa_durezas;
int Api::ancho_mapa_durezas;
int Api::alto_mapa_durezas;
vector <unsigned char> Api::mapa_alturas;
int Api::ancho_mapa_alturas;
int Api::alto_mapa_alturas;
int Api::ultima_distancia_colision;


CentroDeMensajes * Api::mensajeria;
int Api::numCiclo ;

void
Api::Inicializar()
{
                      
    CargarMapaAlturas();
    CargarMapaDurezas();
    
    //Se  prepara el vector de jugadores
    vector <Jugador *> temp( Interfaz::Api::GetMaxJugadores() );
    jugadores = temp ;
    for ( vector <Jugador *>::iterator i = jugadores.begin() ; i != jugadores.end() ; ++i )
    {
        (*i) = NULL ;
    }
    
    partidaEmpezada = false ;
  
    //Se crea un nuevo centro para enviar mensajes
    mensajeria= new CentroDeMensajes();
    
    logicaActiva = true ;

}

void
Api::EmpezarPartida()
{
    if ( partidaEmpezada )
    {
        return ;
    }
    partidaEmpezada = true ;
    numCiclo = 0 ;
    
    Red::Api::GetApiInterfaz()->EmpezarPartida();

    int maxJugadores = Interfaz::Api::GetInfoMapa()->GetMaxJugadores();
    // Guardamos que posiciones de inicio hemos gastado
    vector<bool> usados(maxJugadores);
    for ( vector<bool>::iterator i = usados.begin() ; i != usados.end() ; ++i )
    {
        *i = false ;
    }

    for ( int i = 0 ; i < jugadores.size()  ; ++i )
    {
        if ( jugadores.at(i) != NULL )
        {
            // Buscamos un hueco
            int hueco ;
            do
            {
                hueco = rand()%maxJugadores ;
            }while ( usados.at(hueco) );
            usados.at(hueco) = true ;
            
            // Obtenemos las coordenadas
            Util::Vector2D pos = Interfaz::Api::GetInfoMapa()->GetPosicionInicio(hueco);
            float x = pos.getX() * Graficos::Api::GetEscalaTerreno();
            float y = pos.getY() * Graficos::Api::GetEscalaTerreno();
                
            CrearUnidad( Red::Protocolo::Unidad::CuartelGeneral, i, x, y);
        }
    }
    
    for ( int i = 0 ; i < Interfaz::Api::GetInfoMapa()->GetNumPetroleos() ; i++ )
    {    
        Util::Vector2D pos = Interfaz::Api::GetInfoMapa()->GetPosicionPetroleo(i) ;
        float x = pos.getX() * Graficos::Api::GetEscalaTerreno() + Graficos::Api::GetEscalaTerreno()/2;
        float y = pos.getY() * Graficos::Api::GetEscalaTerreno() + Graficos::Api::GetEscalaTerreno()/2;
        CrearUnidad( Red::Protocolo::Unidad::Petroleo, -1, x, y);
    }
}

void
Api::CaidaJugador(int jugador)
{
    if ( partidaEmpezada )
    {
        return ;
    }
    
    // El jugador abandona antes de empezar la partida, por lo que le quitamos
    delete jugadores.at(jugador);
    jugadores.at(jugador) = NULL ;
    
    // Notificamos el abandono    
    Red::Api::GetApiInterfaz()->AbandonoJugador(jugador);
    
}

void
Api::Finalizar()
{
    mapa_alturas.clear();
    mapa_durezas.clear();
    
    if (mensajeria) delete mensajeria;
    
    logicaActiva = false ;
    
    for (int i=0;i<unidades.size();i++)
    {
        if ( unidades.at(i) != NULL )
        {
            DestruirUnidad(i);
        }
    }
}

Jugador *
Api::GetJugador(int numJugador)
{
    if ( jugadores.at(numJugador) == NULL )
    {
        throw Error::ExcepcionLocalizada("Logica::Api::GetJugador(int)");
    }
    return jugadores.at(numJugador);
}

void
Api::UnirsePartida(int numJugador, string nombreJugador)
{
    int pos = numJugador ;
    if ( pos >= jugadores.size() )
    {
        throw Error::ExcepcionLocalizada("Logica::Api::UnirsePartida(int, string)");
    }
    
    jugadores.at(pos) = new Jugador(nombreJugador);
    
    // Buscamos un color para el jugador
    int color = 0 ;
    while ( true )
    {
        for ( int i = 0 ; i < jugadores.size() ; i++ )
        {
            if ( i != pos && jugadores.at(i) != NULL )
            {
                if ( jugadores.at(i)->GetColor() == color )
                {
                    color++ ;
                    continue ;
                }
            }
        }
        jugadores.at(pos)->SetColor(color) ;
        break ;
    }


    // Notificamos jugadores pre-existentes al nuevo jugador   
    Red::Api::GetApiInterfaz()->SetMascaraEnvio(1<<pos) ;
    for ( int i = 0 ; i < jugadores.size() ; i++ )
    {
        if ( i != pos && jugadores.at(i) != NULL )
        {
            Red::Api::GetApiInterfaz()->NuevoJugador(i, jugadores.at(i)->GetNombre());
            Red::Api::GetApiInterfaz()->SetColorJugador(i, jugadores.at(i)->GetColor());
        }
    }
    // Notificamos al nuevo jugador cual es su posicion
    Red::Api::GetApiInterfaz()->SetJugadorPropio(pos);
    // Notificamos al nuevo jugador cual es el mapa de la partida
    Red::Api::GetApiInterfaz()->SetMapa( Interfaz::Api::GetInfoMapa()->GetNombreFichero() );

    // Notificamos a todos los jugadores (incluido el nuevo) la conexion del nuevo
    Red::Api::GetApiInterfaz()->SetMascaraEnvio(0xFFFFFFFF);
    Red::Api::GetApiInterfaz()->NuevoJugador(pos, nombreJugador);
    Red::Api::GetApiInterfaz()->SetColorJugador(pos, color);
} 

void
Api::CambiarColor(int numJugador)
{
    int color = jugadores.at(numJugador)->GetColor() ;
    
    color = (color+1)%8 ;
    
    while ( true )
    {
        for ( int i = 0 ; i < jugadores.size() ; ++i )
        {
            if ( i != numJugador && jugadores.at(i) != NULL )
            {
                if ( jugadores.at(i)->GetColor() == color )
                {
                    color = (color+1)%8 ;
                    continue ;
                }
            }
        }
        break ;
    }
    
    // Establecemos el nuevo color
    jugadores.at(numJugador)->SetColor(color);
    
    // Notificamos a todo el mundo el cambio de color
    Red::Api::GetApiInterfaz()->SetColorJugador(numJugador, color);
}

/*
 * Bucle principal de la lógica
 * (El diseño de pasar comandos a la lógica será
 *  sustituido por uno más eficiente en un futuro)
 */
void
Api::Actualizar()
{
    if ( logicaActiva )
    {
                if ( partidaEmpezada )
        {
            if ( numCiclo % ciclosDia == 0 )
            {
                Red::Api::GetApiInterfaz()->SetDia(true);
            }
            if ( numCiclo % ciclosDia == ciclosDia/2 )
            {
                Red::Api::GetApiInterfaz()->SetDia(false);
            }   
            
            if ( numCiclo % ciclosAnyo == 100+ciclosDia/2 )
            {
                Red::Api::GetApiInterfaz()->SetInvierno(false);
            }
            if ( numCiclo % ciclosAnyo == ciclosAnyo/2 + 100 +ciclosDia/2 )
            {
                Red::Api::GetApiInterfaz()->SetInvierno(true);
            }  
            
            numCiclo++ ;
                
            
            
            // Actualizamos el dinero
            for ( int i = 0 ; i < jugadores.size() ; i++ )
            {
                if ( jugadores.at(i) != NULL )
                {
                    jugadores.at(i)->AddDinero(0.05) ;
                    Red::Api::GetApiInterfaz()->SetDinero( i, jugadores.at(i)->GetDinero() );
                }
            }
            
        }
        
        
        
       Unidad *uni;
       Vector2D pos,temp;
       
       //Bucle de unidades
       for (int i=0;i<unidades.size();i++)
       {
          if (unidades[i]!=NULL && GetVida(i)>0)
          {
                //Cambia la posicion de la unidad si esta solapando
                //con otras1
                RestringirPenetracionConUnidades(unidades[i]);
                
                temp=unidades[i]->getPosicion(); 
                unidades[i]->Actualizar();
                pos=unidades[i]->getPosicion();  
                
          
                if (temp!=pos || true)//Notificar cambio de posicion a la interfaz
                {
                    float escala = Graficos::Api::GetEscalaTerreno();
                    float altura = Graficos::Api::GetAlturaSuave(pos.getX(), pos.getY());
                    
                    Red::Api::GetApiInterfaz()->SetPosicionUnidad(i, pos.getX(), altura , pos.getY());
                    //Red::Api::GetApiInterfaz()->SetPosicionUnidad(i, pos.getX(), 20.0, pos.getY());
                    Red::Api::GetApiInterfaz()->SetAnguloUnidad(i,unidades[i]->getDireccion().AnguloDEG());
                   //Interfaz::Api::MoverUnidad(i,pos.getX(),20,pos.getY());
                }
          }else//Si se destruye directamente puede dar problemas si las IA's 
               //no saben que ya no existe.
               DestruirUnidad(i);
               //Notificar a la Interfaz
       
       }
    }
 
}

//------------------------

      
//API de unidades
int
Api::CrearUnidad(int tipo, int numJugador, float x, float z)
{
   Unidad * uni=NULL;
   int i,final;
   Vector2D pos(x,z);

   //Buscar el primer hueco en el vector para insertar
   for (i=0;i<unidades.size();i++)
       if (unidades[i]==NULL)
       {
          final=i;
          break;
       }
   //Si no hay huecos, al final
   if (i>=unidades.size())
   {
         final=unidades.size();
         unidades.push_back(NULL);
   }


    switch(tipo)
    {
    case Red::Protocolo::Unidad::Petroleo:
        uni = new Petroleo(final, pos, numJugador);
        break ;        
    case Red::Protocolo::Unidad::Soldado:
        uni = new UnidadSoldado(final,pos, numJugador);
        break;
    case Red::Protocolo::Unidad::Ingeniero:
        uni = new UnidadSoldado(final,pos,numJugador);
        //tipo = Red::Protocolo::Unidad::Soldado ;
        break ;
    case Red::Protocolo::Unidad::CuartelGeneral:
        uni = new CuartelGeneral(final, pos, numJugador);
        break;        
    case Red::Protocolo::Unidad::Barracones:
        uni = new Barracones(final, pos, numJugador);
        break;
        
    //Creacion de proyectiles
    
    case Red::Protocolo::Proyectiles::Bala:
        //uni = new Bala( ); 
        break;  
    }
   unidades[final] = uni ;
   //Notificar al módulo de gráficos de la existencia de una nueva unidad
   //Se le pasa un identificador de la unidad para posteriores cambios
   if (uni)
   {       
           //Notificación de nueva unidad
           //Red::Api::GetApiInterfaz()->CrearUnidad(final,tipo,x, Api::GetAlturaMapa(pos.getX(), pos.getY()) /10.0, z);           
            Red::Api::GetApiInterfaz()->CrearUnidad(final,numJugador,tipo,x, 20, z);                      
   }
   else
   {
        throw Error::ExcepcionParametro("Logica::Api::CrearUnidad(int, float, float)", 1, "Unidad no reconocida");
    }
   
   return (uni?final:-1);
}

bool
Api::DestruirUnidad(int identificador)
{
  //No se comprueba que el identificador es válido
  Unidad* uni = unidades[identificador];
  if (uni)
  {
          delete uni;
          unidades[identificador]=NULL; //Marcar como no existente
          //Si es el ultimo sacarlo
          if ((unidades.size()-1)==identificador) unidades.pop_back();          
          
          return true;
  }
  else
       return false;
}

int
Api::ConstruirEdificio(int tipo, int numJugador, float x, float z)
{
   Unidad * uni=NULL;
   int i,final;
   Vector2D pos(x,z);

   //Buscar el primer hueco en el vector para insertar
   for (i=0;i<unidades.size();i++)
       if (unidades[i]==NULL)
       {
          final=i;
          break;
       }
   //Si no hay huecos, al final
   if (i>=unidades.size())
   {
         final=unidades.size();
         unidades.push_back(NULL);
   }

    switch(tipo)
    {
    case Red::Protocolo::Unidad::CuartelGeneral:
        uni = new CuartelGeneral(final, pos, numJugador);
        Api::GetJugador(uni->getEquipo())->AddDinero(-1000.0);
        break;        
    case Red::Protocolo::Unidad::Barracones:
        uni = new Barracones(final, pos, numJugador);
        Api::GetJugador(uni->getEquipo())->AddDinero(-350.0);
        break;
    case Red::Protocolo::Unidad::PozoDeExtraccion:
        // Primero se comprueba que sea una posicion valida.
        float escala = Graficos::Api::GetEscalaTerreno();
        for ( int i = 0 ; i < Interfaz::Api::GetInfoMapa()->GetNumPetroleos() ; i++ )
        {
            Util::Vector2D pet = Interfaz::Api::GetInfoMapa()->GetPosicionPetroleo(i);
            if ( pet.getX() == ((int)(pos.getX()/escala)) && pet.getY() == ((int)(pos.getY()/escala)) )
            {
                if ( !Interfaz::Api::GetInfoMapa()->GetPetroleoOcupado(i) )
                {
                    // Se construye
                    uni = new PozoDeExtraccion(final, pos, numJugador);
                    Api::GetJugador(uni->getEquipo())->AddDinero(-500.0);
                    Red::Api::GetApiInterfaz()->SetOcupadoPetroleo(i, true);
                    break ;
                }
            }
        }
        break ;
    }
   unidades[final] = uni ;
   //Notificar al módulo de gráficos de la existencia de una nueva unidad
   //Se le pasa un identificador de la unidad para posteriores cambios
   if (uni)
   {       
           //Notificación de nueva unidad
           //Red::Api::GetApiInterfaz()->CrearUnidad(final,tipo,x, Api::GetAlturaMapa(pos.getX(), pos.getY()) /10.0, z);           
            Red::Api::GetApiInterfaz()->CrearUnidad(final,numJugador,tipo,x, 20, z);                      
   }
   else
   {
        throw Error::ExcepcionParametro("Logica::Api::ConstruirEdificio(int, int, float, float, int, int *)", 1, "Unidad no reconocida");
    }
   
   return (uni?final:-1);
}

void
Api::ImprimirUnidades()
{
  for (int i=0;i<unidades.size();i++)
      if (unidades[i])
      {
             Vector2D v;
             v=unidades[i]->getPosicion();
             printf("Unidad %d: Pos (%f,%f)\n",i,v.getX(),v.getY());
      }
             
}

//----------------


/*
  Busqueda de unidades
  -Muy optimizable organizando el terreno en una rejilla
*/
void
Api::BuscarUnidadesBegin(const Unidad * unidad,float radio)
{
  unidades_encontradas.clear();
  
  Vector2D p = unidad->getPosicion();
  
  //OPTIMIZAR !
  for (int i=0;i<unidades.size();i++)
    if ( unidades[i]!=NULL && unidades[i]!=unidad && (radio==0 || ((unidades[i]->getPosicion() - p).Longitud()) <= radio ) )       
           unidades_encontradas.push_back(i);
}

int
Api::BuscarUnidadesNext()
{
  int uni=unidades_encontradas[unidades_encontradas.size() - 1];
  unidades_encontradas.pop_back();
  
  return uni;
}

bool
Api::BuscarUnidadesEnd()
{
  return unidades_encontradas.size()==0;
}


//Funciones de terreno

void
Api::CargarMapaAlturas()
{
    mapa_alturas.clear();
	
    BITMAP * imagen=NULL;
    imagen = load_bitmap(Interfaz::Api::GetInfoMapa()->GetFicheroAlturas().c_str(),NULL);
    
     if (!imagen) throw Error::ExcepcionFichero("Logica::Api::CarcarMapaAlturas:Fallo cargando imagen");
    
    mapa_alturas.resize( imagen->w * imagen->h );
    ancho_mapa_alturas = imagen->w;
    alto_mapa_alturas = imagen->h;
    
    for(int j=0; j<imagen->w; j++)
       for(int i=0; i<imagen->h; i++)
          mapa_alturas[j*imagen->w + i]=getr32(getpixel(imagen, i, j));
          
    destroy_bitmap(imagen);
}

void
Api::CargarMapaDurezas()
{
    mapa_durezas.clear();
	
    BITMAP * imagen=NULL;	
    // = load_bitmap(Interfaz::Api::GetInfoMapa()->GetFicheroDurezas().c_str(),NULL);
    //De prueba mientras pueda cargarla desde el infomapa...
    imagen=load_bitmap("Mapas\\MapaDePrueba\\durezas.bmp",NULL);
    
    if (!imagen) throw Error::ExcepcionFichero("Logica::Api::CarcarMapaDurezas","Fallo cargando imagen");
    
    mapa_durezas.resize( imagen->w * imagen->h );
    ancho_mapa_durezas = imagen->w;
    alto_mapa_durezas = imagen->h;
    
    for(int j=0; j<imagen->h; j++)
      for(int i=0; i<imagen->w; i++)
          mapa_durezas[j*imagen->w + i]=getr32(getpixel(imagen, i, j));


    destroy_bitmap(imagen);
}

int
Api::GetAlturaMapa(float x,float y)
{
    if ( x >= 200 || x < 0 || y >= 200 || y < 0 )
    {
        return 0 ;
    }
   float escala = Graficos::Api::GetEscalaTerreno();
   
   return mapa_alturas[ (int)((y/escala)*ancho_mapa_alturas + (x/escala)) ];
}

/*
 * Devuelve true si el punto marcado
 * corresponde con una dureza del mapa (Zona no accesible)
 */
bool
Api::HayDureza(float x,float y)
{
    
  float escala = Graficos::Api::GetEscalaTerreno();
  int ix = (int)(x/escala);
  int iy = (int)(y/escala);

  return (ix<0 || ix>=ancho_mapa_durezas || iy<0 || iy>=alto_mapa_durezas ||mapa_durezas[ iy*ancho_mapa_durezas + ix ]!=0);
}

/*
 * Devuelve true si la linea formada por los puntos
 * atraviesa alguna dureza
 *
 */
bool
Api::HayDureza(float fx0,float fy0,float fx1,float fy1)
{
    float escala = Graficos::Api::GetEscalaTerreno();
    int x0 = (int)(fx0/escala);                 
    int y0 = (int)(fy0/escala);
    int x1 = (int)(fx1/escala);
    int y1 = (int)(fy1/escala);
    bool steep = (abs(y1 - y0) > abs(x1 - x0));
    int xstep,ystep;
    
    if (steep)
    {
         SWAP(x0, y0)
         SWAP(x1, y1)
    }
     int deltax = abs(x1 - x0);
     int deltay = abs(y1 - y0);
     int error = 0;
     int deltaerr = deltay;
     int x = x0;
     int y = y0;
     
     ultima_distancia_colision=0;
     
     if (x0 < x1) 
       xstep = 1;
     else 
       xstep = -1;
       
     if (y0 < y1)
       ystep = 1;
     else 
       ystep = -1;
       
     while (x != x1)
     {
         ultima_distancia_colision++;
         if (steep)
         {
               if ( x<0 || x>=ancho_mapa_durezas || mapa_durezas[ x*ancho_mapa_durezas + y ]!=0)
                 return true;
         }else
         {
               if ( y<0 || y>=alto_mapa_durezas || mapa_durezas[ y*ancho_mapa_durezas + x ]!=0)
                 return true;
         }
         
         x = x + xstep;
         error = error + deltaerr;
         if (2*error >= deltax)
         {
             y = y + ystep;
             error = error - deltax;
         }
     }
     
     return false;
}

float
Api::UltimaColision()
{ 
       return Graficos::Api::GetEscalaTerreno()*ultima_distancia_colision;
}

//Evita que la unidad quede solapada por las demas, para ello cambia
//su posicion si es una unidad movil
void
Api::RestringirPenetracionConUnidades (Unidad * uni)
{
  for (int i=0; i<unidades.size(); i++)
  {
    const Unidad * const uniDest = unidades[i];
    
    //Si no somos nosotros y no es una unidad fija
    if (uniDest!=NULL && uniDest != uni &&  !uni->esFija() )
    {

        //Vector distancia entre cada dos unidades
        Vector2D vdist =  uni->getPosicion() - uniDest->getPosicion();
    
        //Calculamos la longitud al cuadrado para evitar raices
        double distCuad = vdist.LongitudCuad();
    
        //Cantidad de solape 
        double radios = uniDest->getRadio() + uni->getRadio();
        double radiosCuad = radios*radios;
    
        //Si la distancia real es menor que la distancia necesaria
        //calcular la nueva posicion
        if (radiosCuad-0.00001 > distCuad)
        {
          double dist  = sqrt(distCuad);
          double solape = radios - dist;
          //mover la unidad fuera utilizando el vector de distancia y la
          //cantidad de solape
          
          //cout <<"pos 1:" <<uni->getPosicion() <<endl; 
          //cout <<"pos 2:" <<uniDest->getPosicion() << endl;
          
          uni->setPosicion(uni->getPosicion() + (vdist/dist) * (solape*1.00001));

        }
     }
   }
}
