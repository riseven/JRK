#include <allegro.h>
#include <alleggl.h>

#include "gTerreno.h"
#include "gImagen.h"
#include "gApi.h"
#include "eExcepcionLocalizada.h"
#include <cmath>

#define PIOVER180   0.0174532925

namespace Graficos
{
    
Terreno::Terreno(string ficheroAlturas, bool multiTextura)
{
    try
    {
        numElemento = 0 ;
        listasTerreno = NULL ;
        Terreno::multiTextura = multiTextura ;
        
        alphaNevado = 0.0 ;
        invierno = false ;
        
        BITMAP *bitmap = Imagen::CargarBitmap(ficheroAlturas);
        

        w = bitmap->w;
        h = bitmap->h;

    
        // Inicializamos la matriz de alturas
        alturas = new float[w*h] ;
        
        for ( int i = 0 ; i < h ; i++ )
        {
            for ( int j = 0 ; j < w ; j++ )
            {
                // Utilizamos el canal rojo como se podria usar cualquier otro,
                // se supone que es una imagen en tonos de grises, con todos los
                // canales iguales
                alturas[i*w + j] = (float)getr32( getpixel(bitmap, j, i) );
            }
        }
        
        int col = ((w-1)/10)+1 ;
        int fil = ((h-1)/10)+1 ;
        
        numElementos = fil*col ;
        listasTerreno = new int[fil*col] ;
        listasTerrenoHierba = new int[fil*col] ;
        listasTerrenoNieve = new int[fil*col] ;
        clip = new bool[fil*col] ;
        normales = new Util::Vector3D[w*h];
        
        //Cargamos la textura de hierba
        hierba = new Imagen("Texturas\\Hierba.bmp");
        //nevado = new Imagen("Texturas\\Nevado.bmp");
        agua = new Imagen("Texturas\\Agua.bmp");
        rocas = new Imagen("Texturas\\Rocas.bmp");
        nieve = new Imagen("Texturas\\Nieve.bmp");
        
        nevado[0] = new Imagen("Texturas\\Nevado05.bmp");
        nevado[1] = new Imagen("Texturas\\Nevado15.bmp");
        nevado[2] = new Imagen("Texturas\\Nevado25.bmp");
        nevado[3] = new Imagen("Texturas\\Nevado35.bmp");
        nevado[4] = new Imagen("Texturas\\Nevado45.bmp");
        nevado[5] = new Imagen("Texturas\\Nevado55.bmp");
        nevado[6] = new Imagen("Texturas\\Nevado65.bmp");
        nevado[7] = new Imagen("Texturas\\Nevado.bmp");                                                        
        
        float ymin, ymax ;
        
        //GenerarTexturasHierba();
        
        
        // Calculamos las normales, recorriendo los triangulos, y sumando la normal
        // a cada vertice implicado.
        for ( int i = 0 ; i < h-1 ; i++ )
        {
            for ( int j = 0 ; j < w-1 ; j++ )
            {
                // Calculasmos los vertices del quad
                Util::Vector3D v1(     j*2.0, GetAltura(  j,  i)/10.0,     i*2.0 );
                Util::Vector3D v2( (j+1)*2.0, GetAltura(j+1,  i)/10.0,     i*2.0 );
                Util::Vector3D v3( (j+1)*2.0, GetAltura(j+1,i+1)/10.0, (i+1)*2.0 );
                Util::Vector3D v4(     j*2.0, GetAltura(  j,i+1)/10.0, (i+1)*2.0 );
                
                // Calculamos la normal del primer triangulo
                Util::Vector3D vec1 = v2 - v1 ;
                Util::Vector3D vec2 = v3 - v1 ;
                Util::Vector3D normal = vec2.MultVec(vec1);
                normal.Normalizar();
                
                normales[(i  )*w + j  ] += normal ;
                normales[(i  )*w + j+1] += normal ;
                normales[(i+1)*w + j+1] += normal ;
                
                // Calculamos la normal del segundo triangulo
                vec1 = v3-v1 ;
                vec2 = v4-v1 ;
                normal = vec2.MultVec(vec1);
                normal.Normalizar();
                
                normales[(i  )*w + j  ] += normal ;
                normales[(i+1)*w + j+1] += normal ;
                normales[(i+1)*w + j  ] += normal ;
            }
        }
        
        // Ahora normalizamos las normales
        for ( int i = 0 ; i < h ; i++ )
        {
            for ( int j = 0 ; j < w ; j++ )
            {
                normales[i*w+j].Normalizar();
            }
        }
                
        glInterleavedArrays(GL_T2F_C4F_N3F_V3F, 0, interleavedElements);
        glEnable(GL_BLEND);
        
        float minAng = 40.0 ;
        float maxAng = 70.0 ;
        
        float minH = 175 ;
        float maxH = 215 ;
        
        for ( int y = 0 ; y < fil ; y++ )
        {
            for ( int x = 0 ; x < col ; x++ )
            {
                numElemento = 0 ;
                ymin = ymax = GetAltura(x*10,y*10)/10.0 ;                
                
                // Capa de rocas
                for ( int i = y*10 ; i < (y+1)*10 && i < h-1 ; i++ )
                {
                    for ( int j = x*10 ; j < (x+1)*10 && j < w-1 ; j++ )
                    {
                        float temp = GetAltura(j,i)/10.0 ;
                        if ( ymin > temp )
                        {
                            ymin = temp ;
                        }
                        if ( ymax < temp )
                        {
                            ymax = temp ;
                        }                                               
                
                        
                
                        Util::Vector3D normal ;
                        bool visible ;
                        float ang ;
                        float a ;
                              
                        //******************************************************  
                        // Primer triangulo
                        //******************************************************
                        visible = false ;
                        
                        // Primer Vertice --------------------------------------
                        normal = normales[(i  )*w + j  ];
                        ang = (asin(normal.GetY())/PIOVER180) ;
                        if ( ang < maxAng ) visible = true ;
                        
                        // Textura
                        interleavedElements[numElemento].texture[0] = (j  )/4.0 ;
                        interleavedElements[numElemento].texture[1] = (i  )/4.0 ;
                        
                        // Color
                        interleavedElements[numElemento].color[0] = 1.0 ;
                        interleavedElements[numElemento].color[1] = 1.0 ;
                        interleavedElements[numElemento].color[2] = 1.0 ;
                        interleavedElements[numElemento].color[3] = 1.0 ;
                        
                        // Normal
                        interleavedElements[numElemento].normal[0] = normal.GetX() ;
                        interleavedElements[numElemento].normal[1] = normal.GetY() ;
                        interleavedElements[numElemento].normal[2] = normal.GetZ() ;
                        
                        // Vertice
                        interleavedElements[numElemento].vertex[0] = (j  )*2.0 ;
                        interleavedElements[numElemento].vertex[1] = GetAltura(j  ,i  )/10.0 ;
                        interleavedElements[numElemento].vertex[2] = (i  )*2.0 ;
                        
                        numElemento++ ;
                        
                        // Segundo Vertice -------------------------------------
                        normal = normales[(i  )*w + j+1];
                        ang = (asin(normal.GetY())/PIOVER180) ;
                        if ( ang < maxAng ) visible = true ;                        
                        
                        // Textura
                        interleavedElements[numElemento].texture[0] = (j+1)/4.0 ;
                        interleavedElements[numElemento].texture[1] = (i  )/4.0 ;
                        
                        // Color
                        interleavedElements[numElemento].color[0] = 1.0 ;
                        interleavedElements[numElemento].color[1] = 1.0 ;
                        interleavedElements[numElemento].color[2] = 1.0 ;
                        interleavedElements[numElemento].color[3] = 1.0 ;
                        
                        // Normal
                        interleavedElements[numElemento].normal[0] = normal.GetX() ;
                        interleavedElements[numElemento].normal[1] = normal.GetY() ;
                        interleavedElements[numElemento].normal[2] = normal.GetZ() ;
                        
                        // Vertice
                        interleavedElements[numElemento].vertex[0] = (j+1)*2.0 ;
                        interleavedElements[numElemento].vertex[1] = GetAltura(j+1,i  )/10.0 ;
                        interleavedElements[numElemento].vertex[2] = (i  )*2.0 ;
                        
                        numElemento++ ;
                        
                        // Tercer Vertice --------------------------------------
                        normal = normales[(i+1)*w + j+1];
                        ang = (asin(normal.GetY())/PIOVER180) ;
                        if ( ang < maxAng ) visible = true ;                        
                        
                        // Textura
                        interleavedElements[numElemento].texture[0] = (j+1)/4.0 ;
                        interleavedElements[numElemento].texture[1] = (i+1)/4.0 ;
                        
                        // Color
                        interleavedElements[numElemento].color[0] = 1.0 ;
                        interleavedElements[numElemento].color[1] = 1.0 ;
                        interleavedElements[numElemento].color[2] = 1.0 ;
                        interleavedElements[numElemento].color[3] = 1.0 ;
                        
                        // Normal
                        interleavedElements[numElemento].normal[0] = normal.GetX() ;
                        interleavedElements[numElemento].normal[1] = normal.GetY() ;
                        interleavedElements[numElemento].normal[2] = normal.GetZ() ;
                        
                        // Vertice
                        interleavedElements[numElemento].vertex[0] = (j+1)*2.0 ;
                        interleavedElements[numElemento].vertex[1] = GetAltura(j+1,i+1)/10.0 ;
                        interleavedElements[numElemento].vertex[2] = (i+1)*2.0 ;
                        
                        numElemento++ ;     

                        if ( !visible ) numElemento -= 3 ;

                        //******************************************************  
                        // Primer triangulo
                        //******************************************************
                        
                        visible = false ;
                        
                        // Primer Vertice --------------------------------------
                        normal = normales[(i  )*w + j  ];
                        ang = (asin(normal.GetY())/PIOVER180) ;
                        if ( ang < maxAng ) visible = true ;                        
                        
                        // Textura
                        interleavedElements[numElemento].texture[0] = (j  )/4.0 ;
                        interleavedElements[numElemento].texture[1] = (i  )/4.0 ;
                        
                        // Color
                        interleavedElements[numElemento].color[0] = 1.0 ;
                        interleavedElements[numElemento].color[1] = 1.0 ;
                        interleavedElements[numElemento].color[2] = 1.0 ;
                        interleavedElements[numElemento].color[3] = 1.0 ;
                        
                        // Normal
                        interleavedElements[numElemento].normal[0] = normal.GetX() ;
                        interleavedElements[numElemento].normal[1] = normal.GetY() ;
                        interleavedElements[numElemento].normal[2] = normal.GetZ() ;
                        
                        // Vertice
                        interleavedElements[numElemento].vertex[0] = (j  )*2.0 ;
                        interleavedElements[numElemento].vertex[1] = GetAltura(j  ,i  )/10.0 ;
                        interleavedElements[numElemento].vertex[2] = (i  )*2.0 ;
                        
                        numElemento++ ;
                        
                        // Segundo Vertice -------------------------------------
                        normal = normales[(i+1)*w + j+1];
                        ang = (asin(normal.GetY())/PIOVER180) ;
                        if ( ang < maxAng ) visible = true ;                        
                        
                        // Textura
                        interleavedElements[numElemento].texture[0] = (j+1)/4.0 ;
                        interleavedElements[numElemento].texture[1] = (i+1)/4.0 ;
                        
                        // Color
                        interleavedElements[numElemento].color[0] = 1.0 ;
                        interleavedElements[numElemento].color[1] = 1.0 ;
                        interleavedElements[numElemento].color[2] = 1.0 ;
                        interleavedElements[numElemento].color[3] = 1.0 ;
                        
                        // Normal
                        interleavedElements[numElemento].normal[0] = normal.GetX() ;
                        interleavedElements[numElemento].normal[1] = normal.GetY() ;
                        interleavedElements[numElemento].normal[2] = normal.GetZ() ;
                        
                        // Vertice
                        interleavedElements[numElemento].vertex[0] = (j+1)*2.0 ;
                        interleavedElements[numElemento].vertex[1] = GetAltura(j+1,i+1)/10.0 ;
                        interleavedElements[numElemento].vertex[2] = (i+1)*2.0 ;                        

                        numElemento++ ;
                        
                        // Tercer Vertice --------------------------------------
                        normal = normales[(i+1)*w + j  ];
                        ang = (asin(normal.GetY())/PIOVER180) ;
                        if ( ang < maxAng ) visible = true ;                        
                        
                        // Textura
                        interleavedElements[numElemento].texture[0] = (j  )/4.0 ;
                        interleavedElements[numElemento].texture[1] = (i+1)/4.0 ;
                        
                        // Color
                        interleavedElements[numElemento].color[0] = 1.0 ;
                        interleavedElements[numElemento].color[1] = 1.0 ;
                        interleavedElements[numElemento].color[2] = 1.0 ;
                        interleavedElements[numElemento].color[3] = 1.0 ;
                        
                        // Normal
                        interleavedElements[numElemento].normal[0] = normal.GetX() ;
                        interleavedElements[numElemento].normal[1] = normal.GetY() ;
                        interleavedElements[numElemento].normal[2] = normal.GetZ() ;
                        
                        // Vertice
                        interleavedElements[numElemento].vertex[0] = (j  )*2.0 ;
                        interleavedElements[numElemento].vertex[1] = GetAltura(j  ,i+1)/10.0 ;
                        interleavedElements[numElemento].vertex[2] = (i+1)*2.0 ;
                        
                        numElemento++ ;
                        
                        if ( !visible ) numElemento -= 3 ;

                    }
                }
                
                int lista = Api::ReservarCallListAlta();
                
                glNewList(lista, GL_COMPILE);
                    glDrawArrays(GL_TRIANGLES, 0, numElemento);
                glEndList();
                
                
                listasTerreno[(y*col)+x] = lista ;


                numElemento = 0 ;
                // Capa de hierba
                for ( int i = y*10 ; i < (y+1)*10 && i < h-1 ; i++ )
                {
                    for ( int j = x*10 ; j < (x+1)*10 && j < w-1 ; j++ )
                    {
                        Util::Vector3D normal ;
                        bool visible ;
                        bool visibleH ;
                        float ang ;
                        float a ;
                        float h ;
                              
                        //******************************************************  
                        // Primer triangulo
                        //******************************************************
                        visible = false ;
                        visibleH = false ;
                        
                        // Primer Vertice --------------------------------------
                        normal = normales[(i  )*w + j  ];
                        ang = (asin(normal.GetY())/PIOVER180) ;
                        if ( ang < minAng )
                        {
                            a = 0.0 ;
                        }
                        else
                        {
                            visible = true ;
                            if ( ang > maxAng )
                            {
                                a = 1.0 ;
                            }
                            else
                            {
                                a = (ang - minAng)/(maxAng-minAng) ;
                            }
                        }
                        h = GetAltura(j  ,i  ) ;
                        if ( h > maxH )
                        {
                            a = 0.0 ;
                        }
                        else
                        {
                            visibleH = true ;
                            if ( a != 1.0 )
                            {                                
                                if ( h > minH )
                                {
                                    a *= (maxH-h)/(maxH-minH);
                                }
                            }
                        }
                        
                        // Textura
                        interleavedElements[numElemento].texture[0] = (j  )/4.0 ;
                        interleavedElements[numElemento].texture[1] = (i  )/4.0 ;
                        
                        // Color
                        interleavedElements[numElemento].color[0] = 1.0 ;
                        interleavedElements[numElemento].color[1] = 1.0 ;
                        interleavedElements[numElemento].color[2] = 1.0 ;
                        interleavedElements[numElemento].color[3] = a ;
                        
                        // Normal
                        interleavedElements[numElemento].normal[0] = normal.GetX() ;
                        interleavedElements[numElemento].normal[1] = normal.GetY() ;
                        interleavedElements[numElemento].normal[2] = normal.GetZ() ;
                        
                        // Vertice
                        interleavedElements[numElemento].vertex[0] = (j  )*2.0 ;
                        interleavedElements[numElemento].vertex[1] = GetAltura(j  ,i  )/10.0 ;
                        interleavedElements[numElemento].vertex[2] = (i  )*2.0 ;
                        
                        numElemento++ ;
                        
                        // Segundo Vertice -------------------------------------
                        normal = normales[(i  )*w + j+1];
                        ang = (asin(normal.GetY())/PIOVER180) ;
                        if ( ang < minAng )
                        {
                            a = 0.0 ;
                        }
                        else
                        {
                            visible = true ;
                            if ( ang > maxAng )
                            {
                                a = 1.0 ;
                            }
                            else
                            {
                                a = (ang - minAng)/(maxAng-minAng) ;
                            }
                        }                    
                        h = GetAltura(j+1,i  ) ;
                        if ( h > maxH )
                        {
                            a = 0.0 ;
                        }
                        else
                        {
                            visibleH = true ;
                            if ( a != 1.0 )
                            {                                
                                if ( h > minH )
                                {
                                    a *= (maxH-h)/(maxH-minH);
                                }
                            }
                        }
                        
                        // Textura
                        interleavedElements[numElemento].texture[0] = (j+1)/4.0 ;
                        interleavedElements[numElemento].texture[1] = (i  )/4.0 ;
                        
                        // Color
                        interleavedElements[numElemento].color[0] = 1.0 ;
                        interleavedElements[numElemento].color[1] = 1.0 ;
                        interleavedElements[numElemento].color[2] = 1.0 ;
                        interleavedElements[numElemento].color[3] = a ;
                        
                        // Normal
                        interleavedElements[numElemento].normal[0] = normal.GetX() ;
                        interleavedElements[numElemento].normal[1] = normal.GetY() ;
                        interleavedElements[numElemento].normal[2] = normal.GetZ() ;
                        
                        // Vertice
                        interleavedElements[numElemento].vertex[0] = (j+1)*2.0 ;
                        interleavedElements[numElemento].vertex[1] = GetAltura(j+1,i  )/10.0 ;
                        interleavedElements[numElemento].vertex[2] = (i  )*2.0 ;
                        
                        numElemento++ ;
                        
                        // Tercer Vertice --------------------------------------
                        normal = normales[(i+1)*w + j+1];
                        ang = (asin(normal.GetY())/PIOVER180) ;
                        if ( ang < minAng )
                        {
                            a = 0.0 ;
                        }
                        else
                        {
                            visible = true ;
                            if ( ang > maxAng )
                            {
                                a = 1.0 ;
                            }
                            else
                            {
                                a = (ang - minAng)/(maxAng-minAng) ;
                            }
                        }                   
                        h = GetAltura(j+1,i+1) ;
                        if ( h > maxH )
                        {
                            a = 0.0 ;
                        }
                        else
                        {
                            visibleH = true ;
                            if ( a != 1.0 )
                            {                                
                                if ( h > minH )
                                {
                                    a *= (maxH-h)/(maxH-minH);
                                }
                            }
                        }
                        
                        // Textura
                        interleavedElements[numElemento].texture[0] = (j+1)/4.0 ;
                        interleavedElements[numElemento].texture[1] = (i+1)/4.0 ;
                        
                        // Color
                        interleavedElements[numElemento].color[0] = 1.0 ;
                        interleavedElements[numElemento].color[1] = 1.0 ;
                        interleavedElements[numElemento].color[2] = 1.0 ;
                        interleavedElements[numElemento].color[3] = a ;
                        
                        // Normal
                        interleavedElements[numElemento].normal[0] = normal.GetX() ;
                        interleavedElements[numElemento].normal[1] = normal.GetY() ;
                        interleavedElements[numElemento].normal[2] = normal.GetZ() ;
                        
                        // Vertice
                        interleavedElements[numElemento].vertex[0] = (j+1)*2.0 ;
                        interleavedElements[numElemento].vertex[1] = GetAltura(j+1,i+1)/10.0 ;
                        interleavedElements[numElemento].vertex[2] = (i+1)*2.0 ;
                        
                        numElemento++ ;     

                        if ( (!visible || !visibleH)&&multiTextura ) numElemento -= 3 ;

                        //******************************************************  
                        // Primer triangulo
                        //******************************************************
                        
                        visible = false ;
                        
                        // Primer Vertice --------------------------------------
                        normal = normales[(i  )*w + j  ];
                        ang = (asin(normal.GetY())/PIOVER180) ;
                        if ( ang < minAng )
                        {
                            a = 0.0 ;
                        }
                        else
                        {
                            visible = true ;
                            if ( ang > maxAng )
                            {
                                a = 1.0 ;
                            }
                            else
                            {
                                a = (ang - minAng)/(maxAng-minAng) ;
                            }
                        }                 
                        h = GetAltura(j  ,i  ) ;
                        if ( h > maxH )
                        {
                            a = 0.0 ;
                        }
                        else
                        {
                            visibleH = true ;
                            if ( a != 1.0 )
                            {                                
                                if ( h > minH )
                                {
                                    a *= (maxH-h)/(maxH-minH);
                                }
                            }
                        }
                        
                        // Textura
                        interleavedElements[numElemento].texture[0] = (j  )/4.0 ;
                        interleavedElements[numElemento].texture[1] = (i  )/4.0 ;
                        
                        // Color
                        interleavedElements[numElemento].color[0] = 1.0 ;
                        interleavedElements[numElemento].color[1] = 1.0 ;
                        interleavedElements[numElemento].color[2] = 1.0 ;
                        interleavedElements[numElemento].color[3] = a ;
                        
                        // Normal
                        interleavedElements[numElemento].normal[0] = normal.GetX() ;
                        interleavedElements[numElemento].normal[1] = normal.GetY() ;
                        interleavedElements[numElemento].normal[2] = normal.GetZ() ;
                        
                        // Vertice
                        interleavedElements[numElemento].vertex[0] = (j  )*2.0 ;
                        interleavedElements[numElemento].vertex[1] = GetAltura(j  ,i  )/10.0 ;
                        interleavedElements[numElemento].vertex[2] = (i  )*2.0 ;
                        
                        numElemento++ ;
                        
                        // Segundo Vertice -------------------------------------
                        normal = normales[(i+1)*w + j+1];
                        ang = (asin(normal.GetY())/PIOVER180) ;
                        if ( ang < minAng )
                        {
                            a = 0.0 ;
                        }
                        else
                        {
                            visible = true ;
                            if ( ang > maxAng )
                            {
                                a = 1.0 ;
                            }
                            else
                            {
                                a = (ang - minAng)/(maxAng-minAng) ;
                            }
                        }                   
                        h = GetAltura(j+1,i+1) ;
                        if ( h > maxH )
                        {
                            a = 0.0 ;
                        }
                        else
                        {
                            visibleH = true ;
                            if ( a != 1.0 )
                            {                                
                                if ( h > minH )
                                {
                                    a *= (maxH-h)/(maxH-minH);
                                }
                            }
                        }
                        
                        // Textura
                        interleavedElements[numElemento].texture[0] = (j+1)/4.0 ;
                        interleavedElements[numElemento].texture[1] = (i+1)/4.0 ;
                        
                        // Color
                        interleavedElements[numElemento].color[0] = 1.0 ;
                        interleavedElements[numElemento].color[1] = 1.0 ;
                        interleavedElements[numElemento].color[2] = 1.0 ;
                        interleavedElements[numElemento].color[3] = a ;
                        
                        // Normal
                        interleavedElements[numElemento].normal[0] = normal.GetX() ;
                        interleavedElements[numElemento].normal[1] = normal.GetY() ;
                        interleavedElements[numElemento].normal[2] = normal.GetZ() ;
                        
                        // Vertice
                        interleavedElements[numElemento].vertex[0] = (j+1)*2.0 ;
                        interleavedElements[numElemento].vertex[1] = GetAltura(j+1,i+1)/10.0 ;
                        interleavedElements[numElemento].vertex[2] = (i+1)*2.0 ;                        

                        numElemento++ ;
                        
                        // Tercer Vertice --------------------------------------
                        normal = normales[(i+1)*w + j  ];
                        ang = (asin(normal.GetY())/PIOVER180) ;
                        if ( ang < minAng )
                        {
                            a = 0.0 ;
                        }
                        else
                        {
                            visible = true ;
                            if ( ang > maxAng )
                            {
                                a = 1.0 ;
                            }
                            else
                            {
                                a = (ang - minAng)/(maxAng-minAng) ;
                            }
                        }         
                        h = GetAltura(j  ,i+1) ;
                        if ( h > maxH )
                        {
                            a = 0.0 ;
                        }
                        else
                        {
                            visibleH = true ;
                            if ( a != 1.0 )
                            {                                
                                if ( h > minH )
                                {
                                    a *= (maxH-h)/(maxH-minH);
                                }
                            }
                        }
                        
                        // Textura
                        interleavedElements[numElemento].texture[0] = (j  )/4.0 ;
                        interleavedElements[numElemento].texture[1] = (i+1)/4.0 ;
                        
                        // Color
                        interleavedElements[numElemento].color[0] = 1.0 ;
                        interleavedElements[numElemento].color[1] = 1.0 ;
                        interleavedElements[numElemento].color[2] = 1.0 ;
                        interleavedElements[numElemento].color[3] = a ;
                        
                        // Normal
                        interleavedElements[numElemento].normal[0] = normal.GetX() ;
                        interleavedElements[numElemento].normal[1] = normal.GetY() ;
                        interleavedElements[numElemento].normal[2] = normal.GetZ() ;
                        
                        // Vertice
                        interleavedElements[numElemento].vertex[0] = (j  )*2.0 ;
                        interleavedElements[numElemento].vertex[1] = GetAltura(j  ,i+1)/10.0 ;
                        interleavedElements[numElemento].vertex[2] = (i+1)*2.0 ;
                        
                        numElemento++ ;
                        
                        if ( (!visible || !visibleH)&&multiTextura ) numElemento -= 3 ;

                    }
                }
                
                lista = Api::ReservarCallListAlta();
                
                glNewList(lista, GL_COMPILE);
                    glDrawArrays(GL_TRIANGLES, 0, numElemento);
                glEndList();
                
                
                listasTerrenoHierba[(y*col)+x] = lista ;
                
                
                numElemento = 0 ;
                // Capa de nieve
                for ( int i = y*10 ; i < (y+1)*10 && i < h-1 ; i++ )
                {
                    for ( int j = x*10 ; j < (x+1)*10 && j < w-1 ; j++ )
                    {
                        Util::Vector3D normal ;
                        bool visible ;
                        bool visibleH ;
                        float ang ;
                        float a ;
                        float h ;
                              
                        //******************************************************  
                        // Primer triangulo
                        //******************************************************
                        visible = false ;
                        visibleH = false ;
                        
                        // Primer Vertice --------------------------------------
                        normal = normales[(i  )*w + j  ];
                        ang = (asin(normal.GetY())/PIOVER180) ;
                        if ( ang < minAng )
                        {
                            a = 0.0 ;
                        }
                        else
                        {
                            visible = true ;
                            if ( ang > maxAng )
                            {
                                a = 1.0 ;
                            }
                            else
                            {
                                a = (ang - minAng)/(maxAng-minAng) ;
                            }
                        }
                        h = GetAltura(j  ,i  ) ;
                        if ( h < minH )
                        {
                            a = 0.0 ;
                        }
                        else
                        {
                            visibleH = true ;
                            if ( a != 1.0 )
                            {                                
                                if ( h < maxH )
                                {
                                    a *= (h-minH)/(maxH-minH) ;
                                }
                            }
                            else
                            {
                                if ( h < maxH )
                                {
                                    a = (h-minH)/(maxH-minH) ;
                                }
                                else
                                {
                                    a = 1.0 ;
                                }
                            }
                        }
                        
                        // Textura
                        interleavedElements[numElemento].texture[0] = (j  )/4.0 ;
                        interleavedElements[numElemento].texture[1] = (i  )/4.0 ;
                        
                        // Color
                        interleavedElements[numElemento].color[0] = 1.0 ;
                        interleavedElements[numElemento].color[1] = 1.0 ;
                        interleavedElements[numElemento].color[2] = 1.0 ;
                        interleavedElements[numElemento].color[3] = a ;
                        
                        // Normal
                        interleavedElements[numElemento].normal[0] = normal.GetX() ;
                        interleavedElements[numElemento].normal[1] = normal.GetY() ;
                        interleavedElements[numElemento].normal[2] = normal.GetZ() ;
                        
                        // Vertice
                        interleavedElements[numElemento].vertex[0] = (j  )*2.0 ;
                        interleavedElements[numElemento].vertex[1] = GetAltura(j  ,i  )/10.0 ;
                        interleavedElements[numElemento].vertex[2] = (i  )*2.0 ;
                        
                        numElemento++ ;
                        
                        // Segundo Vertice -------------------------------------
                        normal = normales[(i  )*w + j+1];
                        ang = (asin(normal.GetY())/PIOVER180) ;
                        if ( ang < minAng )
                        {
                            a = 0.0 ;
                        }
                        else
                        {
                            visible = true ;
                            if ( ang > maxAng )
                            {
                                a = 1.0 ;
                            }
                            else
                            {
                                a = (ang - minAng)/(maxAng-minAng) ;
                            }
                        }                    
                        h = GetAltura(j+1,i  ) ;
                        if ( h < minH )
                        {
                            a = 0.0 ;
                        }
                        else
                        {
                            visibleH = true ;
                            if ( a != 1.0 )
                            {                                
                                if ( h < maxH )
                                {
                                    a *= (h-minH)/(maxH-minH) ;
                                }
                            }
                            else
                            {
                                if ( h < maxH )
                                {
                                    a = (h-minH)/(maxH-minH) ;
                                }
                                else
                                {
                                    a = 1.0 ;
                                }
                            }
                        }
                        
                        // Textura
                        interleavedElements[numElemento].texture[0] = (j+1)/4.0 ;
                        interleavedElements[numElemento].texture[1] = (i  )/4.0 ;
                        
                        // Color
                        interleavedElements[numElemento].color[0] = 1.0 ;
                        interleavedElements[numElemento].color[1] = 1.0 ;
                        interleavedElements[numElemento].color[2] = 1.0 ;
                        interleavedElements[numElemento].color[3] = a ;
                        
                        // Normal
                        interleavedElements[numElemento].normal[0] = normal.GetX() ;
                        interleavedElements[numElemento].normal[1] = normal.GetY() ;
                        interleavedElements[numElemento].normal[2] = normal.GetZ() ;
                        
                        // Vertice
                        interleavedElements[numElemento].vertex[0] = (j+1)*2.0 ;
                        interleavedElements[numElemento].vertex[1] = GetAltura(j+1,i  )/10.0 ;
                        interleavedElements[numElemento].vertex[2] = (i  )*2.0 ;
                        
                        numElemento++ ;
                        
                        // Tercer Vertice --------------------------------------
                        normal = normales[(i+1)*w + j+1];
                        ang = (asin(normal.GetY())/PIOVER180) ;
                        if ( ang < minAng )
                        {
                            a = 0.0 ;
                        }
                        else
                        {
                            visible = true ;
                            if ( ang > maxAng )
                            {
                                a = 1.0 ;
                            }
                            else
                            {
                                a = (ang - minAng)/(maxAng-minAng) ;
                            }
                        }                   
                        h = GetAltura(j+1,i+1) ;
                        if ( h < minH )
                        {
                            a = 0.0 ;
                        }
                        else
                        {
                            visibleH = true ;
                            if ( a != 1.0 )
                            {                                
                                if ( h < maxH )
                                {
                                    a *= (h-minH)/(maxH-minH) ;
                                }
                            }
                            else
                            {
                                if ( h < maxH )
                                {
                                    a = (h-minH)/(maxH-minH) ;
                                }
                                else
                                {
                                    a = 1.0 ;
                                }
                            }
                        }
                        
                        // Textura
                        interleavedElements[numElemento].texture[0] = (j+1)/4.0 ;
                        interleavedElements[numElemento].texture[1] = (i+1)/4.0 ;
                        
                        // Color
                        interleavedElements[numElemento].color[0] = 1.0 ;
                        interleavedElements[numElemento].color[1] = 1.0 ;
                        interleavedElements[numElemento].color[2] = 1.0 ;
                        interleavedElements[numElemento].color[3] = a ;
                        
                        // Normal
                        interleavedElements[numElemento].normal[0] = normal.GetX() ;
                        interleavedElements[numElemento].normal[1] = normal.GetY() ;
                        interleavedElements[numElemento].normal[2] = normal.GetZ() ;
                        
                        // Vertice
                        interleavedElements[numElemento].vertex[0] = (j+1)*2.0 ;
                        interleavedElements[numElemento].vertex[1] = GetAltura(j+1,i+1)/10.0 ;
                        interleavedElements[numElemento].vertex[2] = (i+1)*2.0 ;
                        
                        numElemento++ ;     

                        if ( !visible || !visibleH ) numElemento -= 3 ;

                        //******************************************************  
                        // Primer triangulo
                        //******************************************************
                        
                        visible = false ;
                        
                        // Primer Vertice --------------------------------------
                        normal = normales[(i  )*w + j  ];
                        ang = (asin(normal.GetY())/PIOVER180) ;
                        if ( ang < minAng )
                        {
                            a = 0.0 ;
                        }
                        else
                        {
                            visible = true ;
                            if ( ang > maxAng )
                            {
                                a = 1.0 ;
                            }
                            else
                            {
                                a = (ang - minAng)/(maxAng-minAng) ;
                            }
                        }                 
                        h = GetAltura(j  ,i  ) ;
                        if ( h < minH )
                        {
                            a = 0.0 ;
                        }
                        else
                        {
                            visibleH = true ;
                            if ( a != 1.0 )
                            {                                
                                if ( h < maxH )
                                {
                                    a *= (h-minH)/(maxH-minH) ;
                                }
                            }
                            else
                            {
                                if ( h < maxH )
                                {
                                    a = (h-minH)/(maxH-minH) ;
                                }
                                else
                                {
                                    a = 1.0 ;
                                }
                            }
                        }
                        
                        // Textura
                        interleavedElements[numElemento].texture[0] = (j  )/4.0 ;
                        interleavedElements[numElemento].texture[1] = (i  )/4.0 ;
                        
                        // Color
                        interleavedElements[numElemento].color[0] = 1.0 ;
                        interleavedElements[numElemento].color[1] = 1.0 ;
                        interleavedElements[numElemento].color[2] = 1.0 ;
                        interleavedElements[numElemento].color[3] = a ;
                        
                        // Normal
                        interleavedElements[numElemento].normal[0] = normal.GetX() ;
                        interleavedElements[numElemento].normal[1] = normal.GetY() ;
                        interleavedElements[numElemento].normal[2] = normal.GetZ() ;
                        
                        // Vertice
                        interleavedElements[numElemento].vertex[0] = (j  )*2.0 ;
                        interleavedElements[numElemento].vertex[1] = GetAltura(j  ,i  )/10.0 ;
                        interleavedElements[numElemento].vertex[2] = (i  )*2.0 ;
                        
                        numElemento++ ;
                        
                        // Segundo Vertice -------------------------------------
                        normal = normales[(i+1)*w + j+1];
                        ang = (asin(normal.GetY())/PIOVER180) ;
                        if ( ang < minAng )
                        {
                            a = 0.0 ;
                        }
                        else
                        {
                            visible = true ;
                            if ( ang > maxAng )
                            {
                                a = 1.0 ;
                            }
                            else
                            {
                                a = (ang - minAng)/(maxAng-minAng) ;
                            }
                        }                   
                        h = GetAltura(j+1,i+1) ;
                        if ( h < minH )
                        {
                            a = 0.0 ;
                        }
                        else
                        {
                            visibleH = true ;
                            if ( a != 1.0 )
                            {                                
                                if ( h < maxH )
                                {
                                    a *= (h-minH)/(maxH-minH) ;
                                }
                            }
                            else
                            {
                                if ( h < maxH )
                                {
                                    a = (h-minH)/(maxH-minH) ;
                                }
                                else
                                {
                                    a = 1.0 ;
                                }
                            }
                        }
                        
                        // Textura
                        interleavedElements[numElemento].texture[0] = (j+1)/4.0 ;
                        interleavedElements[numElemento].texture[1] = (i+1)/4.0 ;
                        
                        // Color
                        interleavedElements[numElemento].color[0] = 1.0 ;
                        interleavedElements[numElemento].color[1] = 1.0 ;
                        interleavedElements[numElemento].color[2] = 1.0 ;
                        interleavedElements[numElemento].color[3] = a ;
                        
                        // Normal
                        interleavedElements[numElemento].normal[0] = normal.GetX() ;
                        interleavedElements[numElemento].normal[1] = normal.GetY() ;
                        interleavedElements[numElemento].normal[2] = normal.GetZ() ;
                        
                        // Vertice
                        interleavedElements[numElemento].vertex[0] = (j+1)*2.0 ;
                        interleavedElements[numElemento].vertex[1] = GetAltura(j+1,i+1)/10.0 ;
                        interleavedElements[numElemento].vertex[2] = (i+1)*2.0 ;                        

                        numElemento++ ;
                        
                        // Tercer Vertice --------------------------------------
                        normal = normales[(i+1)*w + j  ];
                        ang = (asin(normal.GetY())/PIOVER180) ;
                        if ( ang < minAng )
                        {
                            a = 0.0 ;
                        }
                        else
                        {
                            visible = true ;
                            if ( ang > maxAng )
                            {
                                a = 1.0 ;
                            }
                            else
                            {
                                a = (ang - minAng)/(maxAng-minAng) ;
                            }
                        }         
                        h = GetAltura(j  ,i+1) ;
                        if ( h < minH )
                        {
                            a = 0.0 ;
                        }
                        else
                        {
                            visibleH = true ;
                            if ( a != 1.0 )
                            {                                
                                if ( h < maxH )
                                {
                                    a *= (h-minH)/(maxH-minH) ;
                                }
                            }
                            else
                            {
                                if ( h < maxH )
                                {
                                    a = (h-minH)/(maxH-minH) ;
                                }
                                else
                                {
                                    a = 1.0 ;
                                }
                            }
                        }
                        
                        // Textura
                        interleavedElements[numElemento].texture[0] = (j  )/4.0 ;
                        interleavedElements[numElemento].texture[1] = (i+1)/4.0 ;
                        
                        // Color
                        interleavedElements[numElemento].color[0] = 1.0 ;
                        interleavedElements[numElemento].color[1] = 1.0 ;
                        interleavedElements[numElemento].color[2] = 1.0 ;
                        interleavedElements[numElemento].color[3] = a ;
                        
                        // Normal
                        interleavedElements[numElemento].normal[0] = normal.GetX() ;
                        interleavedElements[numElemento].normal[1] = normal.GetY() ;
                        interleavedElements[numElemento].normal[2] = normal.GetZ() ;
                        
                        // Vertice
                        interleavedElements[numElemento].vertex[0] = (j  )*2.0 ;
                        interleavedElements[numElemento].vertex[1] = GetAltura(j  ,i+1)/10.0 ;
                        interleavedElements[numElemento].vertex[2] = (i+1)*2.0 ;
                        
                        numElemento++ ;
                        
                        if ( !visible || !visibleH ) numElemento -= 3 ;

                    }
                }
                
                lista = Api::ReservarCallListAlta();
                
                glNewList(lista, GL_COMPILE);
                    glDrawArrays(GL_TRIANGLES, 0, numElemento);
                glEndList();
                
                
                listasTerrenoNieve[(y*col)+x] = lista ; 
                               
            }
        }
                 
        
        
        
        
        /*
        for ( int y = 0 ; y < fil   ; y++ )
        {
            for ( int x = 0 ; x < col  ; x++ )
            {
                ymin = ymax = GetAltura(x*10,y*10)/10.0 ;
                int lista = Api::ReservarCallList();
                glNewList(lista, GL_COMPILE);
                    //glDisable(GL_DEPTH_TEST);
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                    glEnable(GL_TEXTURE_2D);
                    
                    glColor4f(1,1,1,0.5);   
                    glNormal3f(1,1,1);             

                    for ( int h = 0 ; h < numPlanosHierba ; h++ )
                    {
                        glBindTexture(GL_TEXTURE_2D, texAgua[h]);
                        glBegin(GL_TRIANGLES); 
                        for ( int i = y*10 ; i < (y+1)*10 && i < h-1 ; i++ )
                        {
                            for ( int j = x*10 ; j < (x+1)*10 && j < w-1 ; j++ )
                            {             
                                float temp = GetAltura(j,i)/10.0 ;
                                if ( ymin > temp )
                                {
                                    ymin = temp ;
                                }
                                if ( ymax < temp )
                                {
                                    ymax = temp ;
                                }
                                      
                                // Generamos la hierba

                                float dh = 0.02 ;
                                float zoom = 15.0 ;
                                                        
                                glTexCoord2f(j/zoom, i/zoom);
                                glVertex3f(j*2.0-x*20-10, GetAltura(j,i)/10.0+(dh*h), i*2.0-y*20-10);
                                glTexCoord2f((j+1)/zoom, i/zoom);
                                glVertex3f((j+1)*2.0-x*20-10, GetAltura(j+1,i)/10.0+(dh*h), i*2.0-y*20-10);
                                glTexCoord2f((j+1)/zoom, (i+1)/zoom);
                                glVertex3f((j+1)*2.0-x*20-10, GetAltura(j+1,i+1)/10.0+(dh*h), (i+1)*2.0-y*20-10);
                                
                                glTexCoord2f(j/zoom, i/zoom);
                                glVertex3f(j*2.0-x*20-10, GetAltura(j,i)/10.0+(dh*h), i*2.0-y*20-10);
                                glTexCoord2f((j+1)/zoom, (i+1)/zoom);
                                glVertex3f((j+1)*2.0-x*20-10, GetAltura(j+1,i+1)/10.0+(dh*h), (i+1)*2.0-y*20-10);
                                glTexCoord2f(j/zoom, (i+1)/zoom);
                                glVertex3f((j)*2.0-x*20-10, GetAltura(j,i+1)/10.0+(dh*h), (i+1)*2.0-y*20-10);
                            }
                        }
                        glEnd();
                    }
                    
                    glEnable(GL_DEPTH_TEST);                         
                glEndList();
                
              
                
                // Creamos el modelo
                Modelo *tempModelo = modelos[(y*col)+x] = new Modelo(lista);
                
                // Definimos el bounding box del modelo
                modelos[(y*col)+x]->SetBoundingBox(Util::Vector3D(-10, 0, -10),Util::Vector3D( 12, ymax,  12)); 
                
                // Creamos el objeto
                objetos[(y*col)+x] = new Objeto3D(tempModelo, Api::ReservarCallListAlta()); 
                
                // Situamos el objeto
                objetos[(y*col)+x]->SetPosicion(10+x*20, 0, 10+y*20);   
                
                objetos[(y*col)+x]->SetTransparente(true);
            }
        }  
        */
              
        
        // Pintamos el agua
        
        listaAgua = Api::ReservarCallListAlta();
        glNewList(listaAgua, GL_COMPILE);
        
            glDisable(GL_ALPHA_TEST);
            glEnable(GL_TEXTURE_2D);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBindTexture(GL_TEXTURE_2D, agua->GetTextura());
            
            glColor4f(1.0, 1.0, 1.0, 0.75);
            glNormal3f(0,1,0);
            glBegin(GL_QUADS);                   
                glTexCoord2f(0, 0);
                glVertex3f(  0, 5.01,   0);
        
                glTexCoord2f(100, 0);                    
                glVertex3f(400, 5.01,   0);
        
                glTexCoord2f(100, 100);
                glVertex3f(400, 5.01, 400);
        
                glTexCoord2f(0, 100);
                glVertex3f(  0, 5.01, 400);
            glEnd();
        glEndList();

        alturaAguaInicial = 0.0 ;
        offsetAgua = 0.0 ;
        
        destroy_bitmap(bitmap);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Terreno::Terreno(string)").Combinar(ex);
    }
}

Terreno::~Terreno()
{
    try
    {
        if ( alturas )
        {
            delete[] alturas ;
        }
        
        /*
        for (int i = 0; i < numElementos ; i++ )
        {
            delete modelos[i] ;
            delete objetos[i] ;
        }
        */
        if ( listasTerreno )
        {
            for ( int i = 0 ; i < numElementos ; i++ )
            {
                glDeleteLists( listasTerreno[i], 1 );
            }
            delete[] listasTerreno ;
        }
        if ( listasTerrenoHierba )
        {
            for ( int i = 0 ; i < numElementos ; i++ )
            {
                glDeleteLists( listasTerrenoHierba[i], 1 );
            }
            delete[] listasTerrenoHierba ;
        }
        if ( listasTerrenoNieve )
        {
            for ( int i = 0 ; i < numElementos ; i++ )
            {
                glDeleteLists( listasTerrenoNieve[i], 1 );
            }
            delete[] listasTerrenoNieve ;
        }
        
        glDeleteLists( listaAgua, 1);
        
        if ( clip )
        {
            delete[] clip ;
        }
        
        
        delete agua ;
        delete hierba ; 
        delete rocas ;
        delete nieve ;  
        
        for ( int i = 0 ; i < 8 ; i++ )
        {
            delete nevado[i] ;
        }      
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Terreno::~Terreno()").Combinar(ex);
    }
}

void
Terreno::GenerarTexturasHierba()
{
    BITMAP *bitmap[numPlanosHierba] ;
    for ( int i = 0 ; i < numPlanosHierba ; i++ )
    {
        bitmap[i] = create_bitmap(512,512);
        clear_to_color(bitmap[i], makecol32(255,0,255));
    }
            
    for ( int x = 0 ; x < 128 ; x++ )
    {
        for ( int y = 0 ; y < 128 ; y++ )
        {
            int cx = x*4 + 2;
            int cy = y*4 + 2;
            
            cx += (rand()%5) - 2 ;
            cy += (rand()%5) - 2 ;
            
            int h = rand()%(numPlanosHierba/2) + numPlanosHierba/2;
            
            for ( int k = 0 ; k < numPlanosHierba ; k++ )
            {
                if ( h >= k )
                {
                    putpixel(bitmap[k], cx, cy, makeacol32(0,rand()%128+127,0,255));
                }
            }
        }
    }

    for ( int i = 0 ; i < numPlanosHierba ; i++ )
    {
        texAgua[i] = allegro_gl_make_texture_ex(AGL_TEXTURE_MIPMAP | AGL_TEXTURE_MASKED | AGL_TEXTURE_FLIP | AGL_TEXTURE_RESCALE, bitmap[i], -1);        
    }
}

void
Terreno::Actualizar()
{
    //objetoAgua->SetPosicion( objetoAgua->GetX(), alturaAguaInicial + 0.3*sin(offsetAgua)  , objetoAgua->GetZ() );
    offsetAgua += 0.01 ;
}

void
Terreno::Pintar()
{
    // Comprobamos cambio de estacion
    if ( invierno )
    {
        if ( alphaNevado < 1.0 )
        {
            alphaNevado += 0.001 ;
        }
        if ( alphaNevado > 1.0 )
        {
            alphaNevado = 1.0 ;
        }
    }
    else
    {
        if ( alphaNevado > 0.0 )
        {
            alphaNevado -= 0.001 ;
        }
        if ( alphaNevado < 0.0 )
        {
            alphaNevado = 0.0 ;
        }
    }
    
    GLdouble ox, oy, oz ;
    GLdouble wx, wy, wz ;
    
    GLdouble modelview[16] ;
    GLdouble projectview[16] ;
    GLint viewport[4] ;
    
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projectview);
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    int fil = ((w-1)/10)+1 ;
    int col = ((h-1)/10)+1 ;
    
    for ( int i = 0 ; i < numElementos ; i++ )
    {
        clip[i] = false ;
    }
    
    // Hago clipping
    for ( int x = 0 ; x <= fil ; x++ )
    {
        for ( int y = 0 ; y <= col ; y++ )
        {
            ox = x * 20.0 ;
            oy = GetAltura(x*10,y*10)/10.0 ;
            oz = y * 20.0 ;
            gluProject(ox, oy, oz, modelview, projectview, viewport, &wx, &wy, &wz);
            if ( !(wx > SCREEN_W || wx < 0.0 || wy > SCREEN_H || wy < 0.0 || wz > 1.0 || wz < 0.0) )
            {
                if ( y < col )
                {
                    if ( x > 0 )
                    {
                        clip[y*col+x-1] = true ;
                    }
                    if ( x < col )
                    {
                        clip[y*col+x] = true ;
                    }
                }
                if ( y > 0 )
                {
                    if ( x > 0 )
                    {
                        clip[(y-1)*col+x-1] = true ;
                    }
                    if ( x < col )
                    {
                        clip[(y-1)*col+x] = true ;
                    }
                }
            }
            
            oy = 0.0 ;
            
            gluProject(ox, oy, oz, modelview, projectview, viewport, &wx, &wy, &wz);
            if ( !(wx > SCREEN_W*1.5 || wx < -0.5*SCREEN_W || wy > SCREEN_H*1.5 || wy < -0.5*SCREEN_H || wz > 1.0 || wz < 0.0) )            {
                if ( y < col )
                {
                    if ( x > 0 )
                    {
                        clip[y*col+x-1] = true ;
                    }
                    if ( x < col )
                    {
                        clip[y*col+x] = true ;
                    }
                }
                if ( y > 0 )
                {
                    if ( x > 0 )
                    {
                        clip[(y-1)*col+x-1] = true ;
                    }
                    if ( x < col )
                    {
                        clip[(y-1)*col+x] = true ;
                    }
                }
            }            
            
        }
    }

    
    // Añado las zonas cercanas a la posicion de la camara
    float x, y, z ;
    Api::GetPosicionCamara(x, y, z);
    
    int dx, dz ;
    dx = x/escala ;
    dz = z/escala ;
    
    dx /= 10 ;
    dz /= 10 ;
    
    for ( int x = dx-1 ; x <= dx+1 ; x++ )
    {
        for ( int z = dz-1 ; z <= dz+1 ; z++ )
        {
            int nx, nz ;
            nx = x ;
            nz = z ;
            if ( x < 0 ) nx = 0 ;
            if ( z < 0 ) nz = 0 ;
            if ( x >= col ) nx = col-1 ;
            if ( z >= fil ) nz = fil-1 ;
            clip[nz*col+nx] = true ;
        }
    }
    
    if ( multiTextura )
    {
        // Pintamos la capa de roca
        glDisable(GL_BLEND);
        glBindTexture(GL_TEXTURE_2D, rocas->GetTextura());
        
        for ( int i = 0 ; i < numElementos ; i++ )
        {
            if ( clip[i] )
            {
                glCallList(listasTerreno[i]);
            }
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        // Pintamos la capa de hierba
        if ( alphaNevado != 1.0 )
        {
            if ( alphaNevado != 0.0 )
            {
                int queNevado = (alphaNevado*7.0) ;
                glBindTexture(GL_TEXTURE_2D, nevado[queNevado]->GetTextura());
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, hierba->GetTextura());
            }
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, nevado[7]->GetTextura());
        }
        for ( int i = 0 ; i < numElementos ; i++ )
        {
            if ( clip[i] )
            {
                glCallList(listasTerrenoHierba[i]);
            }
        }
       
        // Pintamos la capa de nieve
        glBindTexture(GL_TEXTURE_2D, nieve->GetTextura());
        for ( int i = 0 ; i < numElementos ; i++ )
        {
            if ( clip[i] )
            {
                glCallList(listasTerrenoNieve[i]);
            }
        }
    }
    else
    {
        
        if ( alphaNevado != 1.0 )
        {
            if ( alphaNevado != 0.0 )
            {
                int queNevado = (alphaNevado*7.0) ;
                glBindTexture(GL_TEXTURE_2D, nevado[queNevado]->GetTextura());
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, hierba->GetTextura());
            }
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, nevado[7]->GetTextura());
        }
            
        glDisable(GL_BLEND);
        for ( int i = 0 ; i < numElementos ; i++ )
        {
            if ( clip[i] )
            {
                glCallList(listasTerrenoHierba[i]);
            }
        }
    }
    
    glEnable(GL_BLEND);
    // Pinto el agua
    glPushMatrix();    
        glTranslatef(0, 0.05*sin(offsetAgua), 0);
        glCallList(listaAgua);
    glPopMatrix();
    
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glDisable(GL_BLEND);
}

float
Terreno::GetAltura(int x, int y)
{
    if ( x < 0 ) x = 0 ;
    if ( y < 0 ) y = 0 ;
    if ( x >= w ) x = w-1 ;
    if ( y >= h ) y = h-1 ;
    return alturas[y*w + x] ;
}

int
Terreno::GetAncho()
{
    return w ;
}

int
Terreno::GetAlto()
{
    return h ;
}

float 
Terreno::GetAlturaSuave(float x, float z)
{ 
    // Primero determinamos a que quad corresponde
    int qx, qz ;
    
    qx = x / escala ;
    qz = z / escala ;
    
    if ( qx < 0 ) qx = 0 ;
    if ( qz < 0 ) qz = 0 ;
    if ( qx >= w-1 ) qx = w-2 ;
    if ( qz >= h-1 ) qz = h-2 ;
    
    // Obtenemos los vertices del quad
    Util::Vector3D v1, v2, v3, v4 ;
    v1 = Util::Vector3D(0.0, GetAltura(qx  , qz  )/10.0, 0.0) ;
    v2 = Util::Vector3D(1.0, GetAltura(qx+1, qz  )/10.0, 0.0) ;
    v3 = Util::Vector3D(1.0, GetAltura(qx+1, qz+1)/10.0, 1.0) ;
    v4 = Util::Vector3D(0.0, GetAltura(qx  , qz+1)/10.0, 1.0) ;
    
    // La triangulizacion del quad es asi:
    // ------
    // |\   |   
    // | \  |
    // |  \ |
    // |   \|
    // ------
    
    // Si se encuentra en el triangulo superior, se usan los vertices 1 2 y 3
    // Si se encuentra en el triangulo inferior, se usan los vertices 1 3 y 4
    // Por lo que si se encuentra en el triangulo inferior haremos 
    // v2 <- v3
    // v3 <- v4
    
    // Para saber en que triangulo está es necesario obtener las coordenadas
    // relativas a la esquina superior izquierda del triangulo
    float rx, rz ;
    rx = x - qx*escala ;
    rz = z - qz*escala ;
    
    // Y escalar dichas coordenadas relativas para tenerlas en el rango homogeneo
    // 0.0-1.0 tanto para el eje x como z.
    rx /= escala ;
    rz /= escala ;
    
    // La coordenada x-z se proyecta verticalemnte sobre el triangulo inferior si
    // y solo si, se cumple que rx < rz
    if ( rx < rz )
    {
        v2 = v3 ;
        v3 = v4 ;   
    }
    
    // Con los cambios de vertices realizados, ahora hay que aplicar la proyección
    // sobre los vertices v1, v2, v3 (representen el triangulo que representen)
    
    // En primer lugar hay que calcular la normal de dicho triangulo, para lo cual
    // realizamos la multiplicacion vectorial de (v2-v1)*(v3-v1) y normalizamos
    // el vector resultante.
    Util::Vector3D normal, vec1, vec2 ;
    vec1 = v2-v1 ;
    vec2 = v3-v1 ;
    normal = vec2.MultVec(vec1);
    normal.Normalizar();
    
    // Si la normal se define (A, B, C), entonces la ecuación del plano del triangulo
    // es Ax + By + Cz - D = 0, donde D se puede obtener sustituyendo un punto.
    // Si se sutituye el punto x=0.0, z=0.0 (esquian superior izquierda, o v1)
    // y=v1.y, se tiene D = Ax +By +Cz = By = (normal.y * v1.y)
    float a, b, c, d ;
    a = normal.GetX();
    b = normal.GetY();
    c = normal.GetZ();
    d = (b * v1.GetY()) ;
    
    // Con la ecuación del plano Ax + By + Cz + D = 0 se puede obtener la coordenada
    // y que le corresponde a un par x-z:
    // By = -Ax -Cz +D  =>  y = (-Ax-Cz+D)/B
    float y ;
    y = (-a*rx - c*rz + d)/b ;
    
    return y ;
}

void
Terreno::SetInvierno(bool invierno)
{
    Terreno::invierno = invierno ;
}

}
