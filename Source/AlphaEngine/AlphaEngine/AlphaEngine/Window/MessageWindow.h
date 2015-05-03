#ifndef MESSAGEWINDOW_H
#define MESSAGEWINDOW_H
#include <sstream>
#include "SDL.h"
#include <intrin.h>
using namespace std;


inline void ErrorMessageWindow(const char* str1, const char* function, const char* file, long line )
{
	stringstream ss;
	ss << str1 << "\nFunction:" <<function<<"\nFile: "<<file<<"\nLine:"<<line<<"\nClick OK to debug";
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", ss.str().c_str(), NULL);	
}

inline void AssertMessageWindow(const char* str1, const char* function, const char* file, long line)
{
	stringstream ss;
	ss << str1 << "\nFunction:" << function << "\nFile: " << file << "\nLine:" << line << "\nClick OK to debug";
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Assert", ss.str().c_str(), NULL);
}
#endif