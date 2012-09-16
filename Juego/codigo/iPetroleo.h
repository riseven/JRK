#ifndef __I_PETROLEO_H__
#define __I_PETROLEO_H__

#include "iUnidad.h"
#include "rProtocolo.h"
#include <string>

namespace Interfaz
{
    
class Petroleo : public Unidad
{
private:
    static int imagenPetroleo ;
    static int modeloHandler ;
public:
    Petroleo() ;
    virtual ~Petroleo() ; 
    
    int GetTipo(){return Red::Protocolo::Unidad::Petroleo;}
    
    static void Inicializar();
    static void Reset();
};    

}

#endif
