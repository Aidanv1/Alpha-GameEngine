#include "MeshResourceLoader.h"
#include <string>
#include <fstream>
// -----------------------------------------------------------------------

extern "C"
{
	__declspec(dllexport) bool ConvertToAMOD(const char* inputFilePath, const char* outputFilePath)
	{
		MeshResourceLoader* loader = new MeshResourceLoader();
		unsigned char* buffer = NULL;
		unsigned int size = 0;
		bool success = loader->LoadResource(inputFilePath, buffer, size);
		ofstream fOut;
		fOut.open(outputFilePath);
		fOut.write((const char*)&buffer[0], size);
		return success;
	}
}
