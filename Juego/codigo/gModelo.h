#ifndef __G_MODELO_H__
#define __G_MODELO_H__

#include <allegro.h>
#include <alleggl.h>

#include <string>

#include <lib3ds/file.h>                        
#include <lib3ds/camera.h>
#include <lib3ds/mesh.h>
#include <lib3ds/node.h>
#include <lib3ds/material.h>
#include <lib3ds/matrix.h>
#include <lib3ds/vector.h>
#include <lib3ds/light.h>

#include "gBaseModelo.h"
#include "gImagen.h"
#include "uVector3D.h"

using namespace std;

namespace Graficos
{
    
    typedef struct InterleavedElemenent
    {
        float texture[2] ;
        float color[4] ;        
        float normal[3] ;
        float vertex[3] ;
    };
    
class Modelo : public BaseModelo
{
    
private:
    int callList ;
    Lib3dsFile *file ;
    Util::Vector3D boundingBoxMin, boundingBoxMax;
    bool deFichero ;
    Imagen *textura ;
    
    static InterleavedElemenent elementoArray[50000];
    int elementoInicio ;
    int elementoCuenta ;
    
    float colorBando[3] ;
    
    void renderNode(Lib3dsNode *node);
    
    bool transparente ;
public:
    Modelo(string nombreFichero, Imagen *textura=NULL, float r=1.0, float g=1.0, float b=1.0);
    Modelo(int callList);
    ~Modelo();
    void SetBoundingBox(Util::Vector3D bmin, Util::Vector3D bmax);
    void GetBoundingBox(Util::Vector3D &bmin, Util::Vector3D &bmax);
    void Pintar();
    void PintarBoundingBox();
    float GetAltura();
    float GetRadio();
    
    void SetTransparente(bool t){transparente = t;}
    
    static Modelo * CrearQuad(Imagen *imagenQuad, float r, float g, float b, float a);
};    
    
}

#endif
