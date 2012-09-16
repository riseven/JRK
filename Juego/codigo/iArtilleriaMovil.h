#ifndef __I_ARTILLERIA_MOVIL_H__
#define __I_ARTILLERIA_MOVIL_H__

#include "iUnidadAgresiva.h"
#include <string>

namespace Interfaz
{
    
class ArtilleriaMovil : public UnidadAgresiva
{
private:
    static int modeloHandler ;
public:
    ArtilleriaMovil(int numJugador) ;
    virtual ~ArtilleriaMovil() ; 
    void Seleccionar();
    void Actualizar();
    int GetTipo(); 
    bool EsReparable(){return true;}
    
    string GetPathRecursos() { return ""; }
    
    static void Inicializar() ;
};    

}

#endif
