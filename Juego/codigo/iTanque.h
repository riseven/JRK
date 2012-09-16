#ifndef __I_TANQUE_H__
#define __I_TANQUE_H__

#include "iUnidadAgresiva.h"
#include <string>

namespace Interfaz
{
    
class Tanque : public UnidadAgresiva
{
private:
    static int modeloHandler ;
public:
    Tanque(int numJugador) ;
    virtual ~Tanque() ; 
    void Seleccionar();
    void Actualizar();
    int GetTipo(); 
    bool EsReparable(){return true;}
    
    string GetPathRecursos() { return ""; }
    
    static void Inicializar() ;
};    

}

#endif
