#ifndef __I_JUEGO_H__
#define __I_JUEGO_H__

#include <allegro.h>
#include <alleggl.h>

#include "iMenu.h"
#include "iCamara.h"
#include "iInterfazPanel.h"
#include "iUnidadSeleccionable.h"

namespace Interfaz
{

class Juego : public Menu
{
private:
    Camara *camara ;
    InterfazPanel *panel ;
    
    bool escPulsado ;
    
    bool primeraPersona ;
    UnidadSeleccionable *unidadPoseida ;
    
    int rectanguloSeleccion ;
    
    int imagenTextBox ;
    int textBox ;
    
    int chatBox ;
    
    
    bool muestraRectangulo ;
    bool birPulsado ;
    bool bdrPulsado ;
    bool birEstrictoPulsado ;
    bool numKeyPulsado ;
    int orx, ory ;
    bool enterPulsado ;
    bool muestraTextBox ;
    bool hotkey_h ;
    bool hotkey_punto ;
    bool espacioPulsado ;
    bool espacioPulsadoVerdad ;
    bool salirPartida ;

public:
    Juego(Menu *menuPadre);
    ~Juego();
    void Actualizar();
    Camara *GetCamara();
    void PoseerUnidad( UnidadSeleccionable *uni );
    void MensajeChat(int numJugador, string texto);
    bool Escribiendo(){return muestraTextBox;}
    void SalirPartida();
};

}

#endif
