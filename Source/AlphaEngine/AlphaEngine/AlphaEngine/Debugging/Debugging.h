#ifndef DEBUGGING_H
#define DEBUGGING_H
// DEBUGGING--------------------------------------------------------------
#if defined(_DEBUG)
#include "../Window/MessageWindow.h"
#define ALPHA_ASSERT(expr)\
		{ \
			if(!expr)\
			{\
				AssertMessageWindow(#expr, __FUNCTION__, __FILE__, __LINE__); \
				__debugbreak(); \
			}\
		}
#define ALPHA_ERROR(str)\
		{ \
			ErrorMessageWindow(str, __FUNCTION__, __FILE__, __LINE__);\
			__debugbreak(); \
		}
#define GCC_WARNING(str) {} //TODO add functionality
#define ALPHA_LOG(tag, str){}
#else
#define GCC_ERROR(str) do { (void)sizeof(str); } while(0) 
#define GCC_WARNING(str) do { (void)sizeof(str); } while(0) 
#define GCC_LOG(tag, str) do { (void)sizeof(tag); (void)sizeof(str); } while(0) 
#define GCC_ASSERT(expr) do { (void)sizeof(expr); } while(0) 
#endif //_DEBUG
// -----------------------------------------------------------------------
#endif