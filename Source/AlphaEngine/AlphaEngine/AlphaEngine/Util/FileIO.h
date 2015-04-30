#ifndef FILEIO_H
#define FILEIO_H
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
// -----------------------------------------------------------------------
//return filesize in bytes
inline int FileSize(const char* str)
{	
	struct stat filestatus; 						
	stat(str, &filestatus); 
	return filestatus.st_size; 
} 
// -----------------------------------------------------------------------

#endif