
//
// Todas las definiciones específicas de windows aquí
// para que la unica inclusion de <windows.h> sea en este fichero y no 
// haya conflictos con allegro
//

#include <windows.h>
#include "uWinDefs.h"

namespace Util{

//Funcion de tiempo de vista al juego
long GetTiempoActual()
{
       return GetTickCount();
}

}



