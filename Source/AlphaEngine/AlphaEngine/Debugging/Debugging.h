#pragma once
#include "../Window/MessageWindow.h"
// DEBUGGING--------------------------------------------------------------
#if defined(_DEBUG)
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
#define ALPHA_WARNING(str) {} //TODO add functionality
#define ALPHA_LOG(tag, str){}
#else
#define ALPHA_ERROR(str) { ErrorMessageWindow(str, __FUNCTION__, __FILE__, __LINE__); }
#define ALPHA_WARNING(str) do { (void)sizeof(str); } while(0) 
#define ALPHA_LOG(tag, str) do { (void)sizeof(tag); (void)sizeof(str); } while(0) 
#define ALPHA_ASSERT(expr) do { (void)sizeof(expr); } while(0) 
#endif //_DEBUG
// -----------------------------------------------------------------------
