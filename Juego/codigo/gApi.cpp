#include "gApi.h"
#include "iConfiguracion.h"
#include "eExcepcionLocalizada.h"
#include "eExcepcionParametro.h"
#include "eExcepcionFichero.h"
#include <stdexcept>


namespace Graficos
{

BITMAP * Api::page[2] ;
int Api::pageActual = 0;
vector <Imagen *> Api::imagenes ;
vector <Segmento *> Api::segmentos ;
vector <Rectangulo *> Api::rectangulos ;
vector <BarraVida *> Api::barrasVida ;
vector <Sprite *> Api::sprites ;
vector <Boton *> Api::botones ;
vector <BarraProgreso *> Api::barrasProgreso ;
vector <Fuente *> Api::fuentes ;
vector <TextBox *> Api::textBoxs ;
vector <ChatBox *> Api::chatBoxs ;
vector <ListBox *> Api::listBoxs ;
vector <Modelo *> Api::modelos ;
vector <Objeto3D *> Api::objetos ;
vector <Sonido *> Api::sonidos ;

Terreno * Api::terreno = NULL ;

bool Api::mostrarRaton = false ;
int Api::HandlerImagenRaton = -1 ;

bool Api::callLists[Api::maxCallLists];
bool Api::clipping[Api::maxCallLists];

float Api::camaraX ;
float Api::camaraY ;
float Api::camaraZ ;
float Api::camaraIncX ;

float Api::diffuseLight0[4] ;
float Api::desDiffuseLight0[4] ;
float Api::velDiffuseLight0[4] ;

float Api::ambientLight0[4] ;
float Api::desAmbientLight0[4] ;
float Api::velAmbientLight0[4] ;

Util::FrecReal Api::frecuenciaClipping(1);

Util::Vector3D Api::coordenadasVentana[6] ;

float positionLight0[] = {300.0, 500.0, 400.0, 1.0} ;

//------------------------------------------------------------------------------
//! Inicializa el modulo gráfico. Recibe como parámetros el ancho y alto del
//! modo gráfico y trata de establecerlo con 32 bits de profundidad de color.
//! Crea la fuente por defecto, que se podrá utilizar pasando 0 como handler de
//! la fuente.
//! Si se produce algun error devuelve true. El resto de métodos de la Api no 
//! comprueban que se haya inicializado correctamente el modo gráfico, por lo
//! que cualquier llamada a algun método de la Api sin llamar antes a este
//! método (o si ha devuelto true) provocará un error de ejecución en la mayoría
//! de los casos
//------------------------------------------------------------------------------
bool
Api::Inicializar(int Ancho, int Alto)
{
    for ( int i = 0 ; i < maxCallLists ; i++ )
    {
        callLists[i] = false ;        
        clipping[i] = false ;
    }
    
	/* Suggest a good screen mode for OpenGL */
	allegro_gl_clear_settings();
	allegro_gl_set(AGL_COLOR_DEPTH, 32);
	allegro_gl_set(AGL_DOUBLEBUFFER, 1);
	allegro_gl_set(AGL_Z_DEPTH, 32);
	allegro_gl_set(AGL_WINDOWED, FALSE);
	allegro_gl_set(AGL_RENDERMETHOD, 1);
	allegro_gl_set(AGL_SUGGEST, AGL_COLOR_DEPTH | AGL_DOUBLEBUFFER
	       | AGL_RENDERMETHOD | AGL_Z_DEPTH | AGL_WINDOWED);

	/* Set the graphics mode */
	if (set_gfx_mode(GFX_OPENGL_FULLSCREEN, Ancho, Alto, 0, 0) )
    {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("Error estableciendo el modo grafico:\n%s\n", allegro_error);
        return true;
    }
    
    allegro_gl_use_mipmapping(TRUE);
    
    /* Setup OpenGL like we want */
	glEnable(GL_TEXTURE_2D);
	GLfloat la[] ={0.2, 0.2, 0.2, 0.2};
	glLightModelfv(GL_AMBIENT, la);

	/* Skip pixels which alpha channel is lower than 0.5*/
	glAlphaFunc(GL_GREATER, 0.5);

	glShadeModel(GL_SMOOTH);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	float fogColor[] = { 0.2,0.2,0.4,0.0 } ;
	
	glEnable(GL_FOG);
    glFogf(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, 75.0);
    glFogf(GL_FOG_END, 100.0);
    glFogfv(GL_FOG_COLOR, fogColor );
    
    glClearColor( fogColor[0], fogColor[1], fogColor[2], fogColor[3] );
    
    glDepthFunc(GL_LEQUAL);
	
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	float ambientGeneral[] = {0.3, 0.3, 0.3, 1};
	
	float ambientLight0[] = {0.2, 0.2, 0.2, 1};
	
	desDiffuseLight0[0] = diffuseLight0[0] = 0.8 ;
    desDiffuseLight0[1] = diffuseLight0[1] = 0.8 ;
    desDiffuseLight0[2] = diffuseLight0[2] = 0.8 ;
    desDiffuseLight0[3] = diffuseLight0[3] = 1.0 ;
    
	float specularLight0[] = {0,0,0,1} ;
	float zero[] = {0,0,0,0} ;
	
	glLightfv(GL_LIGHT0, GL_POSITION, positionLight0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight0);
	
	
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientGeneral);
	//glMaterialfv(GL_FRONT, GL_AMBIENT, ambientLight0);

	glViewport(0, 0, SCREEN_W, SCREEN_H);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glFrustum(-1.0, 1.0, -1.0, 1.0, 1, 60.0);
	gluPerspective(45.0, 1024.0/768.0, 1.0, 100.0);

	/* Set culling mode - not that we have anything to cull */
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	
	//glEnable(GL_DEPTH_TEST);
			
	glMatrixMode(GL_MODELVIEW);
        
    camaraX = 0.0 ;
    camaraY = 50.0 ;
    camaraZ = 0.0 ;
    camaraIncX = 0.0 ;
    
    // Creamos la fuente por defecto
    fuentes.push_back( new Fuente() );
    
    return false ;
}

//------------------------------------------------------------------------------
//! Libera toda la memoria utilizada por el modulo grafico. Destruye todos los
//! objetos que se hayan creado. Tras esta llamada se debe dejar de utilizar el
//! modulo gráfico por completo. No se asegura que tras esta llamada se pueda
//! volver a llamar a Inicializar para seguir utilizando el modulo.
//------------------------------------------------------------------------------
void
Api::Cerrar()
{
    // Liberamos las imagenes creadas
    for ( vector <Imagen *>::iterator i = imagenes.begin() ; i != imagenes.end() ; ++i )
    {
        if ( (*i) != NULL )
        {
            delete (*i) ;
            (*i) = NULL ;
        }
    }
    imagenes.clear();
    
    // Liberamos las fuentes creadas
    // TO DO ...
    
    // Liberamos las entidades 2D creadas
    Entidad2D::EliminarTodos();
}

//------------------------------------------------------------------------------
//--                                IMAGENES                                  --
//------------------------------------------------------------------------------
// La clase Imagen proporciona la funcionalidad de base de la mayoría del resto
// de objetos del modulo gráfico (en lo referido a 2D). Representa todo aquello
// que "puede" ser pintado en pantalla.
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//! Crea una nueva Imagen y devuelve un handler para utilizarla a partir del
//! nombre del fichero gráfico a cargar. Si no se pude crear el bitmap lanza una
//! ExcepcionParametro
//------------------------------------------------------------------------------
int
Api::CrearImagen(string NombreFichero)
{
    try
    {
        Imagen *imagen = new Imagen(NombreFichero) ;
        imagenes.push_back( imagen );
        for ( int i = 0 ; i < imagenes.size() ; i++ )
        {
            if ( imagenes.at(i) == imagen )
            {
                return i ;
            }
        }
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearImagen(string)");
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearImagen(string)");
    }
    catch ( Error::ExcepcionFichero &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::CrearImagen(string)", 1).Combinar(ex);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearImagen(string)").Combinar(ex);
    }
}
 
//------------------------------------------------------------------------------
//! Crea una nueva imagen y devuelve su handler. La imagen se crea de tal modo
//! que cotiene exactamente un texto escrito con una determinada fuente. Recibe
//! como parámetros el handler de la fuente con la que escribir el texto, y un
//! string con el texto a escribir. El texto se escribe con el color original de
//! la fuente, y con el fondo transparente, además la imagen se establece por
//! defecto como "masked", aunque esto puede ser desactivado posteriormente con
//! una llamada a SetImagenMasked()
//! Devuelve el handler de la imagen creada. Si el handler de fuente no es
//! valido o el string texto está vacio lanza un ExcepcionParametro.
//------------------------------------------------------------------------------
int
Api::CrearImagen(int HandlerFuente, string texto)
{
    try
    {
        if ( texto == "" )
        {
            throw Error::ExcepcionParametro("Graficos::Api::CrearImagen(int, string)", 2);
        }        
        Imagen *imagen = new Imagen( fuentes.at(HandlerFuente), texto ) ;
        imagenes.push_back( imagen );
        for ( int i = 0 ; i < imagenes.size() ; i++ )
        {
            if ( imagenes[i] == imagen )
            {
                return i ;
            }
        }
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearImagen(int, string)");
    }
    catch (out_of_range &ex)
    {
        throw Error::ExcepcionParametro("Graficos::Api::CrearImagen(int, string)", 1);
    }
    catch (Error::ExcepcionParametro &ex)
    {
        if ( ex.GetNumParametroExcepcion() == 1 )
        {
            throw Error::ExcepcionParametro("Graficos::Api::CrearImagen(int, string)", 1).Combinar(ex);
        }
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearImagen(int, string)").Combinar(ex);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearImagen(int, string)").Combinar(ex);
    }
}   

int
Api::CrearImagen(int handlerFuente, string texto, int r, int g, int b)
{
    if ( texto == "" )
    {
        throw Error::ExcepcionParametro("Graficos::Api::CrearImagen(int, string, int, int, int)", 2);
    }  
    Imagen *imagen = new Imagen(fuentes.at(handlerFuente), texto, r, g, b);
    imagenes.push_back(imagen);
    return imagenes.size()-1;
}

//------------------------------------------------------------------------------
//! Recibe el handler de una imagen y destruye la imagen correspondiente. Se debe
//! tener la precaución, si se destruye una imagen, de destruir todos los objetos
//! entidades 2D que se hayan creado a partir de ella (sprites, botones, etc) ya 
//! que generalmente no se crean una copia de la imagen sino que utilizan la 
//! misma imagen para pintar en pantalla.
//! Si el handlerImagen no es valido lanza una ExcepcionParametro
//------------------------------------------------------------------------------
void
Api::DestruirImagen(int HandlerImagen)
{
    try
    {
        if ( imagenes.at(HandlerImagen) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::DestruirImagen(int)", 1) ;
        }    
        delete imagenes.at(HandlerImagen);
        imagenes.at(HandlerImagen) = NULL ;
    }
    catch (out_of_range &ex)
    {
        throw Error::ExcepcionParametro("Graficos::Api::DestruirImagen(int)", 1) ;
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::DestruirImagen(int)").Combinar(ex) ;
    }        
}

//------------------------------------------------------------------------------
//! Permite indicar si una imagen es "masked" o no. Una imagen "masked" se
//! pintará en pantalla, a excepción de aquellos puntos de la imagen que sean
//! rosa puro (255,0,255) que se considerarán transparentes. La función recibe
//! como parametros el handler de la imagen y el valor que se le dará a la
//! propiedad "masked" de la imagen.
//! Si el handler de la imagen no es valido lanza una ExcepcionParametro
//------------------------------------------------------------------------------
void
Api::SetImagenMasked(int HandlerImagen, bool Masked)
{
    try
    {
        if ( imagenes.at(HandlerImagen) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::SetImagenMasked(int, bool)", 1);
        }
        imagenes.at(HandlerImagen)->SetMasked(Masked) ;
    }
    catch (out_of_range &ex)
    {
        throw Error::ExcepcionParametro("Graficos::Api::SetImagenMasked(int, bool)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::SetImagenMasked(int, bool)").Combinar(ex);
    }        
}

//Pinta un pixel en la posicion indicada y del color especificado en una imagen
void
Api::PintarPixel(int HandlerImagen,int x, int y, char r, char g, char b)
{
   imagenes.at(HandlerImagen)->PintarPixel(x,y,r,g,b);
}

//Aplica Mascara transparencia
void
Api::AplicarMascara(int HandlerImagen)
{
    imagenes.at(HandlerImagen)->AplicarMascara();
}




//------------------------------------------------------------------------------
//--                               FUENTES                                    --
//------------------------------------------------------------------------------
// La api gráfica proporciona métodos para crear y destruir fuentes. La forma de
// trabajar con fuentes es mediante el handler que devuelve en la creación.
// Existe creada una fuente que abstrae a la FONT *font (la fuente por defecto
// de allegro). Esta fuente no puede ser creada ni destruida, para utilizarla se
// debe pasar el 0 como handler de fuente. 
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//! Crea una nueva fuente a partir del nombre de fichero en el que está
//! almacenada. Devuelve el handler de la fuente creada para su utilización en
//! otras llamadas a la Api.
//! En caso de error al cargar la fuente lanza una ExcepcionFichero
//------------------------------------------------------------------------------
int
Api::CrearFuente(string NombreFichero)
{
    try
    {
        Fuente *fuente = new Fuente( NombreFichero ) ;
        fuentes.push_back( fuente );
        for ( int i = 0 ; i < fuentes.size() ; i++ )
        {
            if ( fuentes[i] == fuente )
            {
                return i ;
            }
        }
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearFuente(string)");
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearFuente(string)");
    }
    catch ( Error::ExcepcionFichero &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::CrearFuente(string)", 1).Combinar(ex);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearFuente(string)").Combinar(ex);
    }
}

//------------------------------------------------------------------------------
//! Destruye la fuente cuyo handler se pasa por parámetro. La fuente por defecto
//! (handler = 0) no puede ser destruida
//! Si el handler pasado es invalido se lanza una ExcepcionParametro
//------------------------------------------------------------------------------
void
Api::DestruirFuente(int HandlerFuente)
{
    try
    {
        if ( HandlerFuente != 0 )
        {
            delete sprites.at(HandlerFuente) ;
            sprites.at(HandlerFuente) = NULL ;
        }
        else
        {
            throw Error::ExcepcionParametro("Graficos::Api::DestruirFuente(int)", 1);
        }
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::DestruirFuente(int)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::DestruirFuente(int)").Combinar(ex);
    }
}

int
Api::CrearSegmento(float r, float g, float b, float a, int x, int y, int finX, int finY, int z)
{
    try
    {
        Segmento *segmento = new Segmento (r, g, b, a, x, y, finX, finY, z);
        segmentos.push_back(segmento);
        return segmentos.size()-1 ;
    } 
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::CrearSegmento(float, float, float, float, int, int, int, int, int)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearSegmento(float, float, float, float, int, int, int, int, int)").Combinar(ex);
    } 
}

void
Api::DestruirSegmento(int handlerSegmento)
{
    try
    {
        if ( segmentos.at(handlerSegmento) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::DestruirSegmento(int)", 1);
        }
        delete segmentos.at(handlerSegmento);
        segmentos.at(handlerSegmento) = NULL ;
    }
    catch (out_of_range &ex)
    {
        throw Error::ExcepcionParametro("Graficos::Api::DestruirSegmento(int)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::DestruirSegmento(int)").Combinar(ex);
    }
}  

void
Api::ActivarSegmento(int handlerSegmento)
{
    segmentos.at(handlerSegmento)->SetActivo(true);
}

void
Api::DesactivarSegmento(int handlerSegmento)
{
    segmentos.at(handlerSegmento)->SetActivo(false);
}

void
Api::MoverSegmento(int handlerSegmento, int x, int y, int finX, int finY)
{
    segmentos.at(handlerSegmento)->SetPosicion(x,y, segmentos.at(handlerSegmento)->GetZ());
    segmentos.at(handlerSegmento)->SetFinalSegmento(finX,finY);
}

void
Api::SetClippingSegmento(int handlerSegmento, int x0, int y0, int x1, int y1)
{
    segmentos.at(handlerSegmento)->SetClipping(x0, y0, x1, y1);
}

int
Api::CrearRectangulo(int ancho, int alto, float r, float g, float b, float a, bool relleno, int x, int y, int z)
{
    try
    {
        Rectangulo *rectangulo = new Rectangulo (ancho, alto, r, g, b, a, relleno, x, y, z);
        rectangulos.push_back( rectangulo );
        return rectangulos.size()-1 ;
    } 
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::CrearRectangulo(int, int, float, float, float, float, bool, int, int, int)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearRectangulo(int, int, float, float, float, float, bool, int, int, int)").Combinar(ex);
    } 
}

void
Api::DestruirRectangulo(int handlerRectangulo)
{
    try
    {
        if ( rectangulos.at(handlerRectangulo) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::DestruirRectangulo(int)", 1);
        }
        delete rectangulos.at(handlerRectangulo);
        rectangulos.at(handlerRectangulo) = NULL ;
    }
    catch (out_of_range &ex)
    {
        throw Error::ExcepcionParametro("Graficos::Api::DestruirRectangulo(int)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::DestruirRectangulo(int)").Combinar(ex);
    }
}  

void
Api::ActivarRectangulo(int handlerRectangulo)
{
    rectangulos.at(handlerRectangulo)->SetActivo(true);
}

void
Api::DesactivarRectangulo(int handlerRectangulo)
{
    rectangulos.at(handlerRectangulo)->SetActivo(false);
}

void
Api::MoverRectangulo(int handlerRectangulo, int x, int y)
{
    rectangulos.at(handlerRectangulo)->SetPosicion(x,y, rectangulos.at(handlerRectangulo)->GetZ());
}

void
Api::CambiarTamanyoRectangulo(int handlerRectangulo, int ancho, int alto)
{
    rectangulos.at(handlerRectangulo)->SetAncho(ancho);
    rectangulos.at(handlerRectangulo)->SetAlto(alto);    
}

void
Api::SetColorRectangulo(int handlerRectangulo, float r, float g, float b, float a)
{
    rectangulos.at(handlerRectangulo)->SetColor(r, g, b, a);
}

int
Api::CrearBarraVida(int ancho, int alto, int borde, int X, int Y, int Z)
{
    BarraVida *barraVida = new BarraVida(ancho, alto, borde, X, Y, Z);
    barrasVida.push_back(barraVida);
    return barrasVida.size()-1 ;
}

void
Api::DestruirBarraVida(int handlerBarraVida)
{
    try
    {
        if ( barrasVida.at(handlerBarraVida) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::DestruirBarraVida(int)", 1);
        }
        delete barrasVida.at(handlerBarraVida);
        barrasVida.at(handlerBarraVida) = NULL ;
    }
    catch (out_of_range &ex)
    {
        throw Error::ExcepcionParametro("Graficos::Api::DestruirBarraVida(int)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::DestruirBarraVida(int)").Combinar(ex);
    }
}

void
Api::ActivarBarraVida(int handlerBarraVida)
{
    barrasVida.at(handlerBarraVida)->SetActivo(true);
}

void
Api::DesactivarBarraVida(int handlerBarraVida)
{
    barrasVida.at(handlerBarraVida)->SetActivo(false);
}

void
Api::MoverBarraVida(int handlerBarraVida, int x, int y)
{
    barrasVida.at(handlerBarraVida)->SetPosicion(x, y, barrasVida.at(handlerBarraVida)->GetZ());
}

void
Api::CentrarBarraVida(int handlerBarraVida, int x, int y)
{
    barrasVida.at(handlerBarraVida)->SetPosicion(x-barrasVida[handlerBarraVida]->GetAncho()/2, y-barrasVida[handlerBarraVida]->GetAlto()/2, barrasVida.at(handlerBarraVida)->GetZ());    
}

void
Api::SetPorcentajeBarraVida(int handlerBarraVida, float porcentaje)
{
    barrasVida.at(handlerBarraVida)->SetPorcentaje(porcentaje);
}

//------------------------------------------------------------------------------
//--                                  SPRITES                                 -- 
//------------------------------------------------------------------------------
// En contra del concepto de Imagen, que es aquello que "puede" ser pintado en
// pantalla, un sprite es una entidad que es pintada en la pantalla. Un sprite
// se compone de una imagen (que se pinta en pantalla) y de tres coordenadas
// (x, y, z) que indican donde se pinta. Las coordenadas x, y indican las
// coordenadas de pantalla en las que se pinta la imagen, mientras que la
// coordenada z se utiliza para calcular que objetos se pintan encima de otros.
// Todos los objetos 2D que se pintan en pantalla son pintados desde la z mas
// pequeña hasta la z mayor.
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//! Crea un nuevo sprite a partir de un handler de unidad y las coordenadas x,
//! y, z.
//! En caso de proporcionar un handler de imagen invalido lanza una 
//! ExcepcionParametro
//------------------------------------------------------------------------------
int
Api::CrearSprite(int HandlerImagen, int X, int Y, int Z)
{
    try
    {
        Sprite * sprite = new Sprite( imagenes.at(HandlerImagen), X, Y, Z ) ;
        sprites.push_back( sprite );
        for ( int i = 0 ; i < sprites.size() ; i++ )
        {
            if ( sprites[i] == sprite )
            {
                return i ;
            }
        }
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearSprite(int, int, int, int)");
    } 
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::CrearSprite(int, int, int, int)", 1);
    }
    catch ( Error::ExcepcionParametro &ex )
    {
        if ( ex.GetNumParametroExcepcion() == 1 )
        {
            throw Error::ExcepcionParametro("Graficos::Api::CrearSprite(int, int, int, int)", 1).Combinar(ex) ;
        }
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearSprite(int, int, int, int)").Combinar(ex);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearSprite(int, int, int, int)").Combinar(ex);
    }
}

//------------------------------------------------------------------------------
//! Destruye un sprite a partir del handler del sprite
//! En caso de que el handler no sea un handler de sprite valido, se lanza una
//! ExcepcionParametro
//------------------------------------------------------------------------------
void
Api::DestruirSprite(int HandlerSprite)
{
    try
    {
        if ( sprites.at(HandlerSprite) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::DestruirSprite(int)", 1);
        }
        delete sprites.at(HandlerSprite);
        sprites.at(HandlerSprite) = NULL ;
    }
    catch (out_of_range &ex)
    {
        throw Error::ExcepcionParametro("Graficos::Api::DestruirSprite(int)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::DestruirSprite(int)").Combinar(ex);
    }
}

//------------------------------------------------------------------------------
//! Activa un sprite a partir del handler del sprite, lo que provoca que el 
//! sprite se pinte en pantalla. Por defecto los sprites estan activados cuando
//! se crean.
//! En caso de que el handler de sprite sea invalido se lanza una 
//! ExcepcionParametro
//------------------------------------------------------------------------------
void
Api::ActivarSprite(int HandlerSprite)
{
    try
    {
        if ( sprites.at(HandlerSprite) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::ActivarSprite(int)", 1);
        }
        sprites.at(HandlerSprite)->SetActivo(true);
    }
    catch (out_of_range &ex)
    {
        throw Error::ExcepcionParametro("Graficos::Api::ActivarSprite(int)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::ActivarSprite(int)").Combinar(ex);
    }
}

//------------------------------------------------------------------------------
//! Desactiva un sprite a partir de su handler. Esto provoca que el sprite deje
//! de ser pintado en la pantalla, pero no destruido. El sprite puede volverse
//! a activar en cualquier momento, conservando sus propiedades. Se pueden modi-
//! ficar las propiedades de un sprite desactivado, pero por supuesto no seran
//! apreciables hasta que el sprite se active
//! Si se pasa un handler invalido se lanza una ExcepcionParametro
//------------------------------------------------------------------------------
void
Api::DesactivarSprite(int HandlerSprite)
{
    try
    {
        if ( sprites.at(HandlerSprite) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::DesactivarSprite(int)", 1);
        }        
        sprites.at(HandlerSprite)->SetActivo(false);
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::DesactivarSprite(int)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::DesactivarSprite(int)").Combinar(ex);
    }
}

//------------------------------------------------------------------------------
//! Cambia la posicion de un sprite a partir del handler del sprite y las nuevas
//! coordenadas x, y en las que se desea mostrar
//! En caso de que el handler sea invalido se lanza una ExcepcionParametro
//------------------------------------------------------------------------------
void
Api::MoverSprite(int HandlerSprite, int x, int y)
{
    try
    {
        if ( sprites.at(HandlerSprite) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::MoverSprite(int, int, int)", 1);
        }        
        sprites.at(HandlerSprite)->SetPosicion(x, y, sprites.at(HandlerSprite)->GetZ() );
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::MoverSprite(int, int, int)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::MoverSprite(int, int, int)").Combinar(ex);
    }
}

int
Api::GetAnchoSprite(int handlerSprite)
{
    return sprites.at(handlerSprite)->GetAncho() ;
}

int
Api::GetAltoSprite(int handlerSprite)
{
    return sprites.at(handlerSprite)->GetAlto() ;
}

void
Api::SetAlphaSprite(int handlerSprite, int alpha)
{
    sprites.at(handlerSprite)->SetAlpha(alpha);
}

//------------------------------------------------------------------------------
//--                               BOTONES                                    --
//------------------------------------------------------------------------------
// El modulo gráfico dispone de la clase Boton para proporcionar la logica de
// botones y construirlos a partir de unos cuantos parámetros. Estos botones 
// se construyen suministrando la imagen del boton, la imagen de selección, una
// fuente de texto y un texto. El botón interactua automáticamente con el raton
// pintando la imagen de selección al 50% de opacidad si el ratón pasa por
// encima, y al 75% si se clicka el boton. Además provee de un efecto de
// fade-off para el nivel de opacidad de la imagen de selección.
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//! Crea un nuevo botón y devuelve un handler para su manipulación. Toma como 
//! parametros el handler de la imagen del boton, el handler de la imagen de 
//! selección, el handler de la fuente con la que se escribirá el texto del 
//! boton, y un string con el texto del boton.
//! Además toma los parámetros típicos x, y, z que comparten todas las entidades
//! 2D que se dibujan en pantalla.
//! Si no se proporciona un handler valido se lanza una ExcepcionParametro. El 
//! handler de la imagen de selección puede ser -1 lo que indica que el boton no
//! tendrá imagen de seleccion.
//------------------------------------------------------------------------------
int
Api::CrearBoton(int HandlerImagenFondo, int HandlerImagenSeleccion, int HandlerFuente, string texto, int X, int Y, int Z)
{
    try
    {
        Boton *boton = new Boton( imagenes.at( HandlerImagenFondo ), HandlerImagenSeleccion==-1?NULL:imagenes.at( HandlerImagenSeleccion ), fuentes.at(HandlerFuente), texto, X, Y, Z);
        botones.push_back( boton );
        for ( int i = 0 ; i < botones.size() ; i++ )
        {
            if ( botones[i] == boton )
            {
                return i ;
            }
        }
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearBoton(int, int, int, string, int, int, int)");
    }
    catch ( out_of_range &ex )
    {
        if ( HandlerImagenFondo < 0 || HandlerImagenFondo >= imagenes.size() )
        {
            throw Error::ExcepcionParametro("Graficos::Api::CrearBoton(int, int, int, string, int, int, int)", 1);
        }
        if ( HandlerImagenSeleccion < -1 || HandlerImagenSeleccion >= imagenes.size() )
        {
            throw Error::ExcepcionParametro("Graficos::Api::CrearBoton(int, int, int, string, int, int, int)", 2);
        }
        if ( HandlerFuente < 0 || HandlerFuente >= fuentes.size() )
        {
            throw Error::ExcepcionParametro("Graficos::Api::CrearBoton(int, int, int, string, int, int, int)", 3);
        }
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearBoton(int, int, int, string, int, int, int)");
    }
    catch ( Error::ExcepcionParametro &ex )
    {
        switch (ex.GetNumParametroExcepcion())
        {
            case 1:
                throw Error::ExcepcionParametro("Graficos::Api::CrearBoton(int, int, int, string, int, int, int)", 1).Combinar(ex);
            case 3:
                throw Error::ExcepcionParametro("Graficos::Api::CrearBoton(int, int, int, string, int, int, int)", 3).Combinar(ex);
        }
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearBoton(int, int, int, string, int, int, int)").Combinar(ex);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearBoton(int, int, int, string, int, int, int)").Combinar(ex);
    }
}

//------------------------------------------------------------------------------
//! Destruye un boton a partir de su handler
//! Si el handler no es valido lanza una ExcepcionParametro
//------------------------------------------------------------------------------
void
Api::DestruirBoton(int HandlerBoton)
{
    try
    {
        if ( botones.at(HandlerBoton) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::DestruirBoton(int)", 1);
        }
        delete botones.at(HandlerBoton);
        botones.at(HandlerBoton) = NULL ;
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::DestruirBoton(int)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::DestruirBoton(int)").Combinar(ex);
    }
}

//------------------------------------------------------------------------------
//! Activa un boton a partir de su handler de boton
//! Si el handler no es valid lanza una ExcepcionParametro
//------------------------------------------------------------------------------
void
Api::ActivarBoton(int HandlerBoton)
{
    try
    {
        if ( botones.at(HandlerBoton) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::ActivarBoton(int)", 1);
        }
        botones.at(HandlerBoton)->SetActivo(true);
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::ActivarBoton(int)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::ActivarBoton(int)").Combinar(ex);
    }
}

//------------------------------------------------------------------------------
//! Desactiva un boton a partir de su handler de boton
//! Si el handler no es valid lanza una ExcepcionParametro
//------------------------------------------------------------------------------
void
Api::DesactivarBoton(int HandlerBoton)
{
    try
    {
        if ( botones.at(HandlerBoton) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::DesactivarBoton(int)", 1);
        }
        botones.at(HandlerBoton)->SetActivo(false);
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::DesactivarBoton(int)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::DesactivarBoton(int)").Combinar(ex);
    }
}

//------------------------------------------------------------------------------
//! Recibe un puntero a bool que el boton pondrá a true cuando se genere un
//! evento de click sobre el boton.
//! Si el handler de boton no es valido se lanza una ExcepcionParametro
//! Si OnClickFlag vale NULL se desactiva la notificacion de ClickBoton
//------------------------------------------------------------------------------
void
Api::OnClickBoton(int HandlerBoton, bool *OnClickFlag)
{
    try
    {
        if ( botones.at(HandlerBoton) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::OnClickBoton(int, bool *)", 1);
        }
        botones.at(HandlerBoton)->OnClick(OnClickFlag);
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::OnClickBoton(int, bool *)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::OnClickBoton(int, bool *)").Combinar(ex);
    }
}

void
Api::OnMouseOver(int handlerBoton, bool *flag)
{
    botones.at(handlerBoton)->OnMouseOver(flag);
}

//------------------------------------------------------------------------------
//! Cambia la imagen de fondo del boton. Recibe para ello el handler del boton
//! y el handler de la nueva imagen de fondo
//! Si algun handler es invalido se lanza una ExcepcionParametro
//------------------------------------------------------------------------------
void
Api::CambiarFondoBoton(int HandlerBoton, int HandlerImagen)
{
    try
    {
        if ( botones.at(HandlerBoton) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::CambiarFondoBoton(int, int)", 1);
        }
        botones.at(HandlerBoton)->CambiarImagenFondo( imagenes.at(HandlerImagen) );
    }
    catch ( out_of_range &ex )
    {
        if ( HandlerBoton < 0 || HandlerBoton >= botones.size() )
        {
            throw Error::ExcepcionParametro("Graficos::Api::CambiarFondoBoton(int, int)", 1);
        }
        if ( HandlerImagen < 0 || HandlerImagen >= imagenes.size() )
        {
            throw Error::ExcepcionParametro("Graficos::Api::CambiarFondoBoton(int, int)", 2);
        }
    }
    catch ( Error::ExcepcionParametro &ex )
    {
        if ( ex.GetNumParametroExcepcion() == 1 )
        {
            throw Error::ExcepcionParametro("Graficos::Api::CambiarFondoBoton(int, int)", 2).Combinar(ex);
        }
        throw Error::ExcepcionLocalizada("Graficos::Api::CambiarFondoBoton(int, int)").Combinar(ex);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::CambiarFondoBoton(int, int)").Combinar(ex);   
    }
}

void
Api::MoverBoton(int handlerBoton, int x, int y)
{
    botones.at(handlerBoton)->SetPosicion(x, y, botones.at(handlerBoton)->GetZ());    
}

void
Api::SetFijoBoton(int handlerBoton, bool fijo)
{
    botones.at(handlerBoton)->SetFijo(fijo);
}

bool
Api::GetFijoBoton(int handlerBoton)
{
    return botones.at(handlerBoton)->GetFijo();
}

int
Api::CrearBarraProgreso(int handlerImagenFondo, int handlerImagenProgreso, int bordeLateral, int separacion, int X, int Y, int Z)
{
    //try
    //{
        BarraProgreso *barra = new BarraProgreso(imagenes.at(handlerImagenFondo), imagenes.at(handlerImagenProgreso), bordeLateral, separacion, X, Y, Z);
        barrasProgreso.push_back( barra );
        for ( int i = 0 ; i < barrasProgreso.size() ; i++ )
        {
            if ( barrasProgreso[i] == barra )
            {
                return i ;
            }
        }
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearBarraProgreso(int, int, int, int, int, int, int)");
    //}
}

void
Api::DestruirBarraProgreso(int handlerBarraProgreso)
{
    try
    {
        if ( barrasProgreso.at(handlerBarraProgreso) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::DestruirBarraProgreso(int)", 1);
        }
        delete barrasProgreso.at(handlerBarraProgreso);
        barrasProgreso.at(handlerBarraProgreso) = NULL ;
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::DestruirBarraProgreso(int)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::DestruirBarraProgreso(int)").Combinar(ex);
    }
}    

void
Api::ActivarBarraProgreso(int handlerBarraProgreso)
{
    try
    {
        if ( barrasProgreso.at(handlerBarraProgreso) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::ActivarBarraProgreso(int)", 1);
        }
        barrasProgreso.at(handlerBarraProgreso)->SetActivo(true);
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::ActivarBarraProgreso(int)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::ActivarBarraProgreso(int)").Combinar(ex);
    }
}    

void
Api::DesactivarBarraProgreso(int handlerBarraProgreso)
{
    try
    {
        if ( barrasProgreso.at(handlerBarraProgreso) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::DesactivarBarraProgreso(int)", 1);
        }
        barrasProgreso.at(handlerBarraProgreso)->SetActivo(false);
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::DesactivarBarraProgreso(int)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::DesactivarBarraProgreso(int)").Combinar(ex);
    }
}   
    
void
Api::SetPorcentajeBarraProgreso(int handlerBarraProgreso, int porcentaje)
{
    try
    {
        if ( barrasProgreso.at(handlerBarraProgreso) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::SetPorcentajeBarraProgreso(int, int)", 1);
        }
        barrasProgreso.at(handlerBarraProgreso)->SetPorcentaje(porcentaje);
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::SetPorcentajeBarraProgreso(int, int)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::SetPorcentajeBarraProgreso(int, int)").Combinar(ex);
    }
}     


int
Api::CrearTextBox(int HandlerImagenFondo, int HandlerFuente, string texto, int X, int Y, int Z)
{
    try
    {
        TextBox *text = new TextBox( imagenes.at(HandlerImagenFondo), fuentes.at(HandlerFuente), texto, X, Y, Z);
        textBoxs.push_back( text );
        for ( int i = 0 ; i < textBoxs.size() ; i++ )
        {
            if ( textBoxs[i] == text )
            {
                return i ;
            }
        }
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearTextBox(int, int, string, int, int, int)");
    }
    catch ( out_of_range &ex )
    {
        if ( HandlerImagenFondo < 0 || HandlerImagenFondo >= imagenes.size() )
        {
            throw Error::ExcepcionParametro("Graficos::Api::CrearTextBox(int, int, string, int, int, int)", 1);
        }
        if ( HandlerFuente < 0 || HandlerFuente >= fuentes.size() )
        {
            throw Error::ExcepcionParametro("Graficos::Api::CrearTextBox(int, int, string, int, int, int)", 2);
        }
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearTextBox(int, int, string, int, int, int)");
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearTextBox(int, int, string, int, int, int)").Combinar(ex);
    }
}    

void
Api::DestruirTextBox(int HandlerTextBox)
{
    try
    {
        if ( textBoxs.at(HandlerTextBox) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::DestruirTextBox(int)", 1);
        }
        delete textBoxs.at(HandlerTextBox);
        textBoxs.at(HandlerTextBox) = NULL ;
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::DestruirTextBox(int)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::DestruirTextBox(int)").Combinar(ex);
    }
}

void
Api::ActivarTextBox(int HandlerTextBox)
{
    try
    {
        if ( textBoxs.at(HandlerTextBox) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::ActivarTextBox(int)", 1);
        }
        textBoxs.at(HandlerTextBox)->SetActivo(true);
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::ActivarTextBox(int)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::ActivarTextBox(int)").Combinar(ex);
    }
}

void
Api::DesactivarTextBox(int HandlerTextBox)
{
    try
    {
        if ( textBoxs.at(HandlerTextBox) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::DesactivarTextBox(int)", 1);
        }
        textBoxs.at(HandlerTextBox)->SetActivo(false);
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::DesactivarTextBox(int)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::DesactivarTextBox(int)").Combinar(ex);
    }
}

void
Api::SetTextoTextBox(int handlerTextBox, string texto)
{
    textBoxs.at(handlerTextBox)->SetTexto(texto);
}

string
Api::GetTextoTextBox(int HandlerTextBox)
{
    try
    {
        if ( textBoxs.at(HandlerTextBox) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::GetTextoTextBox(int)", 1);
        }
        return textBoxs.at(HandlerTextBox)->GetTexto();
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::GetTextoTextBox(int)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::GetTextoTextBox(int)").Combinar(ex);
    }
}

void
Api::OnEnterTextBox(int handlerTextBox, bool *flag)
{
    textBoxs.at(handlerTextBox)->OnEnter(flag);
}

int
Api::CrearChatBox(int handlerImagenFondo, int handlerFuente, int ancho, int alto, int borde, int x, int y, int z)
{
    ChatBox *cb = new ChatBox(
        handlerImagenFondo==-1?NULL:imagenes.at(handlerImagenFondo),
        fuentes.at(handlerFuente),
        ancho, alto, borde,
        x, y, z);
    chatBoxs.push_back( cb );
    return chatBoxs.size()-1 ;
}

void
Api::DestruirChatBox(int handlerChatBox)
{
    delete chatBoxs.at(handlerChatBox);
    chatBoxs.at(handlerChatBox) = NULL ;
}

void
Api::ActivarChatBox(int handlerChatBox)
{
    chatBoxs.at(handlerChatBox)->SetActivo(true);
}

void
Api::DesactivarChatBox(int handlerChatBox)
{
    chatBoxs.at(handlerChatBox)->SetActivo(false);
}

void
Api::AddLineaChatBox(int handlerChatBox, string texto, int r, int g, int b)
{
    chatBoxs.at(handlerChatBox)->AddLinea(texto, r, g, b);
}

int
Api::CrearListBox(int HandlerImagenFondo, int HandlerImagenSeleccion, int HandlerImagenResaltado, 
                    int HandlerImagenScroll, int HandlerImagenSeleccionScroll, int HandlerFuente, 
                    int LeftClient, int TopClient, int BottomClient, 
                    int LeftScroll, int TopScroll, int BottomScroll, int X, int Y, int Z)
{
    try
    {
        ListBox *list = new ListBox( 
            imagenes.at(HandlerImagenFondo),
            imagenes.at(HandlerImagenSeleccion),
            imagenes.at(HandlerImagenResaltado),
            NULL,
            NULL,
            fuentes.at(HandlerFuente),
            LeftClient, TopClient, BottomClient,
            LeftScroll, TopScroll, BottomScroll,
            X, Y, Z);
    
        listBoxs.push_back( list );
        for ( int i = 0 ; i < listBoxs.size() ; i++ )
        {
            if ( listBoxs[i] == list )
            {
                return i ;
            }
        }
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearListBox(15 int)");
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearListBox(15 int)");
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearListBox(15 int)").Combinar(ex);
    }
}

void
Api::DestruirListBox(int HandlerListBox)
{
    try
    {
        if ( listBoxs.at(HandlerListBox) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::DestruirListBox(int)", 1);
        }    
        delete listBoxs.at(HandlerListBox) ;
        listBoxs.at(HandlerListBox) = NULL ;
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::DestruirListBox(int)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::DestruirListBox(int)").Combinar(ex);
    }
}

void
Api::AddLineaListBox(int HandlerListBox, string Texto)
{
    try
    {
        if ( listBoxs.at(HandlerListBox) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::AddLineaListBox(int, string)", 1);
        }
        listBoxs.at(HandlerListBox)->AddLinea(Texto);
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::AddLineaListBox(int, string)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::AddLineaListBox(int, string)").Combinar(ex);
    }
}

void
Api::OnCambioListBox(int HandlerListBox, bool *Cambio, int *Seleccionado)
{
    try
    {
        if ( listBoxs.at(HandlerListBox) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::OnCambioListBox(int, bool *, int *)", 1);
        }    
        listBoxs.at(HandlerListBox)->OnCambio(Cambio, Seleccionado);
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::OnCambioListBox(int, bool *, int *)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::OnCambioListBox(int, bool *, int *)").Combinar(ex);
    }
}

void
Api::ActivarListBox(int HandlerListBox)
{
    try
    {
        if ( listBoxs.at(HandlerListBox) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::ActivarListBox(int)", 1);
        }    
        listBoxs.at(HandlerListBox)->SetActivo(true);
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::ActivarListBox(int)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::ActivarListBox(int)").Combinar(ex);
    }
}

void
Api::DesactivarListBox(int HandlerListBox)
{
    try
    {
        if ( listBoxs.at(HandlerListBox) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::DesactivarListBox(int)", 1);
        }    
        listBoxs.at(HandlerListBox)->SetActivo(false);
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::DesactivarListBox(int)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::DesactivarListBox(int)").Combinar(ex);
    }
}

int
Api::CrearModelo(string nombreFichero)
{
    try
    {
        Modelo *modelo = new Modelo( nombreFichero );
        modelos.push_back(modelo);
        for ( int i = 0 ; i < modelos.size() ; i++ )
        {
            if ( modelos.at(i) == modelo )
            {
                return i ;
            }
        }
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearModelo(string)");
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearModelo(string)");
    }
    catch ( Error::ExcepcionFichero &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::CrearModelo(string)", 1).Combinar(ex);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearModelo(string)").Combinar(ex);
    }
}

int
Api::CrearModelo(string nombreFichero, int imagenTextura, float r, float g, float b)
{
    Modelo *modelo = new Modelo( nombreFichero, imagenTextura!=-1?imagenes.at(imagenTextura):NULL , r, g, b );
    modelos.push_back(modelo);
    return modelos.size()-1;
}

int
Api::CrearModeloQuad(int handlerImagenTextura, float r, float g, float b, float a)
{
    try
    {
        Modelo *modelo = Modelo::CrearQuad(imagenes.at(handlerImagenTextura), r, g, b, a);
        modelos.push_back(modelo);
        for ( int i = 0 ; i < modelos.size() ; i++ )
        {
            if ( modelos[i] == modelo )
            {
                return i ;
            }
        }
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearModeloQuad(int, float, float, float, float)");
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearModeloQuad(int, float, float, float, float)");
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearModeloQuad(int, float, float, float, float)").Combinar(ex);
    }
}    

void
Api::DestruirModelo(int HandlerModelo)
{
    try
    {
        if ( modelos.at(HandlerModelo) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::DestruirModelo(int)", 1);
        }
        delete modelos.at(HandlerModelo);
        modelos.at(HandlerModelo) = NULL ;
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::DestruirModelo(int)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::DestruirModelo(int)").Combinar(ex);
    }
}

float
Api::GetAlturaModelo(int handlerModelo)
{
    return modelos.at(handlerModelo)->GetAltura();
}

int
Api::CrearObjeto(int HandlerModelo)
{
    try
    {
        Objeto3D *objeto = new Objeto3D( modelos.at(HandlerModelo), 0);
        objetos.push_back(objeto);
        objetos.back()->SetGlNombre( objetos.size()-1 );
        return objetos.size()-1 ;
        /*
        for ( int i = 0 ; i < objetos.size() ; i++ )
        {
            if ( objetos.at(i) == objeto )
            {
                objetos.at(i)->SetGlNombre(i);
                return i ;
            }
        }
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearObjeto(int)");
        */
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearObjeto(int)");
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearObjeto(int)").Combinar(ex);
    }
}

void
Api::DestruirObjeto(int HandlerObjeto)
{
    try
    {
        if ( objetos.at(HandlerObjeto) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::DestruirObjeto(int)", 1) ;
        }
        delete objetos.at(HandlerObjeto);
        objetos.at(HandlerObjeto) = NULL ;
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::DestruirObjeto(int)", 1) ;
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::DestruirObjeto(int)").Combinar(ex) ;
    }
}

void
Api::MoverObjeto(int HandlerObjeto, float x, float y, float z)
{
    try
    {
        if ( objetos.at(HandlerObjeto) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::MoverObjeto(int, float, float, float)", 1) ;
        }    
        objetos.at(HandlerObjeto)->SetPosicion(x, y, z);
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::MoverObjeto(int, float, float, float)", 1) ;
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::MoverObjeto(int, float, float, float)").Combinar(ex) ;
    }
}

void
Api::SetAnguloObjeto(int HandlerObjeto, float angulo)
{
    try
    {
        if ( objetos.at(HandlerObjeto) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::SetAnguloObjeto(int, float)", 1) ;
        }      
        objetos.at(HandlerObjeto)->SetAngulo(angulo);
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::SetAnguloObjeto(int, float)", 1) ;
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::SetAnguloObjeto(int, float)").Combinar(ex) ;
    }        
}

int
Api::GetObjetosEn(int x, int y, int *nbuffer, int max)
{
    try
    {
        GLuint buffer[512];
        GLint hits ;
        int seleccion = -1 ;
        
        GLint viewport[4] ;
        glGetIntegerv(GL_VIEWPORT, viewport);
        glSelectBuffer(512, buffer);
        
        glRenderMode(GL_SELECT);
        glInitNames();
        glPushName(0);
        
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        
        glLoadIdentity();
        gluPickMatrix(x, viewport[3]-y, 1.0f, 1.0f, viewport);
        gluPerspective(45.0f, ((GLfloat) viewport[2]-viewport[0])/(viewport[3]-viewport[1]), 1.0f, 600.0f);
        
        glMatrixMode(GL_MODELVIEW);
        
        Entidad3D::PintarTodosBoundingBox();
        
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        
        hits = glRenderMode(GL_RENDER);
        
        int pos = 0 ;
        
        for (int i = 0 ; i < hits ; i++ )
        {
            if ( Objeto3D::EsObjetoSeleccionable( buffer[i*4+3] ) )
            {
                nbuffer[pos++] = buffer[i*4+3] ;
                if (pos >= max)
                {
                    return pos ;
                }
            }
        }
        
        return pos ;
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::GetObjetoEn(int, int)").Combinar(ex);
    }
}

int
Api::GetObjetosEnRectangulo(int x, int y, int ancho, int alto, int *nbuffer, int max)
{
    try
    {
        GLuint buffer[512];
        GLint hits ;
        int seleccion = -1 ;
        
        GLint viewport[4] ;
        glGetIntegerv(GL_VIEWPORT, viewport);
        glSelectBuffer(512, buffer);
        
        if ( ancho == 0 ) ancho = 1 ;
        if ( alto == 0 ) alto = 1 ;
        
        if ( ancho < 0 ) 
        {
            x += ancho ;
            ancho = -ancho ;
        }
        if ( alto < 0 )
        {
            y += alto ;
            alto = -alto ;
        }
        
        x += ancho/2 ;
        y += alto/2;
        
        glRenderMode(GL_SELECT);
        glInitNames();
        glPushName(0);
        
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        
        glLoadIdentity();
        gluPickMatrix(x, viewport[3]-y, ancho, alto, viewport);
        gluPerspective(45.0f, ((GLfloat) viewport[2]-viewport[0])/(viewport[3]-viewport[1]), 1.0f, 100.0f);
        
        glMatrixMode(GL_MODELVIEW);
        
        Entidad3D::PintarTodosBoundingBox();
        
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        
        hits = glRenderMode(GL_RENDER);
        
        int pos = 0 ;
        
        for (int i = 0 ; i < hits ; i++ )
        {
            if ( Objeto3D::EsObjetoSeleccionable( buffer[i*4+3] ) )
            {
                nbuffer[pos++] = buffer[i*4+3] ;
                if (pos >= max)
                {
                    return pos ;
                }
            }
        }
        
        return pos ;
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::GetObjetoEn(int, int)").Combinar(ex);
    }
}

void
Api::SetObjetoSiempreVisible(int handlerObjeto, bool siempreVisible)
{
    try
    {
        if ( objetos.at(handlerObjeto) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::SetObjetoSiempreVisible(int, bool)", 1);
        }
        objetos.at(handlerObjeto)->SetSiempreVisible(siempreVisible);
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::SetObjetoSiempreVisible(int, bool)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::SetObjetoSiempreVisible(int, bool)").Combinar(ex);
    }
}

void
Api::SetObjetoSeleccionable(int HandlerObjeto, bool Seleccionable)
{
    try
    {
        if ( objetos.at(HandlerObjeto) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::SetObjetoSeleccionable(int, bool)", 1);
        }    
        objetos.at(HandlerObjeto)->SetSeleccionable(Seleccionable);
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::SetObjetoSeleccionable(int, bool)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::SetObjetoSeleccionable(int, bool)").Combinar(ex);
    }
}

void
Api::SetObjetoBloqueante(int handlerObjeto, bool bloqueante)
{
    objetos.at(handlerObjeto)->SetBloqueante(bloqueante);    
}

void
Api::ActivarObjeto(int handlerObjeto)
{
    try
    {
        if ( objetos.at(handlerObjeto) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::ActivarObjeto(int)", 1);
        }       
        objetos.at(handlerObjeto)->SetActivo(true);
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::ActivarObjeto(int)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::ActivarObjeto(int)").Combinar(ex);
    }
}

void
Api::DesactivarObjeto(int handlerObjeto)
{
    try
    {
        if ( objetos.at(handlerObjeto) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::DesactivarObjeto(int)", 1);
        }       
        objetos.at(handlerObjeto)->SetActivo(false);
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::DesactivarObjeto(int)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::DesactivarObjeto(int)").Combinar(ex);
    }
}

bool
Api::ColisionObjeto(int handlerObjeto)
{
    Objeto3D *o = objetos.at(handlerObjeto);
    
    for ( vector<Objeto3D *>::iterator i = objetos.begin() ; i != objetos.end() ; ++i )
    {
        if ( (*i) != NULL )
        {
            if ( (*i)->GetBloqueante() )
            {
                if ( (*i) != o )
                {
                    if ( o->Colision(*i) )
                    {
                        return true ;
                    }
                }
            }
        }
    }
    return false ;
}

void
Api::SetMascaraObjeto(int handlerObjeto, bool r, bool g, bool b, bool a)
{
    objetos.at(handlerObjeto)->SetMascara(r, g, b, a);
}

float
Api::GetAlturaObjeto(int handlerObjeto)
{
    return objetos.at(handlerObjeto)->GetAltura();
}

float
Api::GetRadioObjeto(int handlerObjeto)
{
    return objetos.at(handlerObjeto)->GetRadio();
}

void
Api::SetEscalaObjeto(int handlerObjeto, float escala)
{
    objetos.at(handlerObjeto)->SetEscala(escala);
}

void
Api::SetTransparenteObjeto(int handlerObjeto, bool transparente)
{
    objetos.at(handlerObjeto)->SetTransparente(transparente);
}

void
Api::CrearTerreno(string ficheroAlturas, bool multiTextura)
{
    try
    {
        terreno = new Terreno(ficheroAlturas, multiTextura);    
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearTerreno(string)").Combinar(ex);
    }
}

void
Api::DestruirTerreno()
{
    try
    {
        if ( terreno == NULL )
        {
            throw Error::ExcepcionLocalizada("Graficos::Api::DestruirTerreno()");
        }
        delete terreno ;
        terreno = NULL ;
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::DestruirTerreno()").Combinar(ex);
    }
}

void
Api::ActivarTerreno()
{
}

void
Api::DesactivarTerreno()
{
}

void
Api::ActualizarTerreno()
{
   terreno->Actualizar();
}

int
Api::GetAnchoTerreno()
{
    try
    {
        return terreno->GetAncho();
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::GetAnchoTerreno()").Combinar(ex);    
    }
}

float
Api::GetEscalaTerreno()
{
    return Terreno::escala ;
}

Util::Vector3D
Api::GetCoordenadas2Dto3D(float wx, float wy)
{
    float wz ;
    GLdouble model[16] ;
    GLdouble proy[16] ;     
    GLint viewport[4] ;

    glGetIntegerv(GL_VIEWPORT, viewport);
 
    glReadPixels((int)wx, (int)(SCREEN_H-wy-1), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &wz);

    glGetDoublev(GL_PROJECTION_MATRIX, proy);
    glGetDoublev(GL_MODELVIEW_MATRIX, model);
    
    GLdouble x, y, z ;
    
    gluUnProject(wx, SCREEN_H-wy, wz, model, proy, viewport, &x, &y, &z);
    return Util::Vector3D(x,y,z) ;
}

Util::Vector3D
Api::GetCoordenadas2Dto3D(int coor)
{
    return coordenadasVentana[coor];
}

Util::Vector2D
Api::GetCoordenadas3Dto2D(float x, float y, float z)
{
    GLdouble model[16] ;
    GLdouble proy[16] ;     
    GLint viewport[4] ;

    glGetDoublev(GL_MODELVIEW_MATRIX, model);
    glGetDoublev(GL_PROJECTION_MATRIX, proy);
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    GLdouble wx, wy, wz ;
    
    gluProject(x, y, z, model, proy, viewport, &wx, &wy, &wz);
    return Util::Vector2D(wx,SCREEN_H-wy) ;
}

int
Api::GetAltoTerreno()
{
    try
    {
        return terreno->GetAlto();
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::GetAltoTerreno()").Combinar(ex);
    }
}

float
Api::GetAltura(int x, int z)
{
    try
    {
        if ( !terreno )
        {
            throw Error::ExcepcionLocalizada("Graficos::Api::GetAltura(int, int)");
        }
        return terreno->GetAltura(x,z)/10.0;
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::GetAltura(int, int)").Combinar(ex);
    }
}

float
Api::GetAlturaSuave(float x, float z)
{
    if ( !terreno )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::GetAlturaSuave(float, float)");
    }
    return terreno->GetAlturaSuave(x,z) ;
}

void
Api::SetInvierno(bool invierno)
{
    if ( !terreno )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::SetInvierno(bool)");
    }
    terreno->SetInvierno(invierno);
}

int
Api::CrearSonido(string fileSonido)
{
    try
    {
        Sonido *sonido = new Sonido(fileSonido);
        sonidos.push_back(sonido);
        for ( int i = 0 ; i < sonidos.size() ; i++ )
        {
            if ( sonidos.at(i) == sonido )
            {
                return i ;
            }
        }
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearSonido(string)");
    }
    catch (Error::ExcepcionParametro &ex)
    {
        if ( ex.GetNumParametroExcepcion() == 1 )
        {
            throw Error::ExcepcionParametro("Graficos::Api::CrearSonido(string)", 1).Combinar(ex);
        }
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearSonido(string)").Combinar(ex);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::CrearSonido(string)").Combinar(ex);
    }
}

void
Api::DestruirSonido(int handlerSonido)
{
    try
    {
        if ( sonidos.at(handlerSonido) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::DestruirSonido(int)", 1);
        }
        delete sonidos.at(handlerSonido) ;
        sonidos.at(handlerSonido) = NULL ;
    }
    catch (out_of_range &ex )
    {
        throw Error::ExcepcionParametro("Graficos::Api::DestruirSonido(int)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::DestruirSonido(int)").Combinar(ex);
    }
}    

void
Api::ReproducirSonido(int handlerSonido)
{
    try
    {
        if ( sonidos.at(handlerSonido) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::ReproducirSonido(int)", 1);
        }
        sonidos.at(handlerSonido)->Play();
    }
    catch (out_of_range &ex)
    {
        throw Error::ExcepcionParametro("Graficos::Api::ReproducirSonido(int)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::ReproducirSonido(int)").Combinar(ex);
    }
}

void
Api::ReproducirLoopSonido(int handlerSonido)
{
    try
    {
        if ( sonidos.at(handlerSonido) == NULL )
        {
            throw Error::ExcepcionParametro("Graficos::Api::ReproducirLoopSonido(int)", 1);
        }
        sonidos.at(handlerSonido)->PlayLoop();
    }
    catch (out_of_range &ex)
    {
        throw Error::ExcepcionParametro("Graficos::Api::ReproducirLoopSonido(int)", 1);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::ReproducirLoopSonido(int)").Combinar(ex);
    }
}

void
Api::ResetCamara()
{
    glLoadIdentity();
}

void
Api::EscalarCamara(float x, float y, float z)
{
    glScalef(x, y, z);
}

void
Api::TrasladarCamara(float x, float y, float z)
{
    glTranslatef(x, y, z);
}

void
Api::RotarCamara(float angulo, float x, float y, float z)
{
    glRotatef(angulo, x, y, z);
}

void
Api::SetPosicionCamara(float x, float y, float z)
{
    camaraX = x ;
    camaraY = y ;
    camaraZ = z ;
}

void
Api::GetPosicionCamara(float &x, float &y, float &z)
{
    x = camaraX ;
    y = camaraY ;
    z = camaraZ ;
}

void
Api::SetInclinacionCamara(float angulo)
{
    camaraIncX = angulo ;
}

void
Api::MostrarRaton()
{
    mostrarRaton = true ;
}

void
Api::OcultarRaton()
{
    mostrarRaton = false ;
}

void
Api::SetImagenRaton(int HandlerImagen)
{
    HandlerImagenRaton = HandlerImagen ;
}

void
Api::SetIluminacionDifusa(int nframes, float r, float g, float b, float a)
{
    desDiffuseLight0[0] = r ;
    desDiffuseLight0[1] = g ;
    desDiffuseLight0[2] = b ;
    desDiffuseLight0[3] = a ;
    
    for ( int i = 0 ; i < 4 ; i++ )
    {
        velDiffuseLight0[i] = (desDiffuseLight0[i]-diffuseLight0[i])/nframes ;
    }
}

void
Api::SetIluminacionAmbiente(int nframes, float r, float g, float b, float a)
{
    desAmbientLight0[0] = r ;
    desAmbientLight0[1] = g ;
    desAmbientLight0[2] = b ;
    desAmbientLight0[3] = a ;
    
    for ( int i = 0 ; i < 4 ; i++ )
    {
        velAmbientLight0[i] = (desAmbientLight0[i]-ambientLight0[i])/nframes ;
    }
}

void
Api::Blit(GLuint textura, int OrigenX, int OrigenY, int DestinoX, int DestinoY, int Ancho, int Alto)
{
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glColor4f(1,1,1,1);
    
    glBindTexture(GL_TEXTURE_2D, textura);	
    			
	glBegin(GL_QUADS);
        glTexCoord2f(0, 1);
        glVertex2f( DestinoX, DestinoY);
        
        glTexCoord2f(1, 1);
        glVertex2f( DestinoX+Ancho, DestinoY);
        
        glTexCoord2f(1, 0);
        glVertex2f( DestinoX+Ancho, DestinoY+Alto );
        
        glTexCoord2f(0, 0);
        glVertex2f( DestinoX, DestinoY+Alto );
	glEnd();
    
}

void
Api::MaskedBlit(GLuint textura, int OrigenX, int OrigenY, int DestinoX, int DestinoY, int Ancho, int Alto)
{
    glDisable(GL_DEPTH_TEST);
    glColor4f(1,1,1,1);
    glEnable(GL_ALPHA_TEST);
    glBindTexture(GL_TEXTURE_2D, textura);				
	glBegin(GL_QUADS);
        glTexCoord2f(0, 1);
        glVertex2f( DestinoX, DestinoY);
        
        glTexCoord2f(1, 1);
        glVertex2f( DestinoX+Ancho, DestinoY);
        
        glTexCoord2f(1, 0);
        glVertex2f( DestinoX+Ancho, DestinoY+Alto );
        
        glTexCoord2f(0, 0);
        glVertex2f( DestinoX, DestinoY+Alto );
	glEnd();
	glDisable(GL_ALPHA_TEST);
}

void
Api::DrawTransSprite(GLuint textura, int OrigenX, int OrigenY, int DestinoX, int DestinoY, int Ancho, int Alto, int Alpha)
{
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);    
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glColor4f(1.0, 1.0, 1.0, Alpha/255.0);
	
    glBindTexture(GL_TEXTURE_2D, textura);				
	glBegin(GL_QUADS);
        glTexCoord2f(0, 1);
        glVertex2f( DestinoX, DestinoY);
        
        glTexCoord2f(1, 1);
        glVertex2f( DestinoX+Ancho, DestinoY);
        
        glTexCoord2f(1, 0);
        glVertex2f( DestinoX+Ancho, DestinoY+Alto );
        
        glTexCoord2f(0, 0);
        glVertex2f( DestinoX, DestinoY+Alto );
	glEnd(); 
	
	glColor4f(1.0, 1.0, 1.0, 1.0);
	
	glBlendFunc(GL_ONE, GL_ZERO);
	glDisable(GL_BLEND);	
}

int
Api::ReservarCallList()
{
    for ( int i = 1 ; i < maxCallLists ; i++ )
    {
        if ( ! callLists[i] )
        {
            callLists[i] = true ;
            return i ;
        }
    }
    throw Error::ExcepcionLocalizada("Graficos::Api::ReservarCallList()", "No quedan CallLists libres");
}

int
Api::ReservarCallListAlta()
{
    for ( int i = callListsAltos ; i < maxCallLists ; i++ )
    {
        if ( ! callLists[i] )
        {
            callLists[i] = true ;
            return i ;
        }
    }
    throw Error::ExcepcionLocalizada("Graficos::Api::ReservarCallListAlta()", "No quedan CallLists libres");
}    

void
Api::LiberarCallList(int callList)
{
    if ( callList < 0 || callList >= maxCallLists )
    {
        throw Error::ExcepcionParametro("Graficos::Api::LiberarCallList(int)", 1) ;
    }
    callLists[callList] = false ;
}

void
Api::ResetClippingReg(int callList)
{
    if ( callList < 0 || callList >= maxCallLists )
    {
        throw Error::ExcepcionParametro("Graficos::Api::ResetClippingReg(int)", 1) ;
    }
    clipping[callList] = false ;
}

bool
Api::ComprobarClippingReg(int callList)
{
    if ( callList < 0 || callList >= maxCallLists )
    {
        throw Error::ExcepcionParametro("Graficos::Api::ComprobarClippingReg(int)", 1) ;
    }
    return clipping[callList] ;
}

void
Api::CalcularClipping()
{
    try
    {
        GLuint buffer[512];
        GLint hits ;
        
        glSelectBuffer(512, buffer);
        
        glRenderMode(GL_SELECT);
        glInitNames();
        glPushName(0);

        Entidad3D::PintarTodosBoundingBox();
        Entidad3D::ResetClippingRegs();
        
        hits = glRenderMode(GL_RENDER);
        
        for (int i = 0 ; i < hits ; i++ )
        {
            clipping[buffer[i*4+3]] = true ;
        }
        return ;
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::CalcularClipping()").Combinar(ex);
    }    
}

void
Api::CalcularCoordenadasVentana()
{
    double x, y, z ;
    Util::Vector2D v ;
    
    coordenadasVentana[CoordenadasVentana::ArribaIzquierda] =   GetCoordenadas2Dto3D(0, 0);
    coordenadasVentana[CoordenadasVentana::ArribaDerecha] =     GetCoordenadas2Dto3D(SCREEN_W-1, 0);
    coordenadasVentana[CoordenadasVentana::AbajoIzquierda] =    GetCoordenadas2Dto3D(0, SCREEN_H-1);
    coordenadasVentana[CoordenadasVentana::AbajoDerecha] =      GetCoordenadas2Dto3D(SCREEN_W-1, SCREEN_H-1);
    coordenadasVentana[CoordenadasVentana::CentroVentana] =     GetCoordenadas2Dto3D(SCREEN_W/2, SCREEN_H/2);                
    coordenadasVentana[CoordenadasVentana::PosicionRaton] =     GetCoordenadas2Dto3D(mouse_x, mouse_y);
}

void
Api::PintarLuz()
{
    for ( int i = 0 ; i < 4 ; i++ )
    {
        if ( diffuseLight0[i] != desDiffuseLight0[i] )
        {
            float temp = diffuseLight0[i] ;
            diffuseLight0[i] += velDiffuseLight0[i] ;
            if ( (temp < desDiffuseLight0[i] && diffuseLight0[i] > desDiffuseLight0[i]) ||
                 (temp > desDiffuseLight0[i] && diffuseLight0[i] < desDiffuseLight0[i]))
            {
                diffuseLight0[i] = desDiffuseLight0[i] ;
            }
        }

        if ( ambientLight0[i] != desAmbientLight0[i] )
        {
            float temp = ambientLight0[i] ;
            ambientLight0[i] += velAmbientLight0[i] ;
            if ( (temp < desAmbientLight0[i] && ambientLight0[i] > desAmbientLight0[i]) ||
                 (temp > desAmbientLight0[i] && ambientLight0[i] < desAmbientLight0[i]))
            {
                ambientLight0[i] = desAmbientLight0[i] ;
            }
        }
        

    }

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);

    // Situamos la luz
	glLightfv(GL_LIGHT0, GL_POSITION, positionLight0);    
}

void
Api::Pintar()
{
    int tiempo = GetTickCount() ;
    try
    {        
        //glFlush();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glClear(GL_DEPTH_BUFFER_BIT);
        
        PintarLuz();
    	
    	float ambient[] = {0.2,0.2,0.2,1.0} ;
    	float diffuse[] = {1.0,1.0,1.0,1.0} ;
    	float specular[] = {0,0,0,0} ;
    	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);        
    	glColor4f(1,1,1,1);
        

        // Pintamos el terreno
        if ( terreno )
        {
            glEnable(GL_DEPTH_TEST);
                terreno->Pintar();
            glDisable(GL_DEPTH_TEST);
            
            CalcularCoordenadasVentana();
        }
        
        //if ( Api::frecuenciaClipping.comprobar() )
        //{
            CalcularClipping();
        //}
        
        glEnable(GL_DEPTH_TEST);
            int tiempoPTC = GetTickCount();
            Entidad3D::PintarTodosClipping();
            //cout << " Entidad3D::PintarTodosClipping(): " << GetTickCount() - tiempoPTC << endl ;
        glDisable(GL_DEPTH_TEST);
    
        allegro_gl_set_projection();    
            glDisable(GL_LIGHTING);
        
            Entidad2D::PintarTodos();
        
            // Mostrar raton
            if ( mostrarRaton )
            {
                imagenes.at(HandlerImagenRaton)->Pintar(mouse_x, mouse_y);
            }
        
            glEnable(GL_LIGHTING);
        allegro_gl_unset_projection();
    
        //glFlush();
    
        
        glFlush();
    
        // Cambiamos la pagina de video
        int tiempoFlip = GetTickCount();
        allegro_gl_flip();   
        //cout << " allegro_gl_flip(): " << GetTickCount() - tiempoFlip << endl ;
        
        
    
        // Actualizamos los inputs
        allegro_gl_set_allegro_mode(); 
        allegro_gl_unset_allegro_mode(); 
           
    }
    catch ( out_of_range &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::Pintar()");
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Api::Pintar()").Combinar(ex);
    }
    //cout << " Api::Pintar: " << GetTickCount() - tiempo << endl ;
}

}  
