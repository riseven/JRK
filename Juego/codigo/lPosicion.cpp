#include "lPosicion.h"

bool
Posicion::operator == (const Posicion &p) const
{
        return (x==p.x && y==p.y);
}
             

bool
Posicion::operator != (const Posicion &p) const
{
        return !((*this)==p);
}
             
Posicion
Posicion::operator + (const Posicion &p) const
{
        Posicion ret;
        ret.x = p.x + this->x;
        ret.y = p.y + this->y;
                      
        return ret;
}
             
Posicion
Posicion::operator - (const Posicion &p) const
{
        Posicion ret;
        ret.x = this->x - p.x;
        ret.y = this->y - p.y;
                      
        return ret;
}
             
float
Posicion::operator * (const Posicion &p) const
{
        return (p.x*x + p.y*y);
}
