
#include "logica.h"
#include "lUnidadSoldado.h"
#include "lApi.h"


using namespace Logica;
using namespace Util;


UnidadSoldado::UnidadSoldado(int ID, Vector2D pos, int eq):UnidadDinamica(ID,
                                                                  pos,
                                                                  VIDAMAXIMA,
                                                                  FUERZAMAXIMA,
                                                                  VELOCIDADMAXIMA,
                                                                  VELGIROMAXIMA,
                                                                  CAMPODEVISION,
                                                                  DISTANCIADEVISION,
                                                                  eq,
                                                                  MASA)
{ 
   //!Por ahora se asocia con IA Movil, de los parámetros que se tengan que pasar
   //para realizar acciones de movimiento como el destino, se ocupará la Api
   //en el momento de actualizar
   asociaIA(0);
   
   Vector2D dir(rand(),rand());
   dir.Normalizar();
   setDireccion(dir);
   distanciaDefensa = DISTANCIADEFENSA;
   
   setRadio(0.5);
}


UnidadSoldado::~UnidadSoldado()
{
   //Obligatorio
   quitaIA();
}
      
/*
 * Bucle principal de la unidad
 */
void
UnidadSoldado::Actualizar()
{
   //Llamar a ActualizarIAUnidad cuando sea necesario
   ActualizarIAUnidad();
 
/*   //Cuando se actualiza la IA recalcular el vector de incremento
   //aplicando la velocidad de la unidad a la velocidad simbólica
   //de la IA (0,1,2,3)
   if (!velocidad.esCero())
   {  
      //Calculamos el incremento 
      float rad=DEGTORAD(angulo);
      direccion.setX(velocidad.Longitud()*Api::VEL_JUEGO*cos(rad));
      direccion.setY(velocidad.Longitud()*Api::VEL_JUEGO*sin(rad));
      posicion+=direccion;

   }*/
}
