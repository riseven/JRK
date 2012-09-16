#include "gEntidad3D.h"
#include "eExcepcionLocalizada.h"
#include "gApi.h"

namespace Graficos
{

list <Entidad3D *> Entidad3D::entidades ;

Entidad3D::Entidad3D(float X, float Y, float Z, float angY, float angXZ):
    x(X), y(Y), z(Z), angY(angY), angXZ(angXZ)
{
    activo = true ;
    entidades.push_back(this);
    radio = 1.0 ;
}

Entidad3D::~Entidad3D()
{
    entidades.remove(this);
}

float
Entidad3D::GetX()
{
    return x ;
}

float
Entidad3D::GetY()
{
    return y ;
}

float
Entidad3D::GetZ()
{
    return z ;
}

float
Entidad3D::GetAnguloY()
{
    return angY ;
}

float
Entidad3D::GetAnguloXZ()
{
    return angXZ ;
}

void
Entidad3D::SetPosicion(float x, float y, float z)
{
    Entidad3D::x = x ;
    Entidad3D::y = y ;
    Entidad3D::z = z ;
}

bool
Entidad3D::IsActivo()
{
    return activo ;
}

void
Entidad3D::SetActivo(bool Activo)
{
    activo = Activo ;
}

void
Entidad3D::Pintar()
{
    glTranslatef(x, y, z);
}

void
Entidad3D::PintarTodos()
{
    try
    {
        for ( list <Entidad3D *>::iterator i = entidades.begin() ; i != entidades.end() ; ++i )
        {
            if ( (*i)->activo )
            {
                glPushMatrix();
                (*i)->Pintar();
                glPopMatrix();
            }
        }
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Entidad3D::PintarTodos()").Combinar(ex);
    }
}

void
Entidad3D::PintarTodosEn(float x0, float y0, float x1, float y1)
{
    try
    {
        for ( list <Entidad3D *>::iterator i = entidades.begin() ; i != entidades.end() ; ++i )
        {
            if ( (*i)->activo && !(*i)->EsSiempreVisible() && !(*i)->EsTransparente())
            {
                float radio = (*i)->GetRadio() ;
                if ( (*i)->GetX() > (x0-radio) && (*i)->GetX() < (x1+radio) &&
                     (*i)->GetZ() > (y0-radio) && (*i)->GetZ() < (y1+radio))
                {
                    glPushMatrix();
                    (*i)->Pintar();
                    glPopMatrix();
                }
            }
        }
    
        for ( list <Entidad3D *>::iterator i = entidades.begin() ; i != entidades.end() ; ++i )
        {
            if ( (*i)->activo && !(*i)->EsSiempreVisible() && (*i)->EsTransparente())
            {
                float radio = (*i)->GetRadio() ;
                if ( (*i)->GetX() > (x0-radio) && (*i)->GetX() < (x1+radio) &&
                     (*i)->GetZ() > (y0-radio) && (*i)->GetZ() < (y1+radio))
                {
                    glPushMatrix();
                    (*i)->Pintar();
                    glPopMatrix();
                }
            }
        }
        
        for ( list <Entidad3D *>::iterator i = entidades.begin() ; i != entidades.end() ; ++i )
        {
            if ( (*i)->activo && (*i)->EsSiempreVisible() )
            {
                float radio = (*i)->GetRadio() ;
                if ( (*i)->GetX() > (x0-radio) && (*i)->GetX() < (x1+radio) &&
                     (*i)->GetZ() > (y0-radio) && (*i)->GetZ() < (y1+radio))
                {
                    glPushMatrix();
                    (*i)->Pintar();
                    glPopMatrix();
                }
            }
        }
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Entidad3D::PintarTodosEn(float, float, float, float)").Combinar(ex);
    }        
}    

void
Entidad3D::EliminarTodos()
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
        throw Error::ExcepcionLocalizada("Graficos::Entidad3D::EliminarTodos()");
    }
}

void
Entidad3D::PintarTodosBoundingBox()
{
    try
    {
        for ( list <Entidad3D *>::iterator i = entidades.begin() ; i != entidades.end() ; ++i )
        {
            if ( (*i)->activo )
            {
                glPushMatrix();
                (*i)->PintarBoundingBox();
                glPopMatrix();
            }
        }
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Entidad3D::PintarTodos()").Combinar(ex);
    }
}

void
Entidad3D::ResetClippingRegs()
{
    try
    {
        for ( list <Entidad3D *>::iterator i = entidades.begin() ; i != entidades.end() ; ++i )
        {
            if ( (*i)->activo )
            {
                int temp = (*i)->GetCallList() ;
                if ( temp > -1 )
                {
                    Api::ResetClippingReg(temp);
                }
            }
        }
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Entidad3D::PintarTodos()").Combinar(ex);
    }
}

void
Entidad3D::PintarTodosClipping()
{
    try
    {
        for ( list <Entidad3D *>::iterator i = entidades.begin() ; i != entidades.end() ; ++i )
        {
            if ( (*i)->activo && !(*i)->EsSiempreVisible() && !(*i)->EsTransparente() )
            {
                int temp = (*i)->GetCallList() ;
                if ( temp > -1 )
                {
                    if ( Api::ComprobarClippingReg(temp) )
                    {
                        int tiempoPintar = GetTickCount() ;
                        (*i)->Pintar();
                        //cout << " Entidad3D::Pintar(): " << GetTickCount() - tiempoPintar << endl ;
                    }
                }
            }
        }
        for ( list <Entidad3D *>::iterator i = entidades.begin() ; i != entidades.end() ; ++i )
        {
            if ( (*i)->activo && !(*i)->EsSiempreVisible() && (*i)->EsTransparente() )
            {
                int temp = (*i)->GetCallList() ;
                if ( temp > -1 )
                {
                    if ( Api::ComprobarClippingReg(temp) )
                    {
                        (*i)->Pintar();
                    }
                }
            }
        }
        for ( list <Entidad3D *>::iterator i = entidades.begin() ; i != entidades.end() ; ++i )
        {
            if ( (*i)->activo && (*i)->EsSiempreVisible() )
            {
                int temp = (*i)->GetCallList() ;
                if ( temp > -1 )
                {
                    if ( Api::ComprobarClippingReg(temp) )
                    {
                        (*i)->Pintar();
                    }
                }
            }
        }                
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Entidad3D::PintarTodos()").Combinar(ex);
    }
}

void
Entidad3D::SetRadio(float radio)
{
    Entidad3D::radio = radio ;
}

float
Entidad3D::GetRadio()
{
    return radio ;
}

}
