#ifndef __G_TERRENO_H__
#define __G_TERRENO_H__

#include <string>
#include <vector>

#include "gModelo.h"
#include "gObjeto3D.h"

using namespace std;

namespace Graficos
{
    
class Terreno
{
private:
    
    InterleavedElemenent interleavedElements[50000] ;
    int numElemento ;
    
    float *alturas ;
    int w, h;
    int callList ;
    int numElementos ;
    Imagen *hierba, *agua, *rocas, *nieve ;
    Imagen *nevado[8] ;
    
    bool invierno ;
    float alphaNevado ;
    
    int *listasTerreno ;
    int *listasTerrenoHierba ;
    int *listasTerrenoNieve ;
    Util::Vector3D *normales ;
    int listaAgua ;
    
    bool *clip ;
    
    float alturaAguaInicial ;
    float offsetAgua ;
    
    static const int numPlanosHierba = 15;
    int texAgua[numPlanosHierba] ;
    
    bool multiTextura ;
    
    void GenerarTexturasHierba();
public:
    static const float escala = 2.0 ;
    
    Terreno(string ficheroAlturas, bool multiTextura) ;
    ~Terreno() ;
    void Actualizar();
    void Pintar();
    float GetAltura(int x, int y);
    int GetAncho();
    int GetAlto();
    float GetAlturaSuave(float x, float z);
    void SetInvierno(bool invierno);
};

}

#endif
