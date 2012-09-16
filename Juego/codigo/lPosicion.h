/*
 * Clase para manejar vectores y sus operaciones básicas
 */
 
#ifndef __LPOSICION_H__
#define __LPOSICION_H__

#include <stdio.h>
#include <math.h>

class Posicion
{
      public:
             
             bool operator == (const Posicion &p) const;
             
             bool operator != (const Posicion &p) const;
             
             Posicion operator + (const Posicion &p) const;
             
             Posicion operator - (const Posicion &p) const;
             
             float operator * (const Posicion &p) const;
             
             float cuadDist() const { return (x*x + y*y); }

             double dist() const { return sqrt(x*x + y*y); }
             
             bool setNulo() { x=0; y=0; }
             
             bool esNulo() const { return (x==0 && y==0); }
             
             void setX(float X) { x=X; }
             void setY(float Y) { y=Y; }
             float getX() const { return x; }
             float getY() const { return y; }

             void imp() { printf("X:%f Y:%f\n",x,y); }
             
      private:        
                      
       float x;
       float y;
};

#endif
