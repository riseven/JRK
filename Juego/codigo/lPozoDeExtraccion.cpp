#include "lPozoDeExtraccion.h"

#include "logica.h"
#include "lApi.h"
#include "rProtocolo.h"
#include "lenum_mensajes.h"
#include "rApi.h"


namespace Logica 
{

PozoDeExtraccion::PozoDeExtraccion(int id, Util::Vector2D pos, int eq):
    Unidad(id, pos, VIDA, 0,0, eq)
{
}

PozoDeExtraccion::~PozoDeExtraccion()
{
}

void
PozoDeExtraccion::Actualizar()
{
    Api::GetJugador( getEquipo() )->AddDinero(0.1);
}

}
