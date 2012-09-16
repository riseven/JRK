#ifndef __I_HELICOPTERO_H__
#define __I_HELICOPTERO_H__

#include <allegro.h>
#include <alleggl.h>

#include <string>

#include "iUnidadAgresiva.h"

using namespace std ;

namespace Interfaz
{
    
class Helicoptero : public UnidadAgresiva
{
private:
    static int modeloHandler ;
public:
    Helicoptero(int numJugador) ;
    virtual ~Helicoptero() ; 
    void Seleccionar();
    void Actualizar();
    int GetTipo(); 
    
    string GetPathRecursos() { return ""; }
    
    static void Inicializar() ;
};    

}

#endif
