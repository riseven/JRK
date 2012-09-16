#include "gSegmento.h"
#include "eExcepcionParametro.h"

namespace Graficos
{

Segmento::Segmento(float r, float g, float b, float a, int X, int Y, int finX, int finY, int Z):
    Entidad2D(X, Y, Z), finX(finX), finY(finY)
{
    color[0] = r ;
    color[1] = g ;
    color[2] = b ;
    color[3] = a ;
    clip = false ;
}

Segmento::~Segmento()
{
    color[0] = 0 ;
    color[1] = 0 ;
    color[2] = 0 ;
    color[3] = 0 ;
    finX = 0 ;
    finY = 0 ;
}

void
Segmento::SetClipping(int x0, int y0, int x1, int y1)
{
    xmin = x0 ;
    xmax = x1 ;
    ymin = y0 ;
    ymax = y1 ;
    clip = true ;
}

void
Segmento::Pintar()
{
    int x0, y0, x1, y1 ;
    int mascara0 = 0 ;
    int mascara1 = 0 ;
    
    if ( clip )
    {
        if ( x < xmin ) mascara0 |= 1 ;
        if ( x > xmax ) mascara0 |= 4 ;
        if ( y < ymin ) mascara0 |= 2 ;
        if ( y > ymax ) mascara0 |= 8 ;
        
        if ( finX < xmin ) mascara1 |= 1 ;
        if ( finX > xmax ) mascara1 |= 4 ;
        if ( finY < ymin ) mascara1 |= 2 ;
        if ( finY > ymax ) mascara1 |= 8 ;
        
        if ( mascara0 | mascara1 )
        {
            if ( mascara0 && mascara1 )
            {
                // No se pinta nada
                // Se ha hecho un and logico en lugar de binario para simplificar
                // puesto que solo hacemos clipping en los segmentos del area visible
                // del minimapa, y podemos descartar el caso de visibilidad con los
                // dos extremos fuera de la region de clipping.  
            }
            else
            {
                // Interseccionamos
                
                // Casos especiales
                if ( finY == y || finX == x)
                {
                    // No se pinta nada   
                }
                else
                {
                    x0 = x ;
                    y0 = y ;
                    x1 = finX ;
                    y1 = finY ;                    
                    
                    if ( mascara1 )
                    {
                        // Como se ha rechado mascara0 && mascara1, solo hay un 
                        // vertice exterior, y cargamos su mascara en mascara0
                        mascara0 = mascara1 ;
                        int temp ;
                        temp = x0 ;
                        x0 = x1 ;
                        x1 = temp ;
                        temp = y0 ;
                        y0 = y1 ;
                        y1 = temp ;
                    }
                    
                    // Calculamos la pendiente
                    double n = ((finY-y)*1.0)/(finX-x) ;
                    
                    // Calculamos el offset
                    double m = y - x*n ;
                    
                    bool acabar = false ;
                    
                    if ( mascara0 & 1 )
                    {
                        int ny = m + xmin*n ;
                        if ( ny >= ymin && ny <= ymax )
                        {
                            acabar = true ;
                            x0 = xmin ;
                            y0 = ny ;
                        }
                    }
                    
                    if ( mascara0 & 4 && !acabar )
                    {
                        int ny = m + xmax*n ;
                        if ( ny >= ymin && ny <= ymax )
                        {
                            acabar = true ;
                            x0 = xmax ;
                            y0 = ny ;
                        }
                    }
                    
                    if ( mascara0 & 2 && !acabar )
                    {
                        int nx = (ymin-m)/n ;
                        if ( nx >= xmin && nx <= xmax )
                        {
                            acabar = true ;
                            x0 = nx ;
                            y0 = ymin ;
                        }
                    }
                    
                    if ( mascara0 & 8 && !acabar )
                    {
                        int nx = (ymax-m)/n ;
                        if ( nx >= xmin && nx <= xmax )
                        {
                            acabar = true ;
                            x0 = nx ;
                            y0 = ymax ;
                        }
                    }
                }
            }
        }
        else
        {
            // Los dos puntos estan dentro
            x0 = x ;
            y0 = y ;
            x1 = finX ;
            y1 = finY ;
        }
    }
    else
    {
        x0 = x ;
        y0 = y ;
        x1 = finX ;
        y1 = finY ;        
    }
    
    try
    {
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);
        glColor4fv(color);
        glBegin(GL_LINES);
            glVertex2f(x0,y0);
            glVertex2f(x1,y1);
        glEnd();
        glEnable(GL_TEXTURE_2D);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Segmento::Pintar()").Combinar(ex);
    }
}

void
Segmento::SetFinalSegmento(int finX, int finY)
{
    Segmento::finX = finX ;
    Segmento::finY = finY ;
}

}
