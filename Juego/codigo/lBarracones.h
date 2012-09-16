#ifndef __BARRACONES__
#define __BARRACONES__

#include <allegro.h>
#include <alleggl.h>

#include "lUnidad.h"
#include "rProtocolo.h"

namespace Logica{

class Barracones : public Unidad
{
private:
    int experiencia;
    int estado ;
    float cantidadConstruido ;
    int cantidadTotal ;
    
public:
    static const int VIDA=100;
    
    static const int EstadoReposo = 0 ;
    static const int EstadoConstruyendoSoldado = 1 ;
    static const int EstadoDesarrollandoArmaduraSoldado = 2 ;
    static const int EstadoDesarrollandoVelocidadDisparoSoldado = 3 ;
    //------------------
    static const int PConsSoldado = 50 ;
    static const int PConsArmaduraSoldado = 200 ;
    static const int PConsVelocidadDisparoSoldado = 200 ;
    
    Barracones(int ID, Util::Vector2D pos, int eq);
    ~Barracones();
    int GetTipo(){ return Red::Protocolo::Unidad::Barracones; }
    void Actualizar();
    bool RecibirMensaje(const Mensaje &msg);

    void ConstruirSoldado();
    void DesarrollarArmaduraSoldado();
    void DesarrollarVelocidadDisparoSoldado();
    void Cancelar();
};

}

#endif
