#ifndef __I_PARACAIDISTA_H__
#define __I_PARACAIDISTA_H__

#include <allegro.h>
#include <alleggl.h>

#include <string>

#include "iUnidadAgresiva.h"

using namespace std ;

namespace Interfaz
{
    
class Paracaidista : public UnidadAgresiva
{
private:
    static int modeloHandler ;
public:
    Paracaidista(int numJugador) ;
    virtual ~Paracaidista() ; 
    void Seleccionar();
    void Actualizar();
    int GetTipo(); 
    
    string GetPathRecursos() { return ""; }
    
    static void Inicializar() ;
};    

}

#endif
