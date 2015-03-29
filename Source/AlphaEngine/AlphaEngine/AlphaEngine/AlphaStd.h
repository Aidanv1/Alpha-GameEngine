#ifndef ALPHASTD_H
#define ALPHASTD_H

using namespace std;
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
// -----------------------------------------------------------------------
//Global AlphaEngine includes---------------------------------------------
#include "tinyxml.h"
#include "tinystr.h"
// -----------------------------------------------------------------------

// MACROS-----------------------------------------------------------------
#if defined(_DEBUG)
#       define ALPHA_NEW new(_NORMAL_BLOCK,__FILE__, __LINE__)
#else
#       define ALPHA_NEW new
#endif



#if !defined(SAFE_DELETE)
#define SAFE_DELETE(x) if(x) delete x; x = NULL;
#endif

#if !defined(SAFE_DELETE_ARRAY)
#define SAFE_DELETE_ARRAY(x) if (x) delete [] x; x = NULL;
#endif

#if !defined(SAFE_RELEASE)
#define SAFE_RELEASE(x) if(x) x->Release(); x = NULL;
#endif


#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC__ __FILE__ "("__STR1__(__LINE__)") : Warning Msg: "

// -----------------------------------------------------------------------

// DEBUGGING--------------------------------------------------------------
#define ALPHA_ASSERT(expr){}//TODO: ADD DEFINITION
#define ALPHA_ERROR(str){}
#define ALPHA_LOG(tag, str){}
// -----------------------------------------------------------------------
#endif

