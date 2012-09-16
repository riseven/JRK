#include "iUnidad.h"

#include "gApi.h"
#include "iUnidadSeleccionable.h"
#include "iPetroleo.h"
#include "iApi.h"

namespace Interfaz
{

bool Unidad::algunaUnidadSeleccionada ;
list <Unidad *> Unidad::unidades ;

void
Unidad::Inicializar()
{
    algunaUnidadSeleccionada = false ;  
    unidades.clear();  

    Api::SetProgresoCarga( Api::GetProgresoCarga()+1.0 );
    Graficos::Api::SetPorcentajeBarraVida(Api::GetBarraProgresoCarga(), Api::GetProgresoCarga());
    Graficos::Api::Pintar();

    Petroleo::Inicializar();

    UnidadSeleccionable::Inicializar(); 

}

void
Unidad::Cerrar()
{
    algunaUnidadSeleccionada = false ;    
    DestruirUnidades();
    UnidadSeleccionable::Cerrar();
}

void
Unidad::Reset()
{
    DestruirUnidades();
    
    UnidadSeleccionable::Reset();
    
    unidades.clear();
    algunaUnidadSeleccionada = false ;
}

void
Unidad::ActualizarUnidades()
{
    if ( algunaUnidadSeleccionada )
    {
        UnidadSeleccionable::ActualizarUnidadesSeleccionables() ;
    }
} 

void
Unidad::DestruirUnidades()
{
    while ( unidades.size() > 0 )
    {
        delete (unidades.front());
        // No hay que extraerlo de la lista de unidades, puesto que se hace en el
        // destructor de la unidad.
    }
}

void
Unidad::SetAlgunaUnidadSeleccionada(bool algunaUnidadSeleccionada)
{
    Unidad::algunaUnidadSeleccionada = algunaUnidadSeleccionada ;
}

//Posicion por defecto de la unidad
Unidad::Unidad()
{
    unidades.push_back(this);
    
    x = 0.0f ;
    y = 0.0f ;
    z = 0.0f ;
    
    objetoHandler = -1 ;
}     

//Detruye unidad
Unidad::~Unidad()
{
    unidades.remove(this);
    
    if ( objetoHandler != -1 )
    {
        Graficos::Api::DestruirObjeto(objetoHandler);
    }
}

void
Unidad::SetHandlerUnidad(int handlerUnidad)
{
    Unidad::handlerUnidad = handlerUnidad ;
}

//Cambia la posicion de la unidad
void    
Unidad::SetPos(float x, float y, float z)
{
    Unidad::x = x ;
    Unidad::y = y ;
    Unidad::z = z ;
    Graficos::Api::MoverObjeto(objetoHandler, x, y, z);
}

//Cambia orientacion de la unidad
void
Unidad::SetAngulo(float angulo)
{
    Unidad::angulo = angulo ;
    Graficos::Api::SetAnguloObjeto(objetoHandler, angulo);
}

//Obtiene posicion de la unidad
void
Unidad::GetPos(float &px, float &py, float &pz)
{
   px = Unidad::x; py = Unidad::y; pz = Unidad::z;
}

}
