#ifndef __G_BASEMODELO_H__
#define __G_BASEMODELO_H__

namespace Graficos
{

class BaseModelo
{
public:
    virtual ~BaseModelo();
    virtual void Pintar()=0;
    virtual void PintarBoundingBox()=0;    
}; 
    
}

#endif
