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
inline char* ReadFile(const char* filename)
{
	// Open the file
	FILE* fp;
	fopen_s(&fp, filename, "r");


	// Move the file pointer to the end of the file and determing the length
	fseek(fp, 0, SEEK_END);
	long file_length = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* contents = new char[file_length + 1];
	// zero out memory
	for (int i = 0; i < file_length + 1; i++) {
		contents[i] = 0;
	}
	// Here's the actual read
	fread(contents, 1, file_length, fp);
	// This is how you denote the end of a string in C
	contents[file_length + 1] = '\0';
	fclose(fp);
	return contents;
}
#endif