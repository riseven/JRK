
#include "lApi.h"
#include "uFrecEv.h"
#include "lIAUnidadMovil.h"


using namespace Logica;

IAUnidadMovil::IAUnidadMovil(Unidad* uni):IAUnidad(uni)
{
          frecevHaLlegado.setFrec(0.8);
          frecevColision.setFrec(0.3);
          destino.Cero();
          dir.setX(0); // Por defecto las unidades miran
          dir.setY(-1);// hacia el norte
}

IAUnidadMovil::~IAUnidadMovil()
{

}


void
IAUnidadMovil::Actualizar()
{

        
}



//Acciones principales

/*
 * Movimiento hacia un destino fijo en linea recta
 */
void
IAUnidadMovil::PasoAndar()
{
       dir=AuxMover(destino);
       angulo=AuxCalcularAngulo(dir);        
       velocidad=1;
}

/*
 * Igual que PasoAndar, simplemente se devuelve una velocidad simbólica mayor
 *
 */
void
IAUnidadMovil::PasoCorrer()
{
       dir=AuxMover(destino);
       angulo=AuxCalcularAngulo(dir);     
       velocidad=2;
}
/*
 * Accion instantánea, se devuelve velocidad 0
 *
 */
void
IAUnidadMovil::PasoParar()
{
     velocidad=0;
}


//Acciones auxiliares

/*
 * Calcula el vector de direccion entre la unidad y el destino
 *
 */
Vector2D
IAUnidadMovil::AuxMover(const Vector2D &dest)
{
   return dest-unidad->getPosicion();
}

void
IAUnidadMovil::AuxCorregirDir(Vector2D &dir)
{
}

/*
 *Devuelve angulo en grados
 */
float
IAUnidadMovil::AuxCalcularAngulo(const Vector2D & p)
{
   return RADTODEG( atan( p.getY()/p.getX() ) ) + ((p.getX()<0)?180:0);  
}

//Eventos de movimiento

bool
IAUnidadMovil::eventoHaLlegado()
{
   if (frecevHaLlegado.comprobar())
   {
     //Codigo de comprobacion
     if (dir.LongitudCuad()<UMBRAL_DIST_CUAD)
         return true;
     else
         return false;
     
   }else
        return false;
}

bool
IAUnidadMovil::eventoColision()
{
   if (frecevColision.comprobar())
   {
     //Codigo de comprobacion
     return false;
     
   }else
        return false;
}
