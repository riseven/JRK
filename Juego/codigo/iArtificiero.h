#ifndef __I_ARTIFICIERO_H__
#define __I_ARTIFICIERO_H__

#include "iUnidadAgresiva.h"
#include <string>

namespace Interfaz
{
    
class Artificiero : public UnidadAgresiva
{
private:
    static int modeloHandler ;
public:
    Artificiero(int numJugador) ;
    virtual ~Artificiero() ; 
    void Seleccionar();
    void Actualizar();
    int GetTipo(); 
    
    string GetPathRecursos() { return ""; }
    
    static void Inicializar() ;
};    

}

#endif
