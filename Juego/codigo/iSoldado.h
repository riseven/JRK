#ifndef __I_SOLDADO_H__
#define __I_SOLDADO_H__

#include "iUnidadAgresiva.h"
#include <string>

namespace Interfaz
{
    
class Soldado : public UnidadAgresiva
{
private:
    static int textura ;
    static int modeloHandler[8] ;
    static bool correrPulsado ;
    bool corriendo ;
    
    static int sonidosMover[3] ;
    static int sonidosAtacar[5] ;
public:
    Soldado(int numJugador) ;
    virtual ~Soldado() ; 
    void Seleccionar();
    void Actualizar(); 
    int GetTipo(); 
    
    string GetPathRecursos() { return "Unidades\\SoldadoRaso\\"; }
    
    virtual void PintarBotonesUnidad();
    
    static void Inicializar();
    static void Reset();
};    
    

}

#endif
