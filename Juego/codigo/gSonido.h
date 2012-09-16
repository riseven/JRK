#ifndef __G_SONIDO_H__
#define __G_SONIDO_H__

#include <allegro.h>
#include <alleggl.h>

#include <string>

using namespace std;

namespace Graficos
{

class Sonido
{
private:
    SAMPLE *sample;
    
public:
    Sonido(string nombreFichero);
    ~Sonido();
    void Play();
    void PlayLoop();
};  
    
}

#endif
