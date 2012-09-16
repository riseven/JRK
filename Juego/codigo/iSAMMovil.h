#ifndef __I_SAM_MOVIL_H__
#define __I_SAM_MOVIL_H__

#include <allegro.h>
#include <alleggl.h>

#include <string>

#include "iUnidadAgresiva.h"

using namespace std;


namespace Interfaz
{
    
class SAMMovil : public UnidadAgresiva
{
private:
    static int modeloHandler ;
public:
    SAMMovil(int numJugador) ;
    virtual ~SAMMovil() ; 
    void Seleccionar();
    void Actualizar();
    int GetTipo(); 
    bool EsReparable(){return true;}
    
    string GetPathRecursos() { return ""; }
    
    static void Inicializar() ;
};    

}

#endif
