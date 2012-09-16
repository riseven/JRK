#ifndef __I_ZAPADOR_H__
#define __I_ZAPADOR_H__

#include "iUnidadMultiSeleccionable.h"
#include <string>

namespace Interfaz
{
    
class Zapador : public UnidadMultiSeleccionable
{
private:
    static int modeloHandler ;
public:
    Zapador(int numJugador) ;
    virtual ~Zapador() ; 
    void Seleccionar();
    void Actualizar();
    int GetTipo();
    bool EsAgresiva(){return false;}
    
    string GetPathRecursos() { return ""; }
    
    static void Inicializar() ;
};    

}

#endif
