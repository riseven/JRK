#include "rApiInterfazRed.h"
#include "iApi.h"
#include "rApi.h"
#include "rProtocolo.h"
#include "eExcepcionLocalizada.h"
#include "lApi.h"

namespace Red
{

ApiInterfazRed::ApiInterfazRed():
    ApiInterfaz()
{
    // Creamos el socket de recepcion de conexiones
    socketEntrada = socket( AF_INET , SOCK_STREAM , 0 ) ;
    if ( socketEntrada == INVALID_SOCKET )
    {
        throw Error::ExcepcionLocalizada("Red::ApiInterfazRed::ApiInterfazRed()", "Error al crear el socket");
    }

    int valor = 1 ;
    setsockopt(socketEntrada, SOL_SOCKET, SO_REUSEADDR, (char *)&valor, sizeof(int));
    
    // Vinculamos el socket a una direccion y puerto
    sockaddr_in dirLocal ;
    memset(&dirLocal, 0, sizeof(sockaddr_in));
    dirLocal.sin_family = AF_INET ;
    dirLocal.sin_port = htons(1984) ;
    dirLocal.sin_addr.s_addr = INADDR_ANY ;
    if ( bind(socketEntrada, (sockaddr *)&dirLocal, sizeof(sockaddr_in)) )
    {
        string error = "No se pudo asignar la direccion al socket: " ;
        char texto[30] ;
        itoa( WSAGetLastError(), texto, 10);
        error += texto ;
        throw Error::ExcepcionLocalizada("Red::ApiInterfazRed::ApiInterfazRed()", texto);
    }
    
    
    // Ponemos el socket a escuchar
    if ( listen(socketEntrada, SOMAXCONN) )
    {
        throw Error::ExcepcionLocalizada("Red::ApiInterfazRed::ApiInterfazRed()", "Error al poner el socket en modo de escucha");
    }
    
    // Creamos el WSAEvent para aceptar las conexiones entrantes
    eventoAceptarConexion = WSACreateEvent();
    if ( WSAEventSelect(socketEntrada, eventoAceptarConexion, FD_ACCEPT) )
    {
        throw Error::ExcepcionLocalizada("Red::ApiInterfazRed::ApiInterfazRed()", "Error al crear el evento de conexion entrante");
    }  
    
    // Inicializamos el vector de sockets con tamaño para maxJugadores
    vector <SOCKET> temp( Interfaz::Api::GetMaxJugadores() ) ;
    sockets = temp ;
    for ( vector <SOCKET>::iterator i = sockets.begin() ; i != sockets.end() ; i++ )
    {
        (*i) = INVALID_SOCKET ;
    }
    
    vector <WSAEVENT> eltemp( Interfaz::Api::GetMaxJugadores() );
    eventoLeer = eltemp ;
    for ( vector <WSAEVENT>::iterator i = eventoLeer.begin() ; i != eventoLeer.end() ; i++ )
    {
        (*i) = WSA_INVALID_EVENT ;
    }
    
    buffer = new unsigned char[0xFFF] ;
    mascaraEnvio = 0xFFFFFFFF ;
    
}

ApiInterfazRed::~ApiInterfazRed()
{
    // Cerramos el evento para acetpar conexiones entrantes
    WSACloseEvent(eventoAceptarConexion); 

    // Cerramos el socket de recepcion de conexiones
    //shutdown(socketEntrada, SD_BOTH);
    /*
    while ( true )
    {
        int ret = recv(socketEntrada, buffer, 0xFFF, 0) ;
        if ( ret == 0 || ret == SOCKET_ERROR )
        {
            break ;
        }
    }
    */
    

    closesocket(socketEntrada);
    
    // Cerramos los sockets de conexion con los jugadores
    for ( vector <SOCKET>::iterator i = sockets.begin() ; i != sockets.end() ; ++i )
    {
        if ( (*i) != INVALID_SOCKET )
        {
            closesocket(*i);
        }
    }
    
    for ( vector <WSAEVENT>::iterator i = eventoLeer.begin() ; i != eventoLeer.end() ; ++i )
    {
        if ( (*i) != WSA_INVALID_EVENT )
        {
            WSACloseEvent(*i);
        }
    }
    
    delete[] buffer ;
    mensajePendiente = "" ;
}

void 
ApiInterfazRed::Actualizar()
{
    // Aceptamos conexiones entrantes
    DWORD resultado = WSA_WAIT_TIMEOUT ;
    do
    {
        
        resultado = WSAWaitForMultipleEvents(1,
                                             &eventoAceptarConexion,
                                             false,
                                             0,
                                             false);
        
        if ( resultado != WSA_WAIT_TIMEOUT )
        {
            for ( int i = 1 ; i < sockets.size() ; i++ )
            {
                if ( sockets.at(i) == INVALID_SOCKET )
                {
                    sockets.at(i) = accept(socketEntrada, NULL, NULL) ;
                    eventoLeer.at(i) = WSACreateEvent() ;
                    if ( WSAEventSelect( sockets.at(i), eventoLeer.at(i), FD_READ ) )
                    {
                        throw Error::ExcepcionLocalizada("Red::ApiInterfazRed::Actualizar()","No se pudo crear el evento de lectura para la conexion entrante");
                    }
                    WSAResetEvent(eventoAceptarConexion);
                    break ;
                }
            }
        }
        
    }while (resultado != WSA_WAIT_TIMEOUT );
    
    // Leemos datos de conexiones establecidas
    for ( int i = 0 ; i < sockets.size() ; i++ )
    {
        if ( sockets.at(i) != INVALID_SOCKET )
        {   
            while ( RecibirMensaje(i) );      
            /*
            do
            {
                resultado = WSAWaitForMultipleEvents(1,
                                                     &eventoLeer.at(i),
                                                     false,
                                                     0,
                                                     false);
                if ( resultado != WSA_WAIT_TIMEOUT )
                {
                    RecibirMensaje(i);
                }
                WSAResetEvent(eventoLeer.at(i));
            }while ( resultado != WSA_WAIT_TIMEOUT );
            ^*/
        }
    }
    
}
 
void
ApiInterfazRed::SetMascaraEnvio(int mascara)
{
    mascaraEnvio = mascara ;
} 


bool
ApiInterfazRed::RecibirMensaje(int canal)
{
    int resultado ;
    if ( (resultado = recv( sockets.at(canal), (char *)buffer, 0xFFF, 0 )) == SOCKET_ERROR )
    {
        int error = WSAGetLastError() ;
        switch ( error )
        {
        case WSAEWOULDBLOCK:
            // No hay mas datos disponibles
            return false;
        case WSAENETDOWN:
            throw Error::ExcepcionLocalizada("Red::ApiInterfazRed::RecibirMensaje(int)","El subsistema de red ha fallado");
        case WSAENOTCONN:
        case WSAENETRESET:
        case WSAENOTSOCK:
        case WSAESHUTDOWN:
        case WSAEINVAL:
        case WSAECONNABORTED:
        case WSAETIMEDOUT:
        case WSAECONNRESET:
            // Desconectar el jugador
            //throw Error::ExcepcionLocalizada("Red::ApiInterfazRed::RecibirMensaje(int)","La conexion no es valida");
            sockets.at(canal) = INVALID_SOCKET ;
            Logica::Api::CaidaJugador(canal);
        case WSAEINPROGRESS:
            return false;
        case WSAEMSGSIZE:
            // Mensaje muy largo
            throw Error::ExcepcionLocalizada("Red::ApiInterfazRed::RecibirMensaje(int)","Mensaje recibido muy grande");
        default:
            string error = "Error de lectura en la conexion con el cliente: " ;
            char *numError = new char[30] ;
            itoa(WSAGetLastError(), numError, 10);
            error += numError ;
            delete[] numError ;
            throw Error::ExcepcionLocalizada("Red::ApiInterfazRed::RecibirMensaje(int)", error);                
        }
    }
    
    int pos = 0 ;
    do
    {
        unsigned orden = buffer[pos++] ;
        switch (orden)
        {
        case Protocolo::Cliente::UnirsePartida:
            {
                string nombre = GetString(pos);
                Api::GetApiLogica()->UnirsePartida(canal, nombre);
            }
            break ;
        case Protocolo::Cliente::MensajeChatMenu:
            {
                string texto = GetString(pos);
                Api::GetApiLogica()->MensajeChatMenu(canal, texto);
            }
            break;
        case Protocolo::Cliente::CambiarColor:
            {
                Api::GetApiLogica()->CambiarColor(canal);
            }
            break ;
        case Protocolo::Cliente::MensajeChatJuego:
            {
                string texto = GetString(pos);
                Api::GetApiLogica()->MensajeChatJuego(canal, texto);
            }
            break ;
        case Protocolo::Cliente::MoverUnidad:
            {
                int handler = GetInt(pos);
                float x = GetFloat(pos);
                float z = GetFloat(pos);
                Api::GetApiLogica()->MoverUnidad(handler, x, z);
            }
            break ;
        case Protocolo::Cliente::DetenerUnidad:
            {
                int handler = GetInt(pos);
                Api::GetApiLogica()->DetenerUnidad(handler);
            }
            break ;
        case Protocolo::Cliente::CambiarModoUnidad:
            {
                int handler = GetInt(pos);
                int modo = GetInt(pos);
                Api::GetApiLogica()->CambiarModo(handler, modo);
            }
            break ;
        case Protocolo::Cliente::ConstruirIngeniero:
            {
                int handler = GetInt(pos);
                Api::GetApiLogica()->ConstruirIngeniero(handler);
            }
            break ;
        case Protocolo::Cliente::DesarrollarMotivacion:
            {
                int handler = GetInt(pos);
                Api::GetApiLogica()->DesarrollarMotivacion(handler);
            }
            break ;
        case Protocolo::Cliente::DesarrollarMotivacionMasDistancia:
            {
                int handler = GetInt(pos);
                Api::GetApiLogica()->DesarrollarMotivacionMasDistancia(handler);
            }
            break ;
        case Protocolo::Cliente::ConstruirSoldado:
            {
                int handler = GetInt(pos);
                Api::GetApiLogica()->ConstruirSoldado(handler);
            }
            break ;
        case Protocolo::Cliente::DesarrollarArmaduraSoldado:
            {
                int handler = GetInt(pos);
                Api::GetApiLogica()->DesarrollarArmaduraSoldado(handler);
            }
            break ;
        case Protocolo::Cliente::DesarrollarVelocidadDisparoSoldado:
            {
                int handler = GetInt(pos);
                Api::GetApiLogica()->DesarrollarVelocidadDisparoSoldado(handler);
            }
            break;
        case Protocolo::Cliente::ConstruirEdificio:
            {
                int tipoEdificio = GetInt(pos);
                int numJugador = GetInt(pos);
                float x = GetFloat(pos);
                float z = GetFloat(pos);
                int numIngenieros = GetInt(pos);
                int *ingenieros = new int[numIngenieros] ;
                for ( int i = 0 ; i < numIngenieros ; i++ )
                {
                    ingenieros[i] = GetInt(pos);
                }
                Api::GetApiLogica()->ConstruirEdificio(tipoEdificio, numJugador, x, z, numIngenieros, ingenieros);
                delete[] ingenieros ;
            }
            break ;
        case Protocolo::Cliente::CorrerSoldado:
            {
                int handlerUnidad = GetInt(pos);
                bool correr = GetBool(pos);
                Api::GetApiLogica()->CorrerSoldado(handlerUnidad, correr);
            }
            break ;
        }
    }while (pos < resultado);
    
    return true ;
}
    

string
ApiInterfazRed::GetString(int &pos)
{
    string s = "" ;
    
    unsigned size = (unsigned)(buffer[pos++]) ;
    
    for ( int i = 0 ; i < size ; i++ )
    {
        s += (char)(buffer[pos++]) ;
    }
    
    return s ;
}    

bool
ApiInterfazRed::GetBool(int &pos)
{
    return buffer[pos++]==1;
}

int
ApiInterfazRed::GetInt(int &pos)
{
    int valor ;
    char *chars = (char *)(&valor);
    chars[3] = buffer[pos++];
    chars[2] = buffer[pos++];
    chars[1] = buffer[pos++];
    chars[0] = buffer[pos++];
    return valor ;
}

float
ApiInterfazRed::GetFloat(int &pos)
{
    float valor ;
    char *chars = (char *)(&valor);
    chars[3] = buffer[pos++] ;
    chars[2] = buffer[pos++];
    chars[1] = buffer[pos++];
    chars[0] = buffer[pos++];
    return valor ;
}

void
ApiInterfazRed::PutComando(int comando)
{
    mensajePendiente += (char)comando ;
}

void
ApiInterfazRed::PutBool(bool valor)
{
    mensajePendiente += (char)(valor?1:0) ;
}

void
ApiInterfazRed::PutInt(int valor)
{
    mensajePendiente += (char)(valor & 0xFF) ;
    mensajePendiente += (char)((valor & 0xFF00)>>8) ;
    mensajePendiente += (char)((valor & 0xFF0000)>>16) ;
    mensajePendiente += (char)((valor & 0xFF000000)>>24) ;
}

void
ApiInterfazRed::PutFloat(float valor)
{
    char *chars = (char *)(&valor) ;
    mensajePendiente += chars[3] ;
    mensajePendiente += chars[2] ;
    mensajePendiente += chars[1] ;
    mensajePendiente += chars[0] ;
    //PutInt( *((int *)(&valor)) );
}

void
ApiInterfazRed::PutString(string texto)
{
    if ( texto.size() > 255 )
    {
        throw Error::ExcepcionLocalizada("ApiLogicaRed::PutString(string)", "Mensaje muy largo");
    }
    mensajePendiente += (char) texto.size() ;
    mensajePendiente += texto ;
}

void 
ApiInterfazRed::EnviarMensaje()
{
    for ( int i = 0 ; i < sockets.size() ; i++ )
    {
        if ( sockets.at(i) != INVALID_SOCKET )
        {
            if ( mascaraEnvio & (1<<i) )
            {
                send(sockets.at(i), mensajePendiente.data(), mensajePendiente.size(), 0);
            }
        }
    }
    mensajePendiente = "" ;
}
    
void
ApiInterfazRed::NuevoJugador(int numJugador, string nombreJugador)
{
    if ( mascaraEnvio & 1 )
    {
        Interfaz::Api::NuevoJugador(numJugador, nombreJugador);
    }
    
    // Envio a todos los que esten conectados
    PutComando( Protocolo::Servidor::UnirsePartida );
    PutInt( numJugador ); 
    PutString( nombreJugador ) ;
    EnviarMensaje();
}

void
ApiInterfazRed::AbandonoJugador(int numJugador)
{
    if ( mascaraEnvio & 1 )
    {
        Interfaz::Api::AbandonoJugador(numJugador);
    }
    
    PutComando( Protocolo::Servidor::AbandonoJugador );
    PutInt( numJugador );
    EnviarMensaje();
}

void
ApiInterfazRed::SetJugadorPropio(int numJugador)
{
    if ( mascaraEnvio & 1 )
    {
        Interfaz::Api::SetJugadorPropio(numJugador);
    }
    
    PutComando( Protocolo::Servidor::SetJugadorPropio );
    PutInt( numJugador );
    EnviarMensaje();
}

void
ApiInterfazRed::SetColorJugador(int numJugador, int color)
{
    if ( mascaraEnvio & 1)
    {
        Interfaz::Api::SetColorJugador(numJugador, color);
    }
    
    PutComando( Protocolo::Servidor::SetColorJugador );
    PutInt(numJugador);
    PutInt(color);
    EnviarMensaje();
}

void
ApiInterfazRed::MensajeChatMenu(int numJugador, string texto)
{
    if ( mascaraEnvio & 1 )
    {
        Interfaz::Api::MensajeChatMenu(numJugador, texto);
    }
    
    PutComando( Protocolo::Servidor::MensajeChatMenu );
    PutInt( numJugador );
    PutString( texto );
    EnviarMensaje();
}

void
ApiInterfazRed::SetMapa(string nombreFichero)
{
    if ( mascaraEnvio & 1 )
    {
        Interfaz::Api::SetMapa(nombreFichero);
    }
    
    PutComando( Protocolo::Servidor::SetMapa );
    PutString( nombreFichero );
    EnviarMensaje();
}

void
ApiInterfazRed::EmpezarPartida()
{
    PutComando( Protocolo::Servidor::EmpezarPartida );
    EnviarMensaje();
    
    if ( mascaraEnvio & 1 )
    {
        Interfaz::Api::EmpezarPartida();
    }    
}

void
ApiInterfazRed::CrearUnidad(int handler, int numJugador, int tipo, float x, float y, float z)
{
    if ( mascaraEnvio & 1 )
    {
        Interfaz::Api::CrearUnidad(handler, numJugador, tipo, x, y, z);
    }
    
    PutComando( Protocolo::Servidor::CrearUnidad );
    PutInt(handler);
    PutInt(numJugador);
    PutInt(tipo);
    PutFloat(x);
    PutFloat(y);
    PutFloat(z);
    EnviarMensaje();
}

void
ApiInterfazRed::SetPosicionUnidad(int handler, float x, float y, float z)
{
    if ( mascaraEnvio & 1 )
    {
        Interfaz::Api::MoverUnidad(handler, x, y, z);
    }
    
    PutComando( Protocolo::Servidor::SetPosicionUnidad );
    PutInt(handler);
    PutFloat(x);
    PutFloat(y);
    PutFloat(z);
    EnviarMensaje();
}

void
ApiInterfazRed::SetAnguloUnidad(int handler, float angulo)
{
    if ( mascaraEnvio & 1 )
    {
        Interfaz::Api::SetAnguloUnidad(handler, angulo);
    }
    
    PutComando( Protocolo::Servidor::SetAnguloUnidad );
    PutInt(handler);
    PutFloat(angulo);
    EnviarMensaje();
}

void
ApiInterfazRed::SetPorcentajeConstruidoUnidad(int handler, int porcentajeConstruido)
{
    if ( mascaraEnvio & 1 )
    {
        Interfaz::Api::SetPorcentajeConstruidoUnidad(handler, porcentajeConstruido);
    }
    
    PutComando( Protocolo::Servidor::SetPorcentajeConstruidoUnidad );
    PutInt(handler);
    PutInt(porcentajeConstruido);
    EnviarMensaje();
}

void
ApiInterfazRed::SetEstadoTecnologia(int numJugador, int numTecnologia, int estado)
{
    if ( mascaraEnvio & 1 )
    {
        Interfaz::Api::SetEstadoTecnologia(numJugador, numTecnologia, estado);
    }
    
    PutComando( Protocolo::Servidor::SetEstadoTecnologia );
    PutInt(numJugador);
    PutInt(numTecnologia);
    PutInt(estado);
    EnviarMensaje();
}

void
ApiInterfazRed::MensajeChatJuego(int numJugador, string texto)
{
    if ( mascaraEnvio & 1 )
    {
        Interfaz::Api::MensajeChatJuego(numJugador, texto);
    }
    
    PutComando( Protocolo::Servidor::MensajeChatJuego );
    PutInt(numJugador);
    PutString(texto);
    EnviarMensaje();
}

void
ApiInterfazRed::SetDinero(int numJugador, int dinero)
{
    if ( mascaraEnvio & 1 )
    {
        Interfaz::Api::SetDinero(numJugador, dinero);
    }
    
    PutComando( Protocolo::Servidor::SetDinero );
    PutInt(numJugador);
    PutInt(dinero);
    EnviarMensaje();
}

void
ApiInterfazRed::SetOcupadoPetroleo(int numPetroleo, bool ocupado)
{
    if ( mascaraEnvio & 1 )
    {
        // Inmediato
        Interfaz::Api::SetOcupadoPetroleo(numPetroleo, ocupado) ;
    }
    
    PutComando( Protocolo::Servidor::SetOcupadoPetroleo );
    PutInt(numPetroleo);
    PutBool(ocupado);
    EnviarMensaje();
}

void
ApiInterfazRed::SetDia(bool dia)
{
    if ( mascaraEnvio & 1 )
    {
        Interfaz::Api::SetDia(dia);
    }
    
    PutComando( Protocolo::Servidor::SetDia );
    PutBool(dia);
    EnviarMensaje();
}

void
ApiInterfazRed::SetInvierno(bool invierno)
{
    if ( mascaraEnvio & 1 )
    {
        Interfaz::Api::SetInvierno(invierno);
    }
    
    PutComando( Protocolo::Servidor::SetInvierno );
    PutBool(invierno);
    EnviarMensaje();
}

}

