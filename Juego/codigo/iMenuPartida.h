#ifndef __I_MENUPARTIDA_H__
#define __I_MENUPARTIDA_H__

#include <allegro.h>
#include <alleggl.h>

#include "iMenu.h"

namespace Interfaz
{

class MenuPartida : public Menu
{
private:
    int imagenNombre[16] ;
    int spriteNombre[16] ;
    int rectanguloColor[16] ;
    
    int imagenChatBox ;
    int chatBox ;
    
    int imagenTextBox ;
    int textBox ;
    bool enterTextBox ;
    
    int imagenBotonColor ;
    int imagenBotonColorSeleccion ;
    int botonColor ;
    bool clickBotonColor ;
    
public:
    MenuPartida(Menu *menuPadre);
    ~MenuPartida();
    void Actualizar();
    void Continuar();
    void Activar();
    void Desactivar();
    void ClickBoton4();
    void ClickBoton5();
    void NuevoJugador(int numJugador, string nombreJugador);
    void AbandonoJugador(int numJugador);
    void MensajeChat(int numJugador, string nombreJugador);
    void EmpezarPartida();
};

}

#endif
