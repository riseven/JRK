#ifndef __PETROLEO__
#define __PETROLEO__

#include <allegro.h>
#include <alleggl.h>

#include "lUnidad.h"
#include "rProtocolo.h"

namespace Logica{

class Petroleo : public Unidad
{
private:

    
public:
    static const int VIDA=100;
    
    Petroleo(int ID, Util::Vector2D pos, int eq);
    ~Petroleo();
    int GetTipo(){ return Red::Protocolo::Unidad::Petroleo; }
    void Actualizar();
};

}

#endif
