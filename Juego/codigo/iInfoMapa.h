#ifndef __I_INFOMAPA_H__
#define __I_INFOMAPA_H__

#include <allegro.h>
#include <alleggl.h>

#include <string>
#include <vector>

#include "uVector2D.h"

using namespace std ;


namespace Interfaz
{

class InfoMapa
{
private:
    string nombreFichero ;
    string nombre ;
    string directorio ;
    string ficheroPreview ;
    string ficheroAlturas ;
    int maxJugadores ;
    vector<Util::Vector2D> inicios ;
    
    int numPetroleos ;
    vector<Util::Vector2D> petroleos ;
    vector<bool> petroleoOcupado ;
    
    bool correcto ;        
    
    void Parse(string NombreFicheroMap);
public:
    InfoMapa(string NombreFicheroMap);
    ~InfoMapa();
    bool Correcto();
    string GetNombreFichero(){return nombreFichero;}
    string GetNombre();
    string GetFicheroPreview();
    string GetFicheroAlturas();
    int GetMaxJugadores();
    Util::Vector2D GetPosicionInicio(int pos);
    int GetNumPetroleos();
    Util::Vector2D GetPosicionPetroleo(int num);
    bool GetPetroleoOcupado(int num);
    void SetPetroleoOcupado(int num, bool ocu);
};

}

#endif
