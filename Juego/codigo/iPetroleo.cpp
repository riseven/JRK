#include "iPetroleo.h"
#include "gApi.h"
#include "iApi.h"
#include "rApi.h"
#include "rProtocolo.h"
#include <cstdlib>

namespace Interfaz
{

int Petroleo::imagenPetroleo ;
int Petroleo::modeloHandler ;


//Creo una unidad Soldado Raso a partir de su modelo
Petroleo::Petroleo():
    Unidad()
{
    objetoHandler = Graficos::Api::CrearObjeto(modeloHandler);
    Graficos::Api::SetObjetoBloqueante(objetoHandler, true);
}

//Destruyo la unidad
Petroleo::~Petroleo()
{
}


void
Petroleo::Inicializar()
{
    imagenPetroleo = Graficos::Api::CrearImagen("Unidades\\Petroleo\\petroleo.bmp");
    modeloHandler = Graficos::Api::CrearModeloQuad(imagenPetroleo, 1, 1, 1, 1);

    Api::SetProgresoCarga( Api::GetProgresoCarga()+1.0 );
    Graficos::Api::SetPorcentajeBarraVida(Api::GetBarraProgresoCarga(), Api::GetProgresoCarga());
    Graficos::Api::Pintar();
}

void
Petroleo::Reset()
{
}

}
