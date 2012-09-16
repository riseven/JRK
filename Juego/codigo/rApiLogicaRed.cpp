#include "rApiLogicaRed.h"
#include "lApi.h"
#include "iApi.h"
#include "iConfiguracion.h"
#include "rProtocolo.h"
#include "eExcepcionLocalizada.h"
#include "rApi.h"

namespace Red
{

ApiLogicaRed::ApiLogicaRed()
{
    buffer = NULL ;
    eventoLeer = WSA_INVALID_EVENT ;
    
    // Creamos el socket
    socketComunicacion = socket( AF_INET, SOCK_STREAM, 0 );
    if ( socketComunicacion == INVALID_SOCKET )
    {
        throw Error::ExcepcionLocalizada("Red::ApiLogicaRed::ApiLogicaRed()");
    }
    
    // Conectamos con el servidor
    sockaddr_in dirRemota ;
    dirRemota.sin_family = AF_INET ;
    u_long dir = inet_addr( Interfaz::Configuracion::GetIpServidor().c_str() );
    if ( dir == INADDR_NONE )
    {
        throw Error::ExcepcionLocalizada("Red::ApiLogicaRed::ApiLogicaRed()", "Direccion no valida");
    }
    dirRemota.sin_addr.s_addr = dir ;
    dirRemota.sin_port = htons(1984);
    if ( connect(socketComunicacion, (sockaddr *)&dirRemota, sizeof(sockaddr_in)) )
    {
        throw Error::ExcepcionLocalizada("Red::ApiLogicaRed::ApiLogicaRed()", "No se pudo conectar con el servidor");    
    }
    
    // Creamos el evento de lectura
    eventoLeer = WSACreateEvent();
    if ( WSAEventSelect( socketComunicacion, eventoLeer, FD_READ ) )
    {
        throw Error::ExcepcionLocalizada("Red::ApiLogicaRed::Actualizar()","No se pudo crear el evento de lectura para la conexion con el servidor");
    }
    
    buffer = new unsigned char[20000] ;
    for ( int i = 0 ; i < 20000 ; i++ )
    {
        buffer[i] = 215 ;
    }
    posLectura = 0 ;
    posEscritura = 0 ;
    primerTope = 10000 ;
    segundoTope = 20000 ;
    posMaxima = primerTope ;    
}

ApiLogicaRed::~ApiLogicaRed()
{
    if ( buffer )
    {
        delete[] buffer ;
    }
    
    if ( socketComunicacion != INVALID_SOCKET )
    {
        closesocket(socketComunicacion);
    }
    
    if ( eventoLeer != WSA_INVALID_EVENT )
    {
        WSACloseEvent(eventoLeer);
    }
}

void
ApiLogicaRed::Actualizar()
{
    bool resultado ;
    do
    {
        resultado = RecibirMensaje() ;
    }while ( resultado );
    // Leemos datos mientras hayan disponibles
    /*
    int resultado ;
    do
    {
        resultado = WSAWaitForMultipleEvents(1,
                                             &eventoLeer,
                                             false,
                                             0,
                                             false);
        if ( resultado != WSA_WAIT_TIMEOUT )
        {
            RecibirMensaje();
        }
        WSAResetEvent(eventoLeer);
    } while ( resultado != WSA_WAIT_TIMEOUT ) ;
    */
}

bool
ApiLogicaRed::RecibirMensaje()
{
    int resultado ;
    int espacio ;
    if ( posEscritura >= posLectura )
    {
        espacio = segundoTope - posEscritura ;
        if ( espacio > 1000 ) espacio = 1000 ;
    }
    else
    {
        espacio = posLectura - posEscritura ;
    }
    
    if ( (resultado = recv( socketComunicacion, (char *)(&buffer[posEscritura]), espacio, 0 )) == SOCKET_ERROR )
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
            throw Error::ExcepcionLocalizada("Red::ApiInterfazRed::RecibirMensaje(int)","La conexion no es valida");
        case WSAEINPROGRESS:
            return false;
        case WSAEMSGSIZE:
            // Mensaje muy largo
            throw Error::ExcepcionLocalizada("Red::ApiInterfazRed::RecibirMensaje(int)","Mensaje recibido muy grande");
        default:
            string error = "Error de lectura en la conexion con el servidor: " ;
            char *numError = new char[30] ;
            itoa(WSAGetLastError(), numError, 10);
            error += numError ;
            delete[] numError ;
            throw Error::ExcepcionLocalizada("Red::ApiInterfazRed::RecibirMensaje(int)", error);                
        }
    }

    posEscritura += resultado ;
    if ( posEscritura > posLectura )
    {
        if ( posEscritura >= primerTope )
        {
            posMaxima = posEscritura ;
        }
    }

    if ( posEscritura >= primerTope && posEscritura > posLectura)
    {
        posEscritura = 0 ;
    }

    
    if ( PendienteLeer() < 1 )
    {
        return false;
    }
    

    
    int pos = 0 ;
    do
    {
        unsigned orden = 0 ;
        orden = (unsigned)(buffer[posLectura]) ;
        
        // Comprobamos si se puede leer
        int necesario = 1 ;
        int cantidad ;
        switch (orden)
        {
        case Protocolo::Servidor::UnirsePartida:
        case Protocolo::Servidor::MensajeChatMenu: 
        case Protocolo::Servidor::MensajeChatJuego:           
            if ( PendienteLeer() < 5 )
            {
                necesario = 5 ;
                break ;
            }          
            cantidad = (unsigned)(buffer[GetSiguientePosicion(5)]) ;
            necesario += cantidad + 5 ;
        case Protocolo::Servidor::AbandonoJugador:
        case Protocolo::Servidor::SetJugadorPropio:
            necesario += 4 ;
            break ;
        case Protocolo::Servidor::SetColorJugador:
            necesario += 8 ;
            break ;
        case Protocolo::Servidor::SetMapa:   
            if ( PendienteLeer() < 1 )
            {
                necesario = 1 ;
                break ;
            }
            cantidad = (unsigned)(buffer[GetSiguientePosicion()]) ;
            necesario += cantidad + 1 ;
            break ;
        case Protocolo::Servidor::EmpezarPartida:
            necesario += 0 ;
            break ;
        case Protocolo::Servidor::CrearUnidad:
            necesario += 24 ;
            break ;
        case Protocolo::Servidor::SetPosicionUnidad:
            necesario += 16 ;
            break ;
        case Protocolo::Servidor::SetAnguloUnidad:
        case Protocolo::Servidor::SetPorcentajeConstruidoUnidad:
        case Protocolo::Servidor::SetDinero:            
            necesario += 8 ;
            break ;
        case Protocolo::Servidor::SetEstadoTecnologia:
            necesario += 12 ;
            break ;
        case Protocolo::Servidor::SetOcupadoPetroleo:
            necesario += 5 ;
            break ;
        case Protocolo::Servidor::SetDia:
        case Protocolo::Servidor::SetInvierno:
            necesario += 1 ;
            break ;
        }
        
        if ( necesario > PendienteLeer() )
        {
            break ;
        }
        
        IncrementarPosicion();        
        switch (orden)
        {
        case Protocolo::Servidor::UnirsePartida:
            {
                int numJugador = GetInt(pos);
                string nombre = GetString(pos);
                Api::GetApiInterfaz()->NuevoJugador(numJugador, nombre);
            }
            break ;
        case Protocolo::Servidor::AbandonoJugador:
            {
                int numJugador = GetInt(pos);
                Api::GetApiInterfaz()->AbandonoJugador(numJugador);
            }
            break ;
        case Protocolo::Servidor::SetJugadorPropio:
            {
                int numJugador = GetInt(pos);
                Api::GetApiInterfaz()->SetJugadorPropio(numJugador);
            }
            break ;
        case Protocolo::Servidor::SetColorJugador:
            {
                int numJugador = GetInt(pos);
                int color = GetInt(pos);
                Api::GetApiInterfaz()->SetColorJugador(numJugador, color);
            }
            break ;
        case Protocolo::Servidor::MensajeChatMenu:
            {
                int numJugador = GetInt(pos);
                string texto = GetString(pos);
                Api::GetApiInterfaz()->MensajeChatMenu(numJugador, texto);
            }
            break ;
        case Protocolo::Servidor::SetMapa:
            {
                string nombreFichero = GetString(pos);
                Api::GetApiInterfaz()->SetMapa(nombreFichero);
            }
            break ;
        case Protocolo::Servidor::EmpezarPartida:
            {
                Api::GetApiInterfaz()->EmpezarPartida();
            }
            break ;
        case Protocolo::Servidor::CrearUnidad:
            {
                int handler = GetInt(pos);
                int numJugador = GetInt(pos);
                int tipo = GetInt(pos);
                float x = GetFloat(pos);
                float y = GetFloat(pos);
                float z = GetFloat(pos);
                Api::GetApiInterfaz()->CrearUnidad(handler, numJugador, tipo, x, y, z);
            }
            break ;
        case Protocolo::Servidor::SetPosicionUnidad:
            {
                int handler = GetInt(pos);
                float x = GetFloat(pos);
                float y = GetFloat(pos);
                float z = GetFloat(pos);
                Api::GetApiInterfaz()->SetPosicionUnidad(handler, x, y, z);
            }
            break ;
        case Protocolo::Servidor::SetAnguloUnidad:
            {
                int handler = GetInt(pos);
                float angulo = GetFloat(pos);
                Api::GetApiInterfaz()->SetAnguloUnidad(handler, angulo);
            }
            break ;
        case Protocolo::Servidor::SetPorcentajeConstruidoUnidad:
            {
                int handler = GetInt(pos);
                int porcentajeConstruido = GetInt(pos);
                Api::GetApiInterfaz()->SetPorcentajeConstruidoUnidad(handler, porcentajeConstruido);
            }
            break ;
        case Protocolo::Servidor::SetEstadoTecnologia:
            {
                int numJugador = GetInt(pos);
                int numTecnologia = GetInt(pos);
                int estado = GetInt(pos);
                Api::GetApiInterfaz()->SetEstadoTecnologia(numJugador, numTecnologia, estado);
            }
            break ;
        case Protocolo::Servidor::MensajeChatJuego:
            {
                int numJugador = GetInt(pos);
                string texto = GetString(pos);
                Api::GetApiInterfaz()->MensajeChatJuego(numJugador, texto);
            }
            break ;
        case Protocolo::Servidor::SetDinero:
            {
                int numJugador = GetInt(pos);
                int dinero = GetInt(pos);
                Api::GetApiInterfaz()->SetDinero(numJugador, dinero);
            }
            break ;
        case Protocolo::Servidor::SetOcupadoPetroleo:
            {
                int numPetroleo = GetInt(pos);
                bool ocupado = GetBool(pos);
                Api::GetApiInterfaz()->SetOcupadoPetroleo(numPetroleo, ocupado);
            }
            break ;
        case Protocolo::Servidor::SetDia:
            {
                bool dia = GetBool(pos);
                Api::GetApiInterfaz()->SetDia(dia);
            }
            break ;
        case Protocolo::Servidor::SetInvierno:
            {
                bool invierno = GetBool(pos);
                Api::GetApiInterfaz()->SetInvierno(invierno);
            }
            break ;
        default:
            {                
                string error = "Op no reconocida Num: " ;
                char *textoNum = new char[30];
                itoa(orden, textoNum, 10);
                error += textoNum ;
                delete[] textoNum ;
                throw Error::ExcepcionLocalizada("ApiLogicaRed::RecibirMensaje()", error);
            }
        }
    }while (PendienteLeer()>0);
    
    return true ;
}

void 
ApiLogicaRed::EnviarMensaje()
{
    send(socketComunicacion, mensajePendiente.data(), mensajePendiente.size(), 0);
    mensajePendiente = "" ;
}


void
ApiLogicaRed::PutComando(int comando)
{
    mensajePendiente += (char)comando ;
}

void
ApiLogicaRed::PutString(string texto)
{
    if ( texto.size() > 255 )
    {
        throw Error::ExcepcionLocalizada("ApiLogicaRed::PutString(string)", "Mensaje muy largo");
    }
    mensajePendiente += (char) texto.size() ;
    mensajePendiente += texto ;
}

void
ApiLogicaRed::PutBool(bool valor)
{
    mensajePendiente += (char)(valor?1:0) ;
}

void
ApiLogicaRed::PutInt(int valor)
{
    char *chars = (char *)(&valor) ;
    mensajePendiente += chars[3] ;
    mensajePendiente += chars[2] ;
    mensajePendiente += chars[1] ;
    mensajePendiente += chars[0] ;
}

void
ApiLogicaRed::PutFloat(float valor)
{
    char *chars = (char *)(&valor) ;
    mensajePendiente += chars[3] ;
    mensajePendiente += chars[2] ;
    mensajePendiente += chars[1] ;
    mensajePendiente += chars[0] ;
}    

void
ApiLogicaRed::IncrementarPosicion()
{
    posLectura = (posLectura+1)%posMaxima ;
}

int
ApiLogicaRed::GetSiguientePosicion()
{
    return (posLectura+1)%posMaxima ;
}

int
ApiLogicaRed::GetSiguientePosicion(int cantidad)
{
    return (posLectura+cantidad)%posMaxima ;
}


int
ApiLogicaRed::PendienteLeer()
{
    if ( posLectura <= posEscritura )
    {
        return posEscritura - posLectura ;
    }
    else
    {
        return (posMaxima - posLectura)+posEscritura ;
    }
}

bool
ApiLogicaRed::GetBool(int &pos)
{
    bool valor = buffer[posLectura]==1 ;
    IncrementarPosicion();
    return valor;
}

int
ApiLogicaRed::GetInt(int &pos)
{
    int valor ;
    valor = ((int)(buffer[posLectura])) ;
    IncrementarPosicion();
    valor += ((int)(buffer[posLectura])) << 8 ;
    IncrementarPosicion();
    valor += ((int)(buffer[posLectura])) << 16 ;
    IncrementarPosicion();
    valor += ((int)(buffer[posLectura])) << 24 ;
    IncrementarPosicion();
    return valor ;
}

float
ApiLogicaRed::GetFloat(int &pos)
{
    float valor ;
    char *chars = (char *)(&valor);
    chars[3] = buffer[posLectura] ;
    IncrementarPosicion();
    chars[2] = buffer[posLectura] ;
    IncrementarPosicion();
    chars[1] = buffer[posLectura] ;
    IncrementarPosicion();
    chars[0] = buffer[posLectura] ;
    IncrementarPosicion();
    return valor ;
}

string
ApiLogicaRed::GetString(int &pos)
{
    string s = "" ;
    
    unsigned size = (unsigned)(buffer[posLectura]) ;
    IncrementarPosicion();    
    
    for ( int i = 0 ; i < size ; i++ )
    {
        s += (char)(buffer[posLectura]) ;
        IncrementarPosicion();        
    }
    
    return s ;
}


void
ApiLogicaRed::UnirsePartida(int numJugador, string nombreJugador)
{
    PutComando( Protocolo::Cliente::UnirsePartida );
    PutString( nombreJugador );
    EnviarMensaje();
}

void
ApiLogicaRed::MensajeChatMenu(int numJugador, string texto)
{
    PutComando( Protocolo::Cliente::MensajeChatMenu );
    PutString( texto );
    EnviarMensaje();
}

void
ApiLogicaRed::CambiarColor(int numJugador)
{
    PutComando( Protocolo::Cliente::CambiarColor );
    EnviarMensaje();
}

void
ApiLogicaRed::MensajeChatJuego(int numJugador, string texto)
{
    PutComando( Protocolo::Cliente::MensajeChatJuego );
    PutString(texto);
    EnviarMensaje();
}

void
ApiLogicaRed::MoverUnidad(int handler, float x, float z)
{
    PutComando( Protocolo::Cliente::MoverUnidad );
    PutInt(handler);
    PutFloat(x);
    PutFloat(z);
    EnviarMensaje();
}

void
ApiLogicaRed::DetenerUnidad(int handler)
{
    PutComando( Protocolo::Cliente::DetenerUnidad );
    PutInt(handler);
    EnviarMensaje();
}

void
ApiLogicaRed::CambiarModo(int handler, int modo)
{
    PutComando( Protocolo::Cliente::CambiarModoUnidad );
    PutInt(handler);
    PutInt(modo);
    EnviarMensaje();
}

void
ApiLogicaRed::ConstruirIngeniero(int handler)
{
    PutComando( Protocolo::Cliente::ConstruirIngeniero );
    PutInt(handler);
    EnviarMensaje();
}

void
ApiLogicaRed::DesarrollarMotivacion(int handler)
{
    PutComando( Protocolo::Cliente::DesarrollarMotivacion );
    PutInt(handler);
    EnviarMensaje();
}

void
ApiLogicaRed::DesarrollarMotivacionMasDistancia(int handler)
{
    PutComando( Protocolo::Cliente::DesarrollarMotivacionMasDistancia );
    PutInt(handler);
    EnviarMensaje();
}

void
ApiLogicaRed::ConstruirSoldado(int handler)
{
    PutComando( Protocolo::Cliente::ConstruirSoldado );
    PutInt(handler);
    EnviarMensaje();
}

void
ApiLogicaRed::DesarrollarArmaduraSoldado(int handler)
{
    PutComando( Protocolo::Cliente::DesarrollarArmaduraSoldado );
    PutInt(handler);
    EnviarMensaje();
}

void
ApiLogicaRed::DesarrollarVelocidadDisparoSoldado(int handler)
{
    PutComando( Protocolo::Cliente::DesarrollarVelocidadDisparoSoldado );
    PutInt(handler);
    EnviarMensaje();
}

void
ApiLogicaRed::ConstruirEdificio(int tipoEdificio, int numJugador, float x, float z, int numIngenieros, int *ingenieros)
{
    PutComando( Protocolo::Cliente::ConstruirEdificio );
    PutInt(tipoEdificio);
    PutInt(numJugador);
    PutFloat(x);
    PutFloat(z);
    PutInt(numIngenieros);
    for ( int i = 0 ; i < numIngenieros ; i++ )
    {
        PutInt(ingenieros[i]);
    }
    EnviarMensaje();
}

void
ApiLogicaRed::CorrerSoldado(int handlerUnidad, bool correr)
{
    PutComando( Protocolo::Cliente::CorrerSoldado );
    PutInt(handlerUnidad);
    PutBool(correr);
    EnviarMensaje();
}

}
