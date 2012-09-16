#include "gObjeto3D.h"
#include "gApi.h"
#include "eExcepcionParametro.h"

namespace Graficos
{

list <Objeto3D *> Objeto3D::objetos ;

Objeto3D::Objeto3D(Modelo *modelo, int glNombre):
    Entidad3D(0,0,0,0,0)
{
    if ( !modelo )
    {
        throw Error::ExcepcionParametro("Graficos::Objeto3D::Objeto3D(Modelo *, int)", 1);
    }    
    objetos.push_back(this);

    Objeto3D::modelo = modelo ;    
    Objeto3D::glNombre = glNombre ;
    seleccionable = false ;
    siempreVisible = false ;
    transparente = false ;
    angulo = 0.0 ;
    escala = 1.0 ;
    
    mascara[0] = true ;
    mascara[1] = true ;
    mascara[2] = true ;
    mascara[3] = true ;
}

Objeto3D::~Objeto3D()
{
    objetos.remove(this);
    
    modelo = NULL ;
    glNombre = -1 ;
}

void
Objeto3D::SetGlNombre(int glNombre)
{
    Objeto3D::glNombre = glNombre ;
}

bool
Objeto3D::EsObjetoSeleccionable(int nombre)
{
    for ( list <Objeto3D *>::iterator i = objetos.begin() ; i != objetos.end() ; ++i )
    {
        if ( (*i)->glNombre == nombre )
        {
            return (*i)->EsSeleccionable() ;
        }
    }  
    throw Error::ExcepcionParametro("Graficos::Objeto3D::EsObjetoSeleccionable(int)", 1);    
}

void  
Objeto3D::SetAngulo(float angulo)
{
    Objeto3D::angulo = angulo ;
}

void
Objeto3D::SetEscala(float escala)
{
    Objeto3D::escala = escala ;
}

void
Objeto3D::SetSeleccionable(bool seleccionable)
{
    this->seleccionable = seleccionable ;
}

bool
Objeto3D::EsSeleccionable()
{
    return seleccionable ;
}

void
Objeto3D::Pintar()
{
    try
    {
        glPushMatrix();
        glLoadName(glNombre);
        Entidad3D::Pintar();
    
        glRotatef(angulo+2*(180-angulo), 0, 1, 0);
        int tiempoPintar = GetTickCount();
        glColorMask(mascara[0], mascara[1], mascara[2], mascara[3]);
        glScalef(escala, escala, escala);
        modelo->Pintar();
        glColorMask(true, true, true, true);
        //cout << " Modelo::Pintar(): " << GetTickCount() - tiempoPintar << endl ;
        glColor4f(1.0,1.0,1.0,1.0);
        glPopMatrix();
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Objeto3D::Pintar()").Combinar(ex);
    }
}

void
Objeto3D::PintarBoundingBox()
{
    try
    {
        glPushMatrix();
        glLoadName(glNombre);
        Entidad3D::Pintar();
    
        glRotatef(angulo+2*(180-angulo), 0, 1, 0);
        glScalef(escala, escala, escala);
        modelo->PintarBoundingBox();
        glColor4f(1.0,1.0,1.0,1.0);
        glPopMatrix();
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Objeto3D::Pintar()").Combinar(ex);
    }
}

void
Objeto3D::SetSiempreVisible(bool siempreVisible)
{
    Objeto3D::siempreVisible = siempreVisible ;
}

bool
Objeto3D::EsSiempreVisible()
{
    return siempreVisible ;
}

void
Objeto3D::SetTransparente(bool transparente)
{
    Objeto3D::transparente = transparente ;
}

bool
Objeto3D::EsTransparente()
{
    return transparente ;
}

bool
Objeto3D::Colision(Objeto3D *o)
{
    Util::Vector3D bmin1, bmax1, bmin2, bmax2 ;
    
    modelo->GetBoundingBox(bmin1, bmax1);
    o->modelo->GetBoundingBox(bmin2, bmax2);
    
    if ( bmin1.GetX() + x > bmax2.GetX() + o->x ) return false ;
    if ( bmax1.GetX() + x < bmin2.GetX() + o->x ) return false ;
    if ( bmin1.GetY() + y > bmax2.GetY() + o->y ) return false ;
    if ( bmax1.GetY() + y < bmin2.GetY() + o->y ) return false ;
    if ( bmin1.GetZ() + z > bmax2.GetZ() + o->z ) return false ;
    if ( bmax1.GetZ() + z < bmin2.GetZ() + o->z ) return false ;        
    return true ;
}

void
Objeto3D::SetMascara(bool r, bool g, bool b, bool a)
{
    mascara[0] = r ;
    mascara[1] = g ;
    mascara[2] = b ;
    mascara[3] = a ;
}

float
Objeto3D::GetAltura()
{
    return modelo->GetAltura() ;
}

float
Objeto3D::GetRadio()
{
    return modelo->GetRadio();
}

}
