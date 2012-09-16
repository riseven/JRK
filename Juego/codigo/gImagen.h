#ifndef __G_IMAGEN_H__
#define __G_IMAGEN_H__

#include <allegro.h>
#include <alleggl.h>
#include <string>

#include "gFuente.h"

using namespace std;

namespace Graficos
{

class Imagen
{
private:
    BITMAP *bitmap;
    GLuint textura ;
    bool masked ;
    int w, h ;
    
public:
    Imagen(string NombreFichero);
    Imagen(Fuente *fuente, string texto);
    Imagen(Fuente *fuente, string texto, int r, int g, int b);
    ~Imagen();
    BITMAP * GetBitmap();
    void SetMasked(bool Masked);
    bool GetMasked();
    int GetAncho();
    int GetAlto();
    GLuint GetTextura();
    void Pintar(int x, int y);
    void PintarTransparente(int x, int y, int alpha);
    void PintarPixel(int x, int y, char r, char g, char b);
    void AplicarMascara();
    
    static BITMAP * CargarBitmap(string NombreFichero);
};

}

#endif 
