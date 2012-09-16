#include "gEntidad2D.h"
#include "eExcepcionLocalizada.h"

namespace Graficos
{

list <Entidad2D *> Entidad2D::entidades ;

Entidad2D::Entidad2D(int X, int Y, int Z):
    x(X), y(Y), z(Z)
{
    activo = true ;
    bool colocado = false ;
    for ( list <Entidad2D *>::iterator i = entidades.begin() ; i != entidades.end(); ++i )
    {
        if ( (*this)>=(**i) )
        {
            entidades.insert(i, this);
            colocado = true ;
            break ;
        }
    }
    if ( !colocado )
    {
        entidades.push_back(this);
    }
}

Entidad2D::~Entidad2D()
{
    entidades.remove(this);
}

int
Entidad2D::GetX()
{
    return x ;
}

int
Entidad2D::GetY()
{
    return y ;
}

int
Entidad2D::GetZ()
{
    return z ;
}

void
Entidad2D::SetPosicion(int x, int y, int z)
{
    Entidad2D::x = x ;
    Entidad2D::y = y ;
    Entidad2D::z = z ;
}

bool
Entidad2D::IsActivo()
{
    return activo ;
}

void
Entidad2D::SetActivo(bool Activo)
{
    activo = Activo ;
}

bool
Entidad2D::operator >=(const Entidad2D& entidad)
{
    return (entidad.z >= z) ;
}

void
Entidad2D::Pintar()
{
}

void
Entidad2D::PintarTodos()
{
    try
    {
        for ( list <Entidad2D *>::iterator i = entidades.begin() ; i != entidades.end() ; ++i )
        {
            if ( (*i)->activo )
            {
                (*i)->Pintar();
            }
        }
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Entidad2D::PintarTodos()").Combinar(ex);
    }
}

void
Entidad2D::EliminarTodos()
{
    try
    {
        while ( entidades.size() > 0 )
        {
            delete entidades.front() ;
        }
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Entidad2D::EliminarTodos()").Combinar(ex);
    }
}

}
