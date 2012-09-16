#include <allegro.h>
#include <fstream>

#include "iInfoMapa.h"
#include "eExcepcionLocalizada.h"

using namespace std;

namespace Interfaz
{

//Obtiene la informacion sobre un mapa del fichero especificado
InfoMapa::InfoMapa(string NombreFicheroMap)
{
    try
    {
        inicios.clear();
        nombreFichero = NombreFicheroMap ;
        Parse("Mapas\\" + NombreFicheroMap);
        if ( correcto )
        {
            ficheroPreview = "Mapas\\" + directorio + "preview.bmp" ;
            ficheroAlturas = "Mapas\\" + directorio + "alturas.bmp" ;
        }
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Interfaz::InfoMapa::InfoMapa(string)").Combinar(ex);
    }
}

InfoMapa::~InfoMapa()
{
    inicios.clear();
}

//Devuelve true si el fichero comprobado es un fichero de mapa válido
bool
InfoMapa::Correcto()
{
    return correcto ;
}

//Obtiene el nombre del mapa
string
InfoMapa::GetNombre()
{
    return nombre ;
}

//Obtiene el nombre del fichero de preview (minimapa)
string
InfoMapa::GetFicheroPreview()
{
    return ficheroPreview ;
}

//Obtiene el nombre del fichero de alturas
string
InfoMapa::GetFicheroAlturas()
{
    return ficheroAlturas ;
}

//Obtiene el maximo de jugadores
int
InfoMapa::GetMaxJugadores()
{
    return maxJugadores ;
}

//Lee y comprueba que el fichero de mapa pasado sea valido
//completando todos los campos
void
InfoMapa::Parse(string NombreFicheroMap)
{
    correcto = false ;
    string temp ;
    
    // Abrimos el fichero    
    fstream fs(NombreFicheroMap.c_str(), ios_base::in );

    // Leemos el nombre del mapa
    temp = "" ;
    fs >> temp ;
    if ( temp != "NombreMapa" )
    {
        throw Error::ExcepcionLocalizada("Interfaz::InfoMapa::Parse(string)");
    }
    
    nombre = "" ;
    for ( temp = "", fs >> temp ; temp != "#" ; temp = "", fs >> temp )
    {
        nombre += temp + " ";
    }
    if ( nombre == "" )
    {
        throw Error::ExcepcionLocalizada("Interfaz::InfoMapa::Parse(string)");
    }
    
    temp = "" ;
    fs >> temp ;
    if ( temp != "Directorio" )
    {
        throw Error::ExcepcionLocalizada("Interfaz::InfoMapa::Parse(string)");
    }
    fs >> directorio ;
    
    temp = "" ;
    fs >> temp ;
    if ( temp != "MaxJugadores" )
    {
        throw Error::ExcepcionLocalizada("Interfaz::InfoMapa::Parse(string)");
    }
    fs >> maxJugadores ;
    
    // Leemos los lugares de inicio
    inicios.clear();
    for ( int i = 0 ; i < maxJugadores ; i++ )
    {
        int x, y ;
        fs >> x >> y ;
        inicios.push_back(Util::Vector2D(x,y));
    }
    
    temp = "" ;
    fs >> temp ;
    if ( temp != "NumPetroleos" )
    {
        throw Error::ExcepcionLocalizada("Interfaz::InfoMapa::Parse(string)");
    }
    fs >> numPetroleos ;
    
    
    // Leemos las posiciones de petroleo
    petroleos.clear();
    for ( int i = 0 ; i < numPetroleos ; i++ )
    {
        int x, y ;
        fs >> x >> y ;
        petroleos.push_back(Util::Vector2D(x,y));
        petroleoOcupado.push_back(false);
    }
    
    correcto = true ;
}

Util::Vector2D
InfoMapa::GetPosicionInicio(int pos)
{
    return inicios.at(pos);
}

int
InfoMapa::GetNumPetroleos()
{
    return numPetroleos ;
}

Util::Vector2D
InfoMapa::GetPosicionPetroleo(int num)
{
    return petroleos.at(num);
}

bool
InfoMapa::GetPetroleoOcupado(int num)
{
    return petroleoOcupado.at(num);
}

void
InfoMapa::SetPetroleoOcupado(int num, bool ocu)
{
    petroleoOcupado.at(num) = ocu ;
}

}
