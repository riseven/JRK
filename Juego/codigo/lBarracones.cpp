#include "lBarracones.h"

#include "logica.h"
#include "lApi.h"
#include "rProtocolo.h"
#include "lenum_mensajes.h"
#include "rApi.h"


using namespace Logica;
using namespace Util;

Barracones::Barracones(int id, Vector2D pos, int eq):
    Unidad(id, pos, VIDA, 0,0, eq)
{
    estado = EstadoReposo ;
}

Barracones::~Barracones()
{
    estado = EstadoReposo ;
}


/*
 * Bucle principal de la unidad
 */
void
Barracones::Actualizar()
{
    if ( estado != EstadoReposo )
    {
        // Aquí habrá que comprobar motivaciones, pero no en el cuartel general
        cantidadConstruido+=1.0 ;
        
        if ( cantidadConstruido >= cantidadTotal )
        {
            switch ( estado )
            {
            case EstadoConstruyendoSoldado:
                Api::CrearUnidad( Red::Protocolo::Unidad::Soldado , getEquipo(), posicion.getX(), posicion.getY()+3.0 );                
                break ; 
            case EstadoDesarrollandoArmaduraSoldado:
                Red::Api::GetApiInterfaz()->SetEstadoTecnologia(equipo, Red::Protocolo::Tecnologia::ArmaduraSoldado, Red::Protocolo::EstadoTecnologia::Desarrollado);
                break ;
            case EstadoDesarrollandoVelocidadDisparoSoldado:
                Red::Api::GetApiInterfaz()->SetEstadoTecnologia(equipo, Red::Protocolo::Tecnologia::VelocidadDisparoSoldado, Red::Protocolo::EstadoTecnologia::Desarrollado);
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
Barracones::RecibirMensaje(const Mensaje &msg)
{
    switch ( msg.msg )
    {
    case MSG_ConstruirSoldado:
        ConstruirSoldado();
        return true ;
    case MSG_DesarrollarArmaduraSoldado:
        DesarrollarArmaduraSoldado();
        return true ;
    case MSG_DesarrollarVelocidadDisparoSoldado:
        DesarrollarVelocidadDisparoSoldado();
        return true ;
    }
    
    return Unidad::RecibirMensaje(msg);
}
    

void
Barracones::ConstruirSoldado()
{
    if ( estado == EstadoReposo )
    {
        estado = EstadoConstruyendoSoldado ;
        cantidadConstruido = 0 ;
        cantidadTotal = PConsSoldado ;
    }
}

void
Barracones::DesarrollarArmaduraSoldado()
{
    if ( estado == EstadoReposo )
    {
        estado = EstadoDesarrollandoArmaduraSoldado ;
        cantidadConstruido = 0 ;
        cantidadTotal = PConsArmaduraSoldado ;
        Red::Api::GetApiInterfaz()->SetEstadoTecnologia(equipo, Red::Protocolo::Tecnologia::ArmaduraSoldado, Red::Protocolo::EstadoTecnologia::EnDesarrollo);
    }
}

void
Barracones::DesarrollarVelocidadDisparoSoldado()
{
    if ( estado == EstadoReposo )
    {
        estado = EstadoDesarrollandoVelocidadDisparoSoldado ;
        cantidadConstruido = 0 ;
        cantidadTotal = PConsVelocidadDisparoSoldado ;
        Red::Api::GetApiInterfaz()->SetEstadoTecnologia(equipo, Red::Protocolo::Tecnologia::VelocidadDisparoSoldado, Red::Protocolo::EstadoTecnologia::EnDesarrollo);        
    }
} 

void
Barracones::Cancelar()
{
    estado = EstadoReposo ;
    cantidadConstruido = 0 ;
    cantidadTotal = 0 ;
    // Falta añadir cambiar de EnDesarrollo a NoDesarrollado
}
       
