#include "lCuartelGeneral.h"

#include "logica.h"
#include "lApi.h"
#include "rProtocolo.h"
#include "lenum_mensajes.h"
#include "rApi.h"


using namespace Logica;
using namespace Util;

CuartelGeneral::CuartelGeneral(int id, Vector2D pos, int eq):
    Unidad(id, pos, VIDA, 0,0, eq)
{
    estado = EstadoReposo ;
}

CuartelGeneral::~CuartelGeneral()
{
    estado = EstadoReposo ;
}


/*
 * Bucle principal de la unidad
 */
void
CuartelGeneral::Actualizar()
{
    if ( estado != EstadoReposo )
    {
        // Aquí habrá que comprobar motivaciones, pero no en el cuartel general
        cantidadConstruido+=1.0 ;
        
        if ( cantidadConstruido >= cantidadTotal )
        {
            switch ( estado )
            {
            case EstadoConstruyendoIngeniero:
                Api::CrearUnidad( Red::Protocolo::Unidad::Ingeniero , getEquipo(), posicion.getX(), posicion.getY()+3.0 );                
                break ; 
            case EstadoDesarrollandoMotivacion:
                Red::Api::GetApiInterfaz()->SetEstadoTecnologia(equipo, Red::Protocolo::Tecnologia::Motivacion, Red::Protocolo::EstadoTecnologia::Desarrollado);
                break ;
            case EstadoDesarrollandoMotivacionMasDistancia:
                Red::Api::GetApiInterfaz()->SetEstadoTecnologia(equipo, Red::Protocolo::Tecnologia::MotivacionMasDistancia, Red::Protocolo::EstadoTecnologia::Desarrollado);
                break;
            }
            estado = EstadoReposo ;
            
            Red::Api::GetApiInterfaz()->SetPorcentajeConstruidoUnidad(getID(), -1);
        }
        else
        {
            Red::Api::GetApiInterfaz()->SetPorcentajeConstruidoUnidad(getID(), (int)((cantidadConstruido/cantidadTotal)*100));
        }
    }    
}

bool
CuartelGeneral::RecibirMensaje(const Mensaje &msg)
{
    switch ( msg.msg )
    {
    case MSG_ConstruirIngeniero:
        ConstruirIngeniero();
        return true ;
    case MSG_DesarrollarMotivacion:
        DesarrollarMotivacion();
        return true ;
    case MSG_DesarrollarMotivacionMasDistancia:
        DesarrollarMotivacionMasDistancia();
        return true ;
    }
    
    return Unidad::RecibirMensaje(msg);
}
    

void
CuartelGeneral::ConstruirIngeniero()
{
    if ( estado == EstadoReposo )
    {
        estado = EstadoConstruyendoIngeniero ;
        cantidadConstruido = 0 ;
        cantidadTotal = PConsIngeniero ;
    }
}

void
CuartelGeneral::DesarrollarMotivacion()
{
    if ( estado == EstadoReposo )
    {
        estado = EstadoDesarrollandoMotivacion ;
        cantidadConstruido = 0 ;
        cantidadTotal = PConsMotivacion ;
        Red::Api::GetApiInterfaz()->SetEstadoTecnologia(equipo, Red::Protocolo::Tecnologia::Motivacion, Red::Protocolo::EstadoTecnologia::EnDesarrollo);
    }
}

void
CuartelGeneral::DesarrollarMotivacionMasDistancia()
{
    if ( estado == EstadoReposo )
    {
        estado = EstadoDesarrollandoMotivacionMasDistancia ;
        cantidadConstruido = 0 ;
        cantidadTotal = PConsMotivacionMasDistancia ;
        Red::Api::GetApiInterfaz()->SetEstadoTecnologia(equipo, Red::Protocolo::Tecnologia::MotivacionMasDistancia, Red::Protocolo::EstadoTecnologia::EnDesarrollo);        
    }
} 

void
CuartelGeneral::Cancelar()
{
    estado = EstadoReposo ;
    cantidadConstruido = 0 ;
    cantidadTotal = 0 ;
}
       
