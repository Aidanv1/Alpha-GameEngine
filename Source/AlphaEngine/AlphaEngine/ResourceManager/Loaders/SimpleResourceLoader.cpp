#include "SimpleResourceLoader.h"
#include <fstream>
// -----------------------------------------------------------------------
SimpleResourceLoader::SimpleResourceLoader()
{

}
// -----------------------------------------------------------------------
SimpleResourceLoader::~SimpleResourceLoader()
{

}
// -----------------------------------------------------------------------
bool SimpleResourceLoader::VLoadResource(string resName, unsigned char*& pBuffer, unsigned int& size)
{
	pBuffer = (unsigned char*)ReadFile(resName.c_str());
	size = VGetSize(resName);
	return true;
}
// -----------------------------------------------------------------------
unsigned int  SimpleResourceLoader::VGetSize(string resName)
{
	//approximate size
	unsigned int  size = FileSize(resName.c_str());
	return size;
}
// -----------------------------------------------------------------------