#include "uVector2D.h"
#include <fstream>

namespace Util{

std::ostream& operator<<(std::ostream& os, const Vector2D& rhs)
{
  os << "(" << rhs.getX() << ", " << rhs.getY() <<")";

  return os;
}

/*  
std::istream& operator>>(std::istream& is, Vector2D& lhs)
{
  is >> lhs.getX() >> lhs.getY();

  return is;
}
*/

}
