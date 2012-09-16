#ifndef __I_TRANSPORTE_BLINDADO_H__
#define __I_TRANSPORTE_BLINDADO_H__

#include <allegro.h>
#include <alleggl.h>

#include <string>

#include "iUnidadMultiSeleccionable.h"

using namespace std;

namespace Interfaz
{
    
class TransporteBlindado : public UnidadMultiSeleccionable
{
private:
    static int modeloHandler ;
public:
    TransporteBlindado(int numJugador) ;
    virtual ~TransporteBlindado() ; 
    void Seleccionar();
    void Actualizar();
    int GetTipo();
    bool EsReparable(){return true;}
    bool EsAgresiva(){return false;}
    
    string GetPathRecursos() { return ""; }
    
    static void Inicializar() ;
};    

}

#endif
