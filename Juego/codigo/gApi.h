#ifndef __G_API_H__
#define __G_API_H__

#include <Allegro.h>
#include <Alleggl.h>
#include <GL/Glu.h>
#include <vector>
#include <list>
#include <iostream>

#include "gImagen.h"
#include "gSegmento.h"
#include "gRectangulo.h"
#include "gBarraVida.h"
#include "gSprite.h"
#include "gBoton.h"
#include "gBarraProgreso.h"
#include "gFuente.h"
#include "gTextBox.h"
#include "gChatBox.h"
#include "gListBox.h"
#include "gTerreno.h"
#include "gEntidad3D.h"
#include "gModelo.h"
#include "gObjeto3D.h"
#include "gSonido.h"
#include "uFrecReal.h"
#include "uVector2D.h"

using namespace std;

namespace Graficos
{

class Api
{
private:
    static BITMAP *page[2] ;
    static int pageActual ;
    
    static vector <Imagen *> imagenes ;
    static vector <Segmento *> segmentos ;
    static vector <Rectangulo *> rectangulos ;
    static vector <BarraVida *> barrasVida ;
    static vector <Sprite *> sprites ;
    static vector <Boton *> botones ;
    static vector <BarraProgreso *> barrasProgreso ;
    static vector <Fuente *> fuentes ;
    static vector <TextBox *> textBoxs ;
    static vector <ChatBox *> chatBoxs ;
    static vector <ListBox *> listBoxs ;
    static vector <Modelo *> modelos ;
    static vector <Objeto3D *> objetos ;
    static vector <Sonido *> sonidos ;
    
    static Terreno *terreno ;
    
    static int HandlerImagenRaton ;
    
    static bool mostrarRaton ;
    
    static const int maxCallLists = 10000;
    static const int callListsAltos = 1000;
    static bool callLists[maxCallLists] ;
    static bool clipping[maxCallLists] ;
    
    static float camaraX, camaraY, camaraZ ;
    static float camaraIncX ;
    
    static float diffuseLight0[4] ;
    static float desDiffuseLight0[4] ;
    static float velDiffuseLight0[4] ;
    
    static float ambientLight0[4] ;
    static float desAmbientLight0[4] ;
    static float velAmbientLight0[4] ;

    
    static Util::FrecReal frecuenciaClipping;
    
    static Util::Vector3D coordenadasVentana[6] ;
    
    static Util::Vector3D GetCoordenadas2Dto3D(float wx, float wy);    
    
public:
    class CoordenadasVentana
    {
    public:
        enum EnumCoordenadasVentana
        {
            ArribaIzquierda,
            ArribaDerecha,
            AbajoIzquierda,
            AbajoDerecha,
            CentroVentana,
            PosicionRaton,
        };  
    };
    
    
    // Generales
    static bool Inicializar(int Ancho, int Alto);
    static void Cerrar();
    static void Pintar();
        
    // Imagenes
    static int  CrearImagen(string NombreFichero);
    static int  CrearImagen(int HandlerFuente, string texto);
    static int  CrearImagen(int handlerFuente, string texto, int r, int g, int b);
    static void DestruirImagen(int HandlerImagen);
    static void SetImagenMasked(int HandlerImagen, bool Masked);
    static void PintarPixel(int HandlerImagen,int x, int y, char r, char g, char b);
    static void AplicarMascara(int HandlerImagen);
    
    // Fuentes
    static int  CrearFuente(string NombreFichero);
    static void DestruirFuente(int HandlerFuente);
    
    // Segmentos
    static int  CrearSegmento(float r, float g, float b, float a, int x, int y, int finX, int finY, int z);
    static void DestruirSegmento(int handlerSegmento);
    static void ActivarSegmento(int handlerSegmento);
    static void DesactivarSegmento(int handlerSegmento);
    static void MoverSegmento(int handlerSegmento, int x, int y, int finX, int finY);
    static void SetClippingSegmento(int handlerSegmento, int x0, int y0, int x1, int y1);
    
    // Rectangulos
    static int  CrearRectangulo(int ancho, int alto, float r, float g, float b, float a, bool relleno, int x, int y, int z);
    static void DestruirRectangulo(int handlerRectangulo);
    static void ActivarRectangulo(int handlerRectangulo);
    static void DesactivarRectangulo(int handlerRectangulo);
    static void MoverRectangulo(int handlerRectangulo, int x, int y);
    static void CambiarTamanyoRectangulo(int handlerRectangulo, int ancho, int alto);
    static void SetColorRectangulo(int handlerRectangulo, float r, float g, float b, float a);
    
    // Barras de vida
    static int  CrearBarraVida(int ancho, int alto, int borde, int X, int Y, int Z);
    static void DestruirBarraVida(int handlerBarraVida);
    static void ActivarBarraVida(int handlerBarraVida);
    static void DesactivarBarraVida(int handlerBarraVida);
    static void MoverBarraVida(int handlerBarraVida, int x, int y);
    static void CentrarBarraVida(int handlerBarraVida, int x, int y);
    static void SetPorcentajeBarraVida(int handlerBarraVida, float porcentaje);
    
    // Sprites
    static int  CrearSprite(int HandlerImagen, int X, int Y, int Z);
    static void DestruirSprite(int HandlerSprite);
    static void ActivarSprite(int HandlerSprite);
    static void DesactivarSprite(int HandlerSprite);
    static void MoverSprite(int HandlerSprite, int x, int y);
    static int  GetAnchoSprite(int handlerSprite);
    static int  GetAltoSprite(int handlerSprite);
    static void SetAlphaSprite(int handlerSprite, int alpha);
    
    // Botones
    static int  CrearBoton(int HandlerImagenFondo, int HandlerImagenSeleccion, int HandlerFuente, string texto, int X, int Y, int Z);
    static void DestruirBoton(int HandlerBoton);
    static void ActivarBoton(int HandlerBoton);
    static void DesactivarBoton(int HandlerBoton);
    static void OnClickBoton(int HandlerBoton, bool *OnClickFlag);
    static void OnMouseOver(int handlerBoton, bool *flag);
    static void CambiarFondoBoton(int HandlerBoton, int HandlerImagen);
    static void MoverBoton(int handlerBoton, int x, int y);
    static void SetFijoBoton(int handlerBoton, bool fijo);
    static bool GetFijoBoton(int handlerBoton);
    
    // BarraProgreso
    static int  CrearBarraProgreso(int handlerImagenFondo, int handlerImagenProgreso, int bordeLateral, int separacion, int X, int Y, int Z);
    static void DestruirBarraProgreso(int handlerBarraProgreso);
    static void ActivarBarraProgreso(int handlerBarraProgreso);
    static void DesactivarBarraProgreso(int handlerBarraProgreso);
    static void SetPorcentajeBarraProgreso(int handlerBarraProgreso, int porcentaje);
    
    // TextBoxs
    static int  CrearTextBox(int HandlerImagenFondo, int HandlerFuente, string texto, int X, int Y, int Z);
    static void DestruirTextBox(int HandlerTextBox);
    static void ActivarTextBox(int HandlerTextBox);
    static void DesactivarTextBox(int HandlerTextBox);
    static void SetTextoTextBox(int handlerTextBox, string texto);
    static string GetTextoTextBox(int HandlerTextBox);
    static void OnEnterTextBox(int handlerTextBox, bool *flag);
    
    // ChatBoxs
    static int  CrearChatBox(int handlerImagenFondo, int handlerFuente, int ancho, int alto, int borde, int x, int y, int z);
    static void DestruirChatBox(int handlerChatBox);
    static void ActivarChatBox(int handlerChatBox);
    static void DesactivarChatBox(int handlerChatBox);
    static void AddLineaChatBox(int handlerChatBox, string texto, int r, int g, int b);
    
    // ListBoxs
    static int  CrearListBox(int HandlerImagenFondo, int HandlerImagenSeleccion, int HandlerImagenResaltado, int HandlerImagenScroll, int HandlerImagenSeleccionScroll, int HandlerFuente, int LeftClient, int TopClient, int BottomClient, int LeftScroll, int TopScroll, int BottomScroll, int X, int Y, int Z);
    static void DestruirListBox(int HandlerListBox);
    static void AddLineaListBox(int HandlerListBox, string Texto);
    static void OnCambioListBox(int HandlerListBox, bool *Cambio, int *Seleccionado);
    static void ActivarListBox(int HandlerListBox);
    static void DesactivarListBox(int HandlerListBox);
    
    // Modelos
    static int  CrearModelo(string FicheroModelo);
    static int  CrearModelo(string ficheroModelo, int imagenTextura, float r, float g, float a);
    static int  CrearModeloQuad(int handlerImagenTextura, float r, float g, float b, float a);
    static void DestruirModelo(int HandlerModelo);
    static float GetAlturaModelo(int handlerModelo);
    
    // Objetos
    static int  CrearObjeto(int HandlerModelo);
    static void DestruirObjeto(int HandlerObjeto);
    static void MoverObjeto(int HandlerObjeto, float x, float y, float z);
    static void SetAnguloObjeto(int HandlerObjeto, float angulo);
    static void SetObjetoSeleccionable(int HandlerObjeto, bool Seleccionable);
    static int  GetObjetosEn(int x, int y, int *buffer, int max);
    static int  GetObjetosEnRectangulo(int x, int y, int ancho, int alto, int *buffer, int max);
    static void SetObjetoSiempreVisible(int handlerObjeto, bool siempreVisible);
    static void SetObjetoBloqueante(int handlerObjeto, bool bloqueante);
    static void ActivarObjeto(int handlerObjeto);
    static void DesactivarObjeto(int handlerObjeto);
    static bool ColisionObjeto(int handlerObjeto);
    static void SetMascaraObjeto(int handlerObjeto, bool r, bool g, bool b, bool a);
    static float GetAlturaObjeto(int handlerObjeto);
    static float GetRadioObjeto(int handlerObjeto);
    static void SetEscalaObjeto(int handlerObjeto, float escala);
    static void SetTransparenteObjeto(int handlerObjeto, bool transparente);
    
    // Terreno
    static void CrearTerreno(string FicheroAlturas, bool multiTextura);
    static void DestruirTerreno();
    static void ActivarTerreno();
    static void ActualizarTerreno();
    static void DesactivarTerreno();
    static float GetAltura(int x, int z);
    static int  GetAnchoTerreno();
    static int  GetAltoTerreno();
    static float GetEscalaTerreno();
    static float GetAlturaSuave(float x, float z);
    static void SetInvierno(bool invierno);

    static Util::Vector3D GetCoordenadas2Dto3D(int coor);
    static Util::Vector2D GetCoordenadas3Dto2D(float x, float y, float z);

    
    // Sonidos 2D
    static int  CrearSonido(string ficheroSonido);
    static void DestruirSonido(int handlerSonido);
    static void ReproducirSonido(int handlerSonido);
    static void ReproducirLoopSonido(int handlerSonido);
    
    // Camara
    static void ResetCamara();
    static void EscalarCamara(float x, float y, float z);
    static void TrasladarCamara(float x, float y, float z);
    static void RotarCamara(float angulo, float x, float y, float z);
    static void GetPosicionCamara(float &x, float &y, float &z);
    
    // Api de camara antigua
    static void SetPosicionCamara(float x, float y, float z);
    static void SetInclinacionCamara(float angulo);
    
    // Raton
    static void MostrarRaton();
    static void OcultarRaton();
    static void SetImagenRaton(int HandlerImagen);
    
    // Iluminacion
    static void SetIluminacionDifusa(int nframes, float r, float g, float b, float a);
    static void SetIluminacionAmbiente(int nframes, float r, float g, float b, float a);

    // Pendientes de hacer private
    static void Blit(GLuint textura, int OrigenX, int OrigenY, int DestinoX, int DestinoY, int Ancho, int Alto);
    static void MaskedBlit(GLuint textura, int OrigenX, int OrigenY, int DestinoX, int DestinoY, int Ancho, int Alto);
    static void DrawTransSprite(GLuint textura, int OrigenX, int OrigenY, int DestinoX, int DestinoY, int Ancho, int Alto, int Alpha);
    static int  ReservarCallList();
    static int  ReservarCallListAlta();
    static void LiberarCallList(int callList);
    static void ResetClippingReg(int callList);
    static bool ComprobarClippingReg(int callList);
    static void CalcularClipping();
    static void CalcularCoordenadasVentana();
    static void PintarLuz();
};

}

#endif
