#include "iCamara.h"
#include "gApi.h"
#include "eExcepcionLocalizada.h"
#include <cmath>

namespace Interfaz
{

//Parametros por defecto de la camara
Camara::Camara()
{
    x = 100 ;
    z = 100 ;
    
    posicion[0] = 0 ;
    posicion[1] = 100 ;
    posicion[2] = 0 ;
    posicion[3] = 90 ;
    
    deseado[0] = 0 ;
    deseado[1] = 50 ;
    deseado[2] = 0 ;
    deseado[3] = 90 ;
    
    velocidad[0] = 0 ;
    velocidad[1] = 0 ;
    velocidad[2] = 0 ;
    velocidad[3] = 0 ;
    
    aceleracion[0] = 0.1 ;
    aceleracion[1] = 0.1 ;
    aceleracion[2] = 0.1 ; 
    aceleracion[3] = 0.15 ;
}

Camara::~Camara()
{
}

//Cambia la posicion de la camara
void
Camara::SetPosicionDeseada(int x, int z)
{
    try
    {
        this->x = x ;
        this->z = z ;
        FijarPosicion();
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::Camara::SetPosicionDeseada(int, int)").Combinar(ex);
    }
}

//Cambia inclinacion de la camara un nº d grados
void
Camara::SetInclinacion(int angulo)
{
    deseado[3] = angulo ;
    FijarInclinacion();
}

//Desplaza la camara hacia el borde de la pantalla indicado
//Es posible hacer desplazamientos hacia 2 bordes a la vez
//aplicando una mascara
void
Camara::DesplazamientoBordes(int bordes)
{
    try
    {
        if ( bordes & BordeSuperior )
        {
            z-- ;
        }
        if ( bordes & BordeInferior )
        {
            z++ ;
        }
        if ( bordes & BordeDerecho )
        {
            x++ ;
        }
        if ( bordes & BordeIzquierdo )
        {
            x-- ;
        }
        //Fija posicion de la camara
        FijarPosicion();
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::Camara::DesplazamientoBordes(int)").Combinar(ex);
    }
}

//Inclinacion de la camara por pasos
void
Camara::InclinarCamara(int difPasos)
{
    deseado[3] += difPasos * 5.0 ;
    FijarInclinacion();
}

// Realiza movimiento suave de la camara a la posicion deseada y
// para esto realiza un movimiento uniformemente acelerado en cada
// dimensión, hasta la mitad del recorrido, momento en el cual
// el movimiento es decelarado para alcanzar el punto final
// suavemente.
// Por esto, para un movimiento sencillo, la función de posicion
// respecto al tiempo tiene la forma de medio periodo de una funcion
// sinusoidal.
// Puesto que se puede cambiar la posicion deseada antes de alcanzar
// la posicion deseada anterior, en realidad lo que se hace es 
// empezar a desacelerar el movimiento a partir de cuando se calcula 
// que actuando así la camara va a llegar a la posicion deseada, para
// esto partiendo de la ecuación del movimiento del MUA, se tiene que
// cumplir que la ecuación tenga una o dos soluciones, es decir, el
// discriminante (argumento de la raiz cuadrada) sea mayor o igual a
// 0, ya que no tiene sentido físico la solucion compleja. 
// El discriminante es (vel^2)-2*acel*(posIni-posFinal).
// La inclinación se trata con el mismo procedimiento.

void
Camara::Actualizar()
{
    try
    {
        deseado[0] = x * Graficos::Api::GetEscalaTerreno() ;
        deseado[2] = z * Graficos::Api::GetEscalaTerreno() ;
        deseado[2] += ((90.0 - deseado[3])/50.0)*20.0 ;
        //Obtiene la altura del terreo en la posicion indicada
        deseado[1] = Graficos::Api::GetAltura( (int)(posicion[0]/Graficos::Api::GetEscalaTerreno()), (int)(posicion[2]/Graficos::Api::GetEscalaTerreno()) ) ;
        deseado[1] += 30.0 ;
        deseado[1] -= ((90.0 - posicion[3])/50.0)*15.0 ;
        
        // Comprobamos el warp
        /*
        if ( fabs(deseado[0] - posicion[0]) > 30 ||
             fabs(deseado[2] - posicion[2]) > 30 )
        {
            posicion[0] = deseado[0] ;
            posicion[1] = deseado[1] ;
            posicion[2] = deseado[2] ;
        }
        */
        
        //Para todas las coordenadas (x,y,z)
        for ( int i = 0 ; i < 4 ; i++ )
        {
            //Si la posicion actual es distinta de la deseada
            if ( posicion[i] != deseado[i] )
            {
                if ( (deseado[i] - posicion[i])*velocidad[i] < 0.0 )
                {
                    velocidad[i] = 0.0 ;
                }
                
                if ( (deseado[i] - posicion[i]) > 0.0 )
                {
                    if ( (velocidad[i]*velocidad[i] + 2*aceleracion[i]*(posicion[i]-deseado[i])) >= 0.0 )
                    {
                        velocidad[i] -= aceleracion[i] ;
                    }
                    else
                    {
                        velocidad[i] += aceleracion[i] ;
                    }
                }else
                {
                    if ( (velocidad[i]*velocidad[i] -2*aceleracion[i]*(posicion[i]-deseado[i])) >= 0.0 )
                    {
                        velocidad[i] += aceleracion[i] ;
                    }
                    else
                    {
                        velocidad[i] -= aceleracion[i] ;
                    }
                }
            }
            
            if ( (deseado[i]-posicion[i])*(deseado[i]-(posicion[i]+velocidad[i])) <= 0.0 )
            {
                velocidad[i] = 0.0 ;
                posicion[i] = deseado[i] ;
            }else
            {
                posicion[i] += velocidad[i] ;
            }    
        }
    
        //Actualizo posicion e inclinacion de la camara
        
        Graficos::Api::SetPosicionCamara(posicion[0], posicion[1], posicion[2]);
        Graficos::Api::SetInclinacionCamara(posicion[3]);    
        
        
        // Realizo las transformacions opengl de camara
        Graficos::Api::ResetCamara();
        Graficos::Api::RotarCamara(posicion[3], 1, 0, 0);
        Graficos::Api::TrasladarCamara(-posicion[0], -posicion[1], -posicion[2]);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::Camara::Actualizar()").Combinar(ex);
    }
}


//Fija la camara en una posicion válida del mapa
void 
Camara::FijarPosicion()
{
    try
    {
        if ( x < 5 ) x = 5 ;
        if ( z < 5 ) z = 5 ;
        if ( x > Graficos::Api::GetAnchoTerreno() - 5 ) x = Graficos::Api::GetAnchoTerreno() - 5 ;
        if ( z > Graficos::Api::GetAltoTerreno() - 11 ) z = Graficos::Api::GetAltoTerreno() - 11 ;
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::Camara::FijarPosicion()").Combinar(ex);
    }
}

//Fija la camara con una inclinacion válida
void
Camara::FijarInclinacion()
{
    if ( deseado[3] < 40 )
    {
        deseado[3] = 40 ;
    }
    if ( deseado[3] > 90 )
    {
        deseado[3] = 90 ;
    }
}

void
Camara::ForzarPosicion()
{
    deseado[0] = x * Graficos::Api::GetEscalaTerreno() ;
    deseado[2] = z * Graficos::Api::GetEscalaTerreno() ;
    deseado[2] += ((90.0 - posicion[3])/50.0)*20.0 ;    
    //Obtiene la altura del terreo en la posicion indicada
    deseado[1] = Graficos::Api::GetAltura( (int)(posicion[0]/Graficos::Api::GetEscalaTerreno()), (int)(posicion[2]/Graficos::Api::GetEscalaTerreno()) ) ;
    deseado[1] += 30.0 ;
    deseado[1] -= ((90.0 - posicion[3])/50.0)*15.0 ;
    
    posicion[0] = deseado[0] ;
    posicion[1] = deseado[1] ;
    posicion[2] = deseado[2] ;
}

}
