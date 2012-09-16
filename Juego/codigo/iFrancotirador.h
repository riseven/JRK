#ifndef __I_FRANCOTIRADOR_H__
#define __I_FRANCOTIRADOR_H__

#include <allegro.h>
#include <alleggl.h>

#include <string>

#include "iUnidadAgresiva.h"

using namespace std ;

namespace Interfaz
{
    
class Francotirador : public UnidadAgresiva
{
private:
    static int modeloHandler ;
public:
    Francotirador(int numJugador) ;
    virtual ~Francotirador() ; 
    void Seleccionar();
    void Actualizar();
    int GetTipo(); 
    
    string GetPathRecursos() { return ""; }
    
    static void Inicializar() ;
};    

}

#endif
