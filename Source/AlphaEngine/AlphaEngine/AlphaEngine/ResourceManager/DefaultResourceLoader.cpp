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
bool DefaultResourceLoader::VLoadResource(string resName, unsigned char* pBuffer, unsigned int& size)
{
	return true;
}
int DefaultResourceLoader::VGetSize(string resName)
{
	return 0;//change
}