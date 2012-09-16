#ifndef __I_CONFIGURACION_H__
#define __I_CONFIGURACION_H__

#include <string>

using namespace std;

namespace Interfaz
{

class Configuracion
{
private:
    static string nombreDirectorio ;
    static string nombreJugador ;
    static string ipServidor ;
    static bool multiTextura ;
    static bool noche ;
    static bool estaciones ;
    
public:
    static void SetNombreDirectorio(string NombreDirectorio);
    static string GetNombreDirectorio();
    static void SetNombreJugador(string NombreJugador);
    static string GetNombreJugador();
    static void SetIpServidor(string ipServidor);
    static string GetIpServidor();
    static void SetMultiTextura(bool multiTextura);
    static bool GetMultiTextura();
    static void SetNoche(bool noche);
    static bool GetNoche();
    static void SetEstaciones(bool estaciones);
    static bool GetEstaciones();
};

}

#endif
