#ifndef __AUTOMATA_OFENSIVO_H__
#define __AUTOMATA_OFENSIVO_H__

//------------------------------------------------------------------------
//
//  Estados para el autómata de unidades ofensivas humanas
//
//------------------------------------------------------------------------

#include <string>

#include "lEstado.h"
#include "lMensaje.h"

namespace Logica{

class UnidadDinamica;

//------------------------------------------------------------------------
class EstadoGlobal_Ofensivo : public Estado<UnidadDinamica>
{
private:
  
  //Constructor privado, para no crear instancias
  EstadoGlobal_Ofensivo(){}

public:

  //clase singleton (sólo una instancia)
  static EstadoGlobal_Ofensivo* This()
  {
      static EstadoGlobal_Ofensivo instancia;
    
      return &instancia;
  }

  void Entrar(UnidadDinamica*){}

  void Ejecutar(UnidadDinamica* );

  void Salir(UnidadDinamica* ){}

  bool RecibirMensaje(UnidadDinamica*, const Mensaje&);
};


//------------------------------------------------------------------------
class EstadoMover_Ofensivo : public Estado<UnidadDinamica>
{
private:
  
  //Constructor privado, para no crear instancias
  EstadoMover_Ofensivo(){}

public:

  //clase singleton (sólo una instancia)
  static EstadoMover_Ofensivo* This()
  {
      static EstadoMover_Ofensivo instancia;
    
      return &instancia;
  }

  void Entrar(UnidadDinamica*);

  void Ejecutar(UnidadDinamica* );

  void Salir(UnidadDinamica* );

  bool RecibirMensaje(UnidadDinamica*, const Mensaje&) { return false; }
};


//------------------------------------------------------------------------
class EstadoDetener_Ofensivo : public Estado<UnidadDinamica>
{
private:
  
  //Constructor privado, para no crear instancias
  EstadoDetener_Ofensivo(){}

public:

  //clase singleton (sólo una instancia)
  static EstadoDetener_Ofensivo* This()
  {
      static EstadoDetener_Ofensivo instancia;
    
      return &instancia;
  }

  void Entrar(UnidadDinamica*);

  void Ejecutar(UnidadDinamica* );

  void Salir(UnidadDinamica* );

  bool RecibirMensaje(UnidadDinamica*, const Mensaje&) { return false; }
};



//------------------------------------------------------------------------
class EstadoSeguir_Ofensivo : public Estado<UnidadDinamica>
{
private:
  
  //Constructor privado, para no crear instancias
  EstadoSeguir_Ofensivo(){}

public:

  //clase singleton (sólo una instancia)
  static EstadoSeguir_Ofensivo* This()
  {
      static EstadoSeguir_Ofensivo instancia;
    
      return &instancia;
  }

  void Entrar(UnidadDinamica*);

  void Ejecutar(UnidadDinamica* );

  void Salir(UnidadDinamica* );

  bool RecibirMensaje(UnidadDinamica*, const Mensaje&) { return false; }
};

//------------------------------------------------------------------------
class ModoPasivo_Ofensivo : public Estado<UnidadDinamica>
{
private:
  
  //Constructor privado, para no crear instancias
  ModoPasivo_Ofensivo(){}

public:

  //clase singleton (sólo una instancia)
  static ModoPasivo_Ofensivo* This()
  {
      static ModoPasivo_Ofensivo instancia;
    
      return &instancia;
  }

  void Entrar(UnidadDinamica*);

  void Ejecutar(UnidadDinamica* ) {}

  void Salir(UnidadDinamica* );

  bool RecibirMensaje(UnidadDinamica*, const Mensaje&) { return false; }
};


//------------------------------------------------------------------------
class ModoDefensivo_Ofensivo : public Estado<UnidadDinamica>
{
private:
  
  //Constructor privado, para no crear instancias
  ModoDefensivo_Ofensivo(){}

public:

  //clase singleton (sólo una instancia)
  static ModoDefensivo_Ofensivo* This()
  {
      static ModoDefensivo_Ofensivo instancia;
    
      return &instancia;
  }

  void Entrar(UnidadDinamica*);

  void Ejecutar(UnidadDinamica* );

  void Salir(UnidadDinamica* );

  bool RecibirMensaje(UnidadDinamica*, const Mensaje&) { return false; }
};


//------------------------------------------------------------------------
class ModoAgresivo_Ofensivo : public Estado<UnidadDinamica>
{
private:
  
  //Constructor privado, para no crear instancias
  ModoAgresivo_Ofensivo(){}

public:

  //clase singleton (sólo una instancia)
  static ModoAgresivo_Ofensivo* This()
  {
      static ModoAgresivo_Ofensivo instancia;
    
      return &instancia;
  }

  void Entrar(UnidadDinamica*);

  void Ejecutar(UnidadDinamica* );

  void Salir(UnidadDinamica* );

  bool RecibirMensaje(UnidadDinamica*, const Mensaje&) { return false; }
};

}

#endif
