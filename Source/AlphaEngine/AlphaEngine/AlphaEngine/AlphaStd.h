#ifndef ALPHASTD_H
#define ALPHASTD_H
//VALUE DEFINITIONS
//time
#define MS_PER_SECOND 1000
#define US_PER_SECOND 1000000
#define NS_PER_SECOND 1000000000
#define MS_PER_MICROSECOND 1000
#define IDEAL_FRAME_DT 1/60		//Deltatime for 60 FPS

//storage
#define BYTES_PER_KB 1024
#define BYTES_PER_MB 1024 * 1024

// C RunTime Header Files-------------------------------------------------
#include <stdlib.h>
#include <malloc.h>
#include <memory>
#include <tchar.h>
//#include <mmsystem.h> spams errors??
#include <algorithm>
#include <string>
#include <list>
#include <vector>
#include <queue>
#include <map>
#include <cctype>
#include <clocale>
#include <fstream>

// -----------------------------------------------------------------------
//Global AlphaEngine includes---------------------------------------------
#include "tinyxml.h"
#include "tinystr.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL.h>
#include "Utilities\FileIO.h"
// -----------------------------------------------------------------------
#include "Debugging/Debugging.h"


//NAMESPACES
using namespace std;
using namespace glm;
// MACROS-----------------------------------------------------------------
#if defined(_DEBUG)
#       define ALPHA_NEW new(_NORMAL_BLOCK,__FILE__, __LINE__)
#else
#       define ALPHA_NEW new
#endif



#ifndef SAFE_DELETE
#define SAFE_DELETE(x) if(x) delete x; x = NULL;
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) if (x) delete [] x; x = NULL;
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) if(x) x->Release(); x = NULL;
#endif


#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC__ __FILE__ "("__STR1__(__LINE__)") : Warning Msg: "

// -----------------------------------------------------------------------


#endif

