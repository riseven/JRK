#ifndef __I_JEEP_H__
#define __I_JEEP_H__

#include "iUnidadAgresiva.h"
#include <string>

namespace Interfaz
{
    
class Jeep : public UnidadAgresiva
{
private:
    static int modeloHandler ;
public:
    Jeep(int numJugador) ;
    virtual ~Jeep() ; 
    void Seleccionar();
    void Actualizar();
    int GetTipo(); 
    bool EsReparable(){return true;}
    
    string GetPathRecursos() { return ""; }
    
    static void Inicializar() ;
};    

}

#endif
