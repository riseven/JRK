#ifndef __G_OBJETO3D_H__
#define __G_OBJETO3D_H__

#include "gEntidad3D.h"
#include "gModelo.h"

namespace Graficos
{

class Objeto3D : public Entidad3D    
{
private:
    static list <Objeto3D *> objetos ;
    
    Modelo *modelo ;
    int glNombre ;
    bool seleccionable ; 
    bool siempreVisible ;
    bool transparente ;
    float angulo ;
    float escala ;
    bool bloqueante ; 
    bool mascara[4] ;
public:
    static bool EsObjetoSeleccionable(int nombre);
    
    Objeto3D(Modelo *modelo, int glNombre);
    ~Objeto3D();
    void SetGlNombre(int glNombre);
    void SetSeleccionable(bool seleccionable);
    void SetAngulo(float angulo);
    void SetEscala(float escala);
    bool EsSeleccionable();
    void SetSiempreVisible(bool siempreVisible);
    bool EsSiempreVisible();
    void SetTransparente(bool transparente);
    bool EsTransparente();
    int  GetCallList(){return glNombre;}
    void SetBloqueante(bool bloquea){bloqueante = bloquea;}
    bool GetBloqueante(){return bloqueante;}
    bool Colision(Objeto3D *o);
    void SetMascara(bool r, bool g, bool b, bool a);
    float GetAltura();
    float GetRadio();
    void Pintar();
    void PintarBoundingBox();
};

}

#endif
