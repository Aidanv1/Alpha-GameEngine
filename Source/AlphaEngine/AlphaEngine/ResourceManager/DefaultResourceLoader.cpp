#include "DefaultResourceLoader.h"

DefaultResourceLoader::DefaultResourceLoader()
{

}

DefaultResourceLoader::~DefaultResourceLoader()
{

}

string DefaultResourceLoader::VGetPattern()
{
	return "*";
}
bool DefaultResourceLoader::VLoadResource(string resName, unsigned char*& pBuffer, unsigned int& size)
{
	string error = "Unknown file format: ";
	error.append(resName);
	ALPHA_ERROR(error.c_str());
	return false;
}
unsigned int  DefaultResourceLoader::VGetSize(string resName)
{
	return 0;//change
}