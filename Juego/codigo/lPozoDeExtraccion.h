#ifndef __POZODEEXTRACCION__
#define __POZODEEXTRACCION__

#include <allegro.h>
#include <alleggl.h>

#include "lUnidad.h"
#include "rProtocolo.h"

namespace Logica{

class PozoDeExtraccion : public Unidad
{
private:

    
public:
    static const int VIDA=100;
    
    PozoDeExtraccion(int ID, Util::Vector2D pos, int eq);
    ~PozoDeExtraccion();
    int GetTipo(){ return Red::Protocolo::Unidad::PozoDeExtraccion; }
    void Actualizar();
};

}

#endif
