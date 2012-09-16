#ifndef __CUARTEL_GENERAL__
#define __CUARTEL_GENERAL__

#include <allegro.h>
#include <alleggl.h>

#include "lUnidad.h"
#include "rProtocolo.h"

namespace Logica{

class CuartelGeneral : public Unidad
{
private:
    int experiencia;
    int estado ;
    float cantidadConstruido ;
    int cantidadTotal ;
    
public:
    static const int VIDA=100;
    
    static const int EstadoReposo = 0 ;
    static const int EstadoConstruyendoIngeniero = 1 ;
    static const int EstadoDesarrollandoMotivacion = 2 ;
    static const int EstadoDesarrollandoMotivacionMasDistancia = 3 ;
    //------------------
    static const int PConsIngeniero = 50 ;
    static const int PConsMotivacion = 250 ;
    static const int PConsMotivacionMasDistancia = 250 ;
    
    CuartelGeneral(int ID, Util::Vector2D pos, int eq);
    ~CuartelGeneral();
    int GetTipo(){ return Red::Protocolo::Unidad::CuartelGeneral; }
    void Actualizar();
    bool RecibirMensaje(const Mensaje &msg);

    void ConstruirIngeniero();
    void DesarrollarMotivacion();
    void DesarrollarMotivacionMasDistancia();
    void Cancelar();
};

}

#endif
