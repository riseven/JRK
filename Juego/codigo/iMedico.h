#ifndef __I_MEDICO_H__
#define __I_MEDICO_H__

#include "iUnidadMultiSeleccionable.h"
#include <string>

namespace Interfaz
{
    
class Medico : public UnidadMultiSeleccionable
{
private:
    static int modeloHandler ;
public:
    Medico(int numJugador) ;
    virtual ~Medico() ; 
    void Seleccionar();
    void Actualizar();
    int GetTipo(); 
    bool EsAgresiva(){return false;}
    
    string GetPathRecursos() { return ""; }
    
    static void Inicializar() ;
};    

}

#endif
