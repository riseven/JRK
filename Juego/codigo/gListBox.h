#ifndef __G_LISTBOX_H__
#define __G_LISTBOX_H__

#include <vector>

#include "gEntidad2D.h"
#include "gImagen.h"

using namespace std ;

namespace Graficos
{
    
class ListBox : public Entidad2D
{
    int leftClient, topClient, bottomClient , rightClient;
    int leftScroll, topScroll, bottomScroll , rightScroll;
    Imagen *imagenFondo ;
    Imagen *imagenSeleccion ;
    Imagen *imagenResaltado ;
    Imagen *imagenScroll ;
    Imagen *imagenSeleccionScroll ;
    Fuente *fuenteTexto ;
    vector <Imagen *> lineas ;
    int posicion ;
    int altoLinea ;
    int lineasCaben ;
    int lineaSeleccionada ;
    bool *outCambio ;
    int *outSeleccionado ;

public:
    ListBox(Imagen *ImagenFondo, Imagen *ImagenSeleccion, Imagen *ImagenResaltado,
            Imagen *ImagenScroll, Imagen *ImagenSeleccionScroll, Fuente *fuente,
            int LeftClient, int TopClient, int BottomClient,
            int LeftScroll, int TopScroll, int BottomScroll,
            int X, int Y, int Z);
    ~ListBox();
    void AddLinea(string Texto);
    void OnCambio(bool *Cambio, int *Seleccionado);
    void Pintar();    
};
    
}

#endif // __G_LISTBOX_H__
