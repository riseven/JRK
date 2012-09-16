#include "lPetroleo.h"

#include "logica.h"
#include "lApi.h"
#include "rProtocolo.h"
#include "lenum_mensajes.h"
#include "rApi.h"


namespace Logica 
{

Petroleo::Petroleo(int id, Util::Vector2D pos, int eq):
    Unidad(id, pos, VIDA, 0,0, eq)
{
}

Petroleo::~Petroleo()
{
}

void
Petroleo::Actualizar()
{
}
       
}
