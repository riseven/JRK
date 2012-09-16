#ifndef __I_ACADEMIA_FRANCOTIRADORES_H__
#define __I_ACADEMIA_FRANCOTIRADORES_H__

#include "iUnidadUniSeleccionable.h"
#include <string>


namespace Interfaz
{
    
class AcademiaDeFrancotiradores : public UnidadUniSeleccionable
{
private:
    static int modeloHandler ;
public:
    AcademiaDeFrancotiradores(int numJugador) ;
    virtual ~AcademiaDeFrancotiradores() ; 
    void Seleccionar();
    void Actualizar();
    int GetTipo();
    bool EsReparable(){return true;}
    
    string GetPathRecursos() { return "Unidades\\AcademiaDeFrancotiradores\\"; }
    
    static void Inicializar() ;
    static void Cerrar();
};    

}

#endif
