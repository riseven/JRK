#include "allegro.h"
#include <alleggl.h>
#include <winsock2.h>
#include <cstdlib>
#include <exception>
#include <string>

#include "iConfiguracion.h"
#include "iMenuPrincipal.h"
#include "gApi.h"
#include "iApi.h"
#include "lApi.h"
#include "eExcepcion.h"


using namespace std;

void InitSockets()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    
    srand((unsigned)time(NULL));
    
    wVersionRequested = MAKEWORD( 2, 2 );
     
    err = WSAStartup( wVersionRequested, &wsaData );
    if ( err != 0 ) {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.                                  */
        exit(-1);
        return;
    }
     
    /* Confirm that the WinSock DLL supports 2.2.*/
    /* Note that if the DLL supports versions greater    */
    /* than 2.2 in addition to 2.2, it will still return */
    /* 2.2 in wVersion since that is the version we      */
    /* requested.                                        */
     
    if ( LOBYTE( wsaData.wVersion ) != 2 ||
            HIBYTE( wsaData.wVersion ) != 2 ) {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.                                  */
        WSACleanup( );
        exit(-1);
        return; 
    }
     
    /* The WinSock DLL is acceptable. Proceed. */    
}

int main(int argc, char *argv[])
{
    try
    {
        allegro_init();
        install_allegro_gl();
        install_keyboard();
        install_timer();
        install_mouse();
        install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);

        // Ponemos el título de la ventana (Visible si se produce algun error, y
        // si se sale al escritorio en la barra de tareas)
        set_window_title("JRV"); // HAY QUE CAMBIARLO POR EL NOMBRE DEL JUEGO

        // Inicializamos los sockets
        InitSockets();

        // Inicializamos los modulos
        Graficos::Api::Inicializar(1024, 768);
        Interfaz::Api::Inicializar();
         
        // Cargamos el directorio de ejecucion en la clase Configuracion
        Interfaz::Configuracion::SetNombreDirectorio(argv[0]);
        
        // Entramos en el bucle del juego
        Interfaz::Api::BuclePrincipal();
        
        // Cerramos los sockets
        WSACleanup();
        
        // Cerramos la Api grafica
        Graficos::Api::Cerrar();   
    }
    catch ( Error::Excepcion ex )
    {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("%s\n", ex.GetMensajeDeError().c_str() );
    }
    catch ( exception &ex )
    {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("%s\n", ex.what() );
    }   
    catch (...)
    {   
    }

    return 0;
}
END_OF_MAIN();
