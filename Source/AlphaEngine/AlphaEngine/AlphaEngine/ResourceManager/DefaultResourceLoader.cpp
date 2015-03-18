#include "DefaultResourceLoader.h"

DefaultResourceLoader::DefaultResourceLoader()
{

}

DefaultResourceLoader::~DefaultResourceLoader()
{

}

string DefaultResourceLoader::GetPattern()
{
	return "*";
}
bool DefaultResourceLoader::LoadResource(string resName, unsigned char* pBuffer, unsigned int& size)
{
	return true;
}
int DefaultResourceLoader::GetSize(string resName)
{
	return 0;//change
}