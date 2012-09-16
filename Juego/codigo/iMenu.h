#ifndef __I_MENU_H__
#define __I_MENU_H__

#include <allegro.h>
#include <alleggl.h>

#include <string>

using namespace std;

namespace Interfaz
{
// --------------------------------
// Clase base para construir menus
// --------------------------------
class Menu
{
private:
    static int handlerImagenBoton[5];
    static int handlerImagenSeleccionBoton[5];
    // Posiciones de los botones
    static int posBoton[5][2] ;
    static int sonidoClick ; 
    static int musica ;   

    static int imagenFondoMenu ;


protected:
    // Menu padre
    Menu *menuPadre ;
    
    // Boton pulsado
    int botonPulsado ;
    Menu *menuHijo ;
    
    // Titulo del menu
    string textoTitulo ;
    int imagenTitulo ;
    int spriteTitulo ;
    
    // Botones
    string textoBoton[5];
    int handlerBoton[5];
    bool clickBoton[5];
    
    int spriteFondoMenu ;

public:
    Menu(Menu *menuPadre, string Titulo, string texto1, string texto2, string texto3, string texto4, string texto5);
    virtual ~Menu();
    virtual void Actualizar();
    virtual void Continuar();
    virtual void Activar();
    virtual void Desactivar();
    virtual void ClickBoton1();
    virtual void ClickBoton2();
    virtual void ClickBoton3();
    virtual void ClickBoton4();
    virtual void ClickBoton5();

    static void Inicializar();
};   
    
}

#endif
