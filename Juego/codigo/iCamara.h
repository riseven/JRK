#ifndef __I_CAMARA_H__
#define __I_CAMARA_H__

namespace Interfaz
{

// Esta clase gestiona la camara en el modo de vista cenital
class Camara
{
private:
    int x, z ;
    
    float posicion[4] ;
    float deseado[4] ;
    float velocidad[4] ;
    float aceleracion[4] ;
    
public:
    static const int BordeDerecho = 1 ;
    static const int BordeIzquierdo = 2 ;
    static const int BordeSuperior = 4 ;
    static const int BordeInferior = 8 ;
    
    
    Camara();
    ~Camara();
    void SetPosicionDeseada(int x, int z);
    void SetInclinacion(int angulo);
    void DesplazamientoBordes(int bordes);
    void InclinarCamara(int difPasos);
    void Actualizar();
    void FijarPosicion();
    void FijarInclinacion();
    void ForzarPosicion();
};
    
}

#endif
