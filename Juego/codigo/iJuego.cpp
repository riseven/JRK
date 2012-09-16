#include <Allegro.h>
#include "iJuego.h"
#include "iApi.h"
#include "gApi.h"
#include "rApi.h"
#include "iCamara.h"

#include "iUnidadSeleccionable.h"
#include "lApi.h"
#include "eExcepcionParametro.h"
#include "eExcepcionLocalizada.h"
#include "rProtocolo.h"
#include "iConfiguracion.h"


namespace Interfaz
{
    
//La clase juego es un menu mas, de esta forma se puede volver al menu padre
//(desde donde se entro al juego) de forma transparente
Juego::Juego(Menu *menuPadre):
    Menu(menuPadre, "", "", "", "", "", "")
{
    try
    {
        Graficos::Api::DesactivarSprite(spriteFondoMenu);
        
        //Guardo referencia al juego
        Api::SetJuego(this);
        
        //Creo la camara
        camara = new Camara();
        
       
        // Creamos el terreno
        Graficos::Api::CrearTerreno( Api::GetInfoMapa()->GetFicheroAlturas(), Configuracion::GetMultiTextura() );
        
        // Crea panel y establece el minimapa del mapa elegido
        panel = new InterfazPanel();
        panel->SetMinimapa( Api::GetInfoMapa()->GetFicheroPreview() );
    
        escPulsado = false ;
        
        int inc = 0;
        float coor=600;
        
        //Creo una unidad del tipo soldado raso
        //Logica::Api::CrearUnidad( Red::Protocolo::Unidad::Barracones, 20, 100);
        //Logica::Api::CrearUnidad( Red::Protocolo::Unidad::CuartelGeneral, 20, 10);
        //Logica::Api::CrearUnidad( Red::Protocolo::Unidad::Soldado, 10, 10);
        //Logica::Api::CrearUnidad( Api::TipoSoldadoRaso, 10, 10);
        
        primeraPersona = false ;
        muestraRectangulo = false ;
        birPulsado = false ;
        bdrPulsado = false ;
        birEstrictoPulsado = false ;
        numKeyPulsado = false ;
        enterPulsado = false ;
        muestraTextBox = false ;
        hotkey_h = false ;
        hotkey_punto = false ;
        espacioPulsado = false ;
        espacioPulsadoVerdad = false ;
        salirPartida = false ;
        
        rectanguloSeleccion = Graficos::Api::CrearRectangulo(50,50,0.5,0.5,1,0.8,false,mouse_x,mouse_y,10);
        Graficos::Api::DesactivarRectangulo(rectanguloSeleccion);        
        
        imagenTextBox = Graficos::Api::CrearImagen("Graficos\\TextBoxChatInGame.bmp");
        textBox = Graficos::Api::CrearTextBox(imagenTextBox, 0, "", 240,555,15);
        Graficos::Api::DesactivarTextBox(textBox);
        
        chatBox = Graficos::Api::CrearChatBox(-1, 0, 600, 300, 10, 10, 250, 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::Juego::Juego(Menu *)").Combinar(ex);
    }
}

//Fin de la partida
Juego::~Juego()
{
    try
    {
        Api::SetJuego(NULL);
        
        delete camara ;
        camara = NULL ;
        
        Graficos::Api::DestruirTerreno();
        Unidad::Reset();
        
        Graficos::Api::DestruirImagen(imagenTextBox);
        Graficos::Api::DestruirTextBox(textBox);

        Graficos::Api::DestruirChatBox(chatBox);
        
        delete panel;
        panel = NULL ;
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::Juego::~Juego()").Combinar(ex);
    }
}


//Actualiza el estado del juego
void
Juego::Actualizar()
{
    try
    {
        if ( salirPartida )
        {
            menuPadre->Continuar() ;
            return ;
        }
        
        // Reseto unidades apuntadas
        UnidadSeleccionable::DesapuntarUnidades();

         
        if ( !primeraPersona )
        {
            if ( !muestraTextBox )
            {
                // Compruebo las hot keys generales            
                if ( key[KEY_H] && !hotkey_h )
                {
                    UnidadSeleccionable::BuscarCuartelGeneral();            
                }
                hotkey_h = key[KEY_H] ;
                
                if ( key[KEY_STOP] && !hotkey_punto )
                {
                    UnidadSeleccionable::BuscarIngeniero();
                }
                hotkey_punto = key[KEY_STOP] ;
            }
            else
            {
                // Asigno el focus al textBox   
                
            }
            
            if ( !birPulsado )
            {
                // Movimiento de camara
                int bordes = 0 ;
                int inc = 0 ;
                
                // Movimiento mediante el teclado
                if ( key[KEY_LEFT] )  bordes |= Camara::BordeIzquierdo ; 
                if ( key[KEY_RIGHT] ) bordes |= Camara::BordeDerecho ;
                if ( key[KEY_UP] )    bordes |= Camara::BordeSuperior ;
                if ( key[KEY_DOWN] )  bordes |= Camara::BordeInferior ;
                
                // Movimiento mediante el raton
                if (mouse_x > SCREEN_W - 5) bordes |= Camara::BordeDerecho ;
                if (mouse_x < 5)            bordes |= Camara::BordeIzquierdo ;
                if (mouse_y > SCREEN_H - 5) bordes |= Camara::BordeInferior ;
                if (mouse_y < 5)            bordes |= Camara::BordeSuperior ;              
                
                // Inclinacion mediante teclado
                if ( key[KEY_PGUP] ) inc = 1;
                if ( key[KEY_PGDN] ) inc = -1;
                
             
                
                // Notificamos a la camara los cambios
                camara->DesplazamientoBordes(bordes);
                camara->InclinarCamara(mouse_z + inc);
                position_mouse_z(0);
                                 
                // Controlamos los grupos de seleccion
                for ( int i = KEY_0 ; i <= KEY_9 ; i++ )
                {
                    if ( key[i] )
                    {
                        if ( key_shifts & KB_CTRL_FLAG )
                        {
                            // Primero comprobamos si estamos añadiendo un grupo
                            UnidadMultiSeleccionable::AsignarGrupo(i-KEY_0);
                        }
                        else
                        {
                            // Si no estamos añadiendo un grupo, borramos la seleccion
                            if ( !(key_shifts & KB_SHIFT_FLAG) )
                            {
                                UnidadMultiSeleccionable::DeseleccionarTodo();
                            }
                            UnidadMultiSeleccionable::AnyadirGrupo(i-KEY_0);
                        }
                    }        
                }
                
          
                
                
                 
                // Apunto unidades bajo el raton
                int bufferUnidades[512] ;
                int numUnis = Graficos::Api::GetObjetosEn(mouse_x, mouse_y, bufferUnidades, 512);
                if ( numUnis == 1 )
                {
                    UnidadSeleccionable::ApuntarUnidad( UnidadSeleccionable::BuscarUnidad(bufferUnidades[0]) );
                }
                else
                {
                    for ( int uniActual = 0 ; uniActual < numUnis ; uniActual++ )
                    {
                        UnidadSeleccionable *uni = UnidadSeleccionable::BuscarUnidad(bufferUnidades[uniActual]);
                        if ( uni->EsMultiSeleccionable() )
                        {
                            UnidadSeleccionable::ApuntarUnidad(uni);
                        }
                    }        
                }
                
                            
                if ( mouse_b & 1 && mouse_y < 570 && !birEstrictoPulsado)
                {
                    birEstrictoPulsado = true ;
                    if ( UnidadSeleccionable::GetAccionActual() == UnidadSeleccionable::AccionNinguna )
                    {
                        // Inicio el click  
                        orx = mouse_x ;
                        ory = mouse_y ;
                        birPulsado = true ;
                    }
                    else
                    {
                        // Primero comprobamos si hay alguna unidad bajo el raton
                        if ( numUnis > 0 )
                        {
                            if ( numUnis == 1 )
                            {
                                UnidadSeleccionable::RespuestaEstrictaUnidad( UnidadSeleccionable::BuscarUnidad(bufferUnidades[0]) );
                            }
                            else
                            {
                                for ( int uniActual = 0 ; uniActual < numUnis ; uniActual++ )
                                {
                                    UnidadSeleccionable *uni = UnidadSeleccionable::BuscarUnidad(bufferUnidades[uniActual]);
                                    if ( uni->EsMultiSeleccionable() )
                                    {
                                        UnidadSeleccionable::RespuestaEstrictaUnidad(uni);
                                        break ;
                                    }
                                } 
                            }
                        }
                        else
                        {
                            float x, z ;
                            Util::Vector3D pos = Graficos::Api::GetCoordenadas2Dto3D(Graficos::Api::CoordenadasVentana::PosicionRaton);
                            UnidadSeleccionable::RespuestaEstrictaTerreno(pos.GetX(), pos.GetZ());
                        }
                    }
                }
                   
                
                
                if ( !(mouse_b & 1) )
                {
                    birEstrictoPulsado = false ;
                }
                
                if ( mouse_b & 2 && mouse_y < 570 && !bdrPulsado)
                {
                    bdrPulsado = true ;
                    
                    // Primero comprobamos si hay alguna unidad bajo el raton
                    if ( numUnis > 0 )
                    {
                        if ( numUnis == 1 )
                        {
                            UnidadSeleccionable::RespuestaInteligenteUnidad( UnidadSeleccionable::BuscarUnidad(bufferUnidades[0]) );
                        }
                        else
                        {
                            for ( int uniActual = 0 ; uniActual < numUnis ; uniActual++ )
                            {
                                UnidadSeleccionable *uni = UnidadSeleccionable::BuscarUnidad(bufferUnidades[uniActual]);
                                if ( uni->EsMultiSeleccionable() )
                                {
                                    UnidadSeleccionable::RespuestaInteligenteUnidad(uni);
                                    break ;
                                }
                            } 
                        }
                    }
                    else
                    {
                        float x, z ;
                        Util::Vector3D pos = Graficos::Api::GetCoordenadas2Dto3D(Graficos::Api::CoordenadasVentana::PosicionRaton);
                        UnidadSeleccionable::RespuestaInteligenteTerreno(pos.GetX(), pos.GetZ());
                    }
                }
                
                   
                if ( !(mouse_b &2) )
                {
                    bdrPulsado = false ;
                }
            }
            else
            {
                int my = mouse_y<570?mouse_y:570 ;
                if ( mouse_b & 1 )
                {
                    // Mostramos el rectangulo
                    Graficos::Api::ActivarRectangulo(rectanguloSeleccion);
                    Graficos::Api::MoverRectangulo(rectanguloSeleccion, orx, ory);
                    Graficos::Api::CambiarTamanyoRectangulo(rectanguloSeleccion, mouse_x-orx, my-ory);
                    
                    // Apunto unidades dentro del rectangulo
                    int bufferUnidades[512] ;
                    int numUnis = Graficos::Api::GetObjetosEnRectangulo(orx, ory, mouse_x-orx, my-ory, bufferUnidades, 512);
                    if ( numUnis == 1 )
                    {
                        UnidadSeleccionable::ApuntarUnidad( UnidadSeleccionable::BuscarUnidad(bufferUnidades[0]) );
                    }
                    else
                    {
                        for ( int uniActual = 0 ; uniActual < numUnis ; uniActual++ )
                        {
                            UnidadSeleccionable *uni = UnidadSeleccionable::BuscarUnidad(bufferUnidades[uniActual]);
                            if ( uni->EsMultiSeleccionable() )
                            {
                                UnidadSeleccionable::ApuntarUnidad(uni);
                            }
                        }        
                    }                      
                }
                else
                {
                    Graficos::Api::DesactivarRectangulo(rectanguloSeleccion);
                    birPulsado = false ;
                    
                    // Apunto unidades bajo el raton
                    int bufferUnidades[512] ;
                    int numUnis = Graficos::Api::GetObjetosEn(mouse_x, mouse_y, bufferUnidades, 512);
                    if ( numUnis == 1 )
                    {
                        UnidadSeleccionable::ApuntarUnidad( UnidadSeleccionable::BuscarUnidad(bufferUnidades[0]) );
                    }
                    else
                    {
                        for ( int uniActual = 0 ; uniActual < numUnis ; uniActual++ )
                        {
                            UnidadSeleccionable *uni = UnidadSeleccionable::BuscarUnidad(bufferUnidades[uniActual]);
                            if ( uni->EsMultiSeleccionable() )
                            {
                                UnidadSeleccionable::ApuntarUnidad(uni);
                            }
                        }        
                    }
                    
                    // Realizo la seleccion de las unidades dentro del rectangulo
                    numUnis = Graficos::Api::GetObjetosEnRectangulo(orx, ory, mouse_x-orx, my-ory, bufferUnidades, 512);
    
                    if ( !(key_shifts & KB_SHIFT_FLAG))
                    {
                        UnidadMultiSeleccionable::DeseleccionarTodo();
                        UnidadUniSeleccionable::DeseleccionarTodo();
                    }
                    
                    if ( numUnis == 1 )
                    {
                        UnidadSeleccionable *uni = UnidadSeleccionable::BuscarUnidad(bufferUnidades[0]);
                        if ( uni->GetNumJugador() == Jugador::GetJugadorPropio() )
                        {
                            if ( !uni->EsMultiSeleccionable() )
                            {
                                UnidadUniSeleccionable::SeleccionarUnidad((UnidadUniSeleccionable *)uni);
                            }
                            else
                            {
                                UnidadMultiSeleccionable::AnyadirUnidad((UnidadMultiSeleccionable *)uni);
                            }
                        }
                    }
                    else if ( numUnis > 1 )
                    {
                        for ( int uniActual = 0 ; uniActual < numUnis ; uniActual++ )
                        {
                            UnidadSeleccionable *uni = UnidadSeleccionable::BuscarUnidad(bufferUnidades[uniActual]);
                            if ( uni->GetNumJugador() == Jugador::GetJugadorPropio() )
                            {
                                if ( uni->EsMultiSeleccionable() )
                                {
                                    UnidadMultiSeleccionable::AnyadirUnidad((UnidadMultiSeleccionable *)uni);
                                }
                            }
                        }
                    }
                }
            }
               
            //Actualizo el panel
            panel->Actualizar();
            // Actualiza las unidades seleccionadas
            Unidad::ActualizarUnidades();
            //Actualizo camara
            camara->Actualizar();        
    
    
        }
        else
        {
            unidadPoseida->ActualizarPrimeraPersona();
            
            // Abandonar la primera persona
            if ( key[KEY_SPACE] && !espacioPulsado )
            {
                espacioPulsadoVerdad = true ;
            }
            if ( !key[KEY_SPACE] && espacioPulsadoVerdad )
            {
                espacioPulsadoVerdad = false ;
                panel->Activar();
                unidadPoseida->SalirPrimeraPersona();
                UnidadMultiSeleccionable::SeleccionarUnidad((UnidadMultiSeleccionable *)unidadPoseida);
                unidadPoseida = NULL ;
                primeraPersona = false ;
            }            
            espacioPulsado = key[KEY_SPACE] ;
        }
        
        if ( key[KEY_ENTER] )
        {
            enterPulsado = true ;
        }
        else if ( enterPulsado )
        {
            enterPulsado = false ;
            if ( muestraTextBox )
            {
                muestraTextBox = false ;
                string texto = Graficos::Api::GetTextoTextBox(textBox);
                if ( texto != "" )
                {
                    Red::Api::GetApiLogica()->MensajeChatJuego(0, texto);
                }
                Graficos::Api::DesactivarTextBox(textBox);
                Graficos::Api::SetTextoTextBox(textBox, "");
            }
            else
            {
                muestraTextBox = true ;
                Graficos::Api::ActivarTextBox(textBox);
            }        
        }
        
        // Actualizar el terreno
        Graficos::Api::ActualizarTerreno();
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::Juego::Actualizar()").Combinar(ex);
    }    
}

//Permite el acceso a la camara desde fuera de la clase
Camara *
Juego::GetCamara()
{
    return camara ;
}   

void
Juego::PoseerUnidad( UnidadSeleccionable *uni )
{
    UnidadUniSeleccionable::DeseleccionarTodo();
    UnidadMultiSeleccionable::DeseleccionarTodo();
    panel->Desactivar();
    uni->EntrarPrimeraPersona();
    unidadPoseida = uni ;
    primeraPersona = true ;
    espacioPulsado = true ; // Para que no se abandone la 1ª persna inmediatamente
}

void
Juego::MensajeChat(int numJugador, string texto)
{
    int color = Api::GetJugador(numJugador)->GetColor();
    Graficos::Api::AddLineaChatBox(chatBox, texto,
                                   Jugador::GetColorRojo(color)*255.0,
                                   Jugador::GetColorVerde(color)*255.0,
                                   Jugador::GetColorAzul(color)*255.0);
}

void
Juego::SalirPartida()
{
    salirPartida = true ;
}

}
