#ifndef __I_ACADEMIA_PARACAIDISMO_H__
#define __I_ACADEMIA_PARACAIDISMO_H__

#include "iUnidadUniSeleccionable.h"
#include <string>

namespace Interfaz
{
    
class AcademiaDeParacaidismo : public UnidadUniSeleccionable
{
private:
    static int modeloHandler ;
public:
    AcademiaDeParacaidismo(int numJugador) ;
    virtual ~AcademiaDeParacaidismo() ; 
    void Seleccionar();
    void Actualizar();
    int GetTipo();
    bool EsReparable(){return true;} 
    
    string GetPathRecursos() { return "Unidades\\AcademiaDeParacaidismo\\"; }
    
    static void Inicializar() ;
    static void Cerrar();    
};    

}

#endif
