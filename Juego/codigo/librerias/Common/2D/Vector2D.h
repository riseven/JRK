#ifndef S2DVECTOR_H
#define S2DVECTOR_H


#include <math.h>
#include <iosfwd>
#include <fstream>
#include <limits>
#include "misc/utils.h"


class Vector2D
{
	private:
	
  double x;
  double y;

  public:

  Vector2D():x(0.0),y(0.0){}
  Vector2D(double a, double b):x(a),y(b){}

  double getX() const { return x; }
  double getY() const { return y; }
  void setX(double x) { this->x=x;}
  void setY(double y) { this->y=y;}


  //sets x and y to zero
  void Cero(){x=0.0; y=0.0;}

  //returns true if both x and y are zero
  bool esCero()const{return (x*x + y*y) < MinDouble;}

  //returns the length of the vector
  inline double    Longitud()const;

  //returns the squared length of the vector (thereby avoiding the sqrt)
  inline double    LongitudCuad()const;

  inline void      Normalizar();

  double    operator*(const Vector2D& v2) const
  {
  	return ProdEsc(v2);
  }

  inline double    ProdEsc(const Vector2D& v2)const;

  //returns positive if v2 is clockwise of this vector,
  //negative if anticlockwise (assuming the Y axis is pointing down,
  //X axis to right like a Window app)
  inline int       Sign(const Vector2D& v2)const;

  //Vector Perpendicular
  inline Vector2D  Perp()const;

  //Trunca el vector a la distancia máxima, si la supera
  inline void      Truncar(double max);

  //Distancia entre ambos
  inline double    Distancia(const Vector2D &v2)const;

  //Version al cuadrado del anterior
  inline double    DistanciaCuad(const Vector2D &v2)const;

  inline void      Reflejar(const Vector2D& norm);

  //Devuelve el opuesto
  inline Vector2D  GetOpuesto()const;


  //Sobrecargas
  const Vector2D& operator+=(const Vector2D &rhs)
  {
    x += rhs.x;
    y += rhs.y;

    return *this;
  }

  const Vector2D& operator-=(const Vector2D &rhs)
  {
    x -= rhs.x;
    y -= rhs.y;

    return *this;
  }

  const Vector2D& operator*=(const double& rhs)
  {
    x *= rhs;
    y *= rhs;

    return *this;
  }

  const Vector2D& operator/=(const double& rhs)
  {
    x /= rhs;
    y /= rhs;

    return *this;
  }

  bool operator==(const Vector2D& rhs)const
  {
    return (isEqual(x, rhs.x) && isEqual(y,rhs.y) );
  }

  bool operator!=(const Vector2D& rhs)const
  {
    return (x != rhs.x) || (y != rhs.y);
  }
  
};

//-----------------------------------------------------------------------some more operator overloads
inline Vector2D operator*(const Vector2D &lhs, double rhs);
inline Vector2D operator*(double lhs, const Vector2D &rhs);
inline Vector2D operator-(const Vector2D &lhs, const Vector2D &rhs);
inline Vector2D operator+(const Vector2D &lhs, const Vector2D &rhs);
inline Vector2D operator/(const Vector2D &lhs, double val);
std::ostream& operator<<(std::ostream& os, const Vector2D& rhs);
std::ifstream& operator>>(std::ifstream& is, Vector2D& lhs);


//------------------------------------------------------------------------member functions

//Longitud
inline double Vector2D::Longitud()const
{
  return sqrt(x * x + y * y);
}


//Longitud al cuadrado
inline double Vector2D::LongitudCuad()const
{
  return (x * x + y * y);
}


//Producto escalar
//------------------------------------------------------------------------
inline double Vector2D::ProdEsc(const Vector2D &v2)const
{
  return x*v2.x + y*v2.y;
}

//Devuelve 1 si el vector v2 esta más adelantado en el sentido de las agujas del reloj
//Devuelve -1 si esta más atrasado
//(Y axis pointing down, X axis to right)
//------------------------------------------------------------------------
enum {clockwise = 1, anticlockwise = -1};

inline int Vector2D::Sign(const Vector2D& v2)const
{
  if (y*v2.x > x*v2.y)
  { 
    return anticlockwise;
  }
  else 
  {
    return clockwise;
  }
}

//Devuelve el vector perpendicular
inline Vector2D Vector2D::Perp()const
{
  return Vector2D(-y, x);
}

//Devuelve la distancia
inline double Vector2D::Distancia(const Vector2D &v2)const
{
  double ySeparation = v2.y - y;
  double xSeparation = v2.x - x;

  return sqrt(ySeparation*ySeparation + xSeparation*xSeparation);
}


//Devuelve la distancia al cuadrado
inline double Vector2D::DistanciaCuad(const Vector2D &v2)const
{
  double ySeparation = v2.y - y;
  double xSeparation = v2.x - x;

  return ySeparation*ySeparation + xSeparation*xSeparation;
}

//Trunca al tamaño indicado si se pasa
inline void Vector2D::Truncar(double max)
{
  if (this->Longitud() > max)
  {
    this->Normalizar();

    *this *= max;
  } 
}

//Reflejar
inline void Vector2D::Reflejar(const Vector2D& norm)
{
  *this += 2.0 * this->ProdEsc(norm) * norm.GetOpuesto();
}

//Devuelve el vector opuesto
inline Vector2D Vector2D::GetOpuesto()const
{
  return Vector2D(-this->x, -this->y);
}


//Normaliza un vector
inline void Vector2D::Normalizar()
{ 
  double vector_length = this->Longitud();

  if (vector_length > std::numeric_limits<double>::epsilon())
  {
    this->x /= vector_length;
    this->y /= vector_length;
  }
}


//------------------------------------------------------------------------non member functions

inline Vector2D Vec2DNormalizar(const Vector2D &v)
{
  Vector2D vec = v;

  double vector_length = vec.Longitud();

  if (vector_length > std::numeric_limits<double>::epsilon())
  {
    vec.setX(  vec.getX() / vector_length);
    vec.setY(  vec.getY() / vector_length);
  }

  return vec;
}


inline double Vec2DDistancia(const Vector2D &v1, const Vector2D &v2)
{

  double ySeparation = v2.getY() - v1.getY();
  double xSeparation = v2.getX() - v1.getX();

  return sqrt(ySeparation*ySeparation + xSeparation*xSeparation);
}

inline double Vec2DDistanciaCuad(const Vector2D &v1, const Vector2D &v2)
{

  double ySeparation = v2.getY() - v1.getY();
  double xSeparation = v2.getX() - v1.getX();

  return ySeparation*ySeparation + xSeparation*xSeparation;
}

inline double Vec2DLongitud(const Vector2D& v)
{
  return sqrt(v.getX()*v.getX() + v.getY()*v.getY());
}

inline double Vec2DLongitudCuad(const Vector2D& v)
{
  return (v.getX()*v.getX() + v.getY()*v.getY());
}

/*
inline Vector2D POINTStoVector(const POINTS& p)
{
  return Vector2D(p.x, p.y);
}

inline Vector2D POINTtoVector(const POINT& p)
{
  return Vector2D((double)p.x, (double)p.y);
}

inline POINTS VectorToPOINTS(const Vector2D& v)
{
  POINTS p;
  p.x = (short)v.x;
  p.y = (short)v.y;

  return p;
}

inline POINT VectorToPOINT(const Vector2D& v)
{
  POINT p;
  p.x = (long)v.x;
  p.y = (long)v.y;

  return p;
}*/



//------------------------------------------------------------------------operator overloads
inline Vector2D operator*(const Vector2D &lhs, double rhs)
{
  Vector2D result(lhs);
  result *= rhs;
  return result;
}

inline Vector2D operator*(double lhs, const Vector2D &rhs)
{
  Vector2D result(rhs);
  result *= lhs;
  return result;
}

//overload the - operator
inline Vector2D operator-(const Vector2D &lhs, const Vector2D &rhs)
{
  Vector2D result(lhs);
  result.setX( result.getX() - rhs.getX() );
  result.setY( result.getY() - rhs.getY() );
  
  return result;
}

//overload the + operator
inline Vector2D operator+(const Vector2D &lhs, const Vector2D &rhs)
{
  Vector2D result(lhs);
  result.setX( result.getX() + rhs.getX() );
  result.setY( result.getY() + rhs.getY() );
  
  return result;
}

//overload the / operator
inline Vector2D operator/(const Vector2D &lhs, double val)
{
  Vector2D result(lhs);
  result.setX( result.getX() / val );
  result.setY( result.getY() / val );

  return result;
}

///////////////////////////////////////////////////////////////////////////////


//returns true if the point p is not inside the region defined by top_left
//and bot_rgt

inline bool DentroDeRegion(Vector2D p,
                         Vector2D top_left,
                         Vector2D bot_rgt)
{
  return !((p.getX() < top_left.getX()) || (p.getX() > bot_rgt.getX()) || 
         (p.getY() < top_left.getY()) || (p.getY() > bot_rgt.getY()));
}

inline bool DentroDeRegion(Vector2D p, int left, int top, int right, int bottom)
{
  return !( (p.getX() < left) || (p.getX() > right) || (p.getY() < top) || (p.getY() > bottom) );
}

//Devuelve si el segund punto esta en el campo de vision del primero, dando la direccion
//de vista del primero y si angulo de vision
inline bool estaElSegundoEnFOVDelPrimero(Vector2D posFirst,
                                 Vector2D facingFirst,
                                 Vector2D posSecond,
                                 double    fov)
{
  Vector2D toTarget = Vec2DNormalizar(posSecond - posFirst);

  return facingFirst.ProdEsc(toTarget) >= cos(fov/2.0);
}

/*
std::ostream& operator<<(std::ostream& os, const Vector2D& rhs)
{
  os << "(" << rhs.getX() << ", " << rhs.getY() <<")";

  return os;
}
*/
/*  
std::ifstream& operator>>(std::ifstream& is, Vector2D& lhs)
{
  is >> lhs.getX() >> lhs.getY();

  return is;
}
*/

#endif

