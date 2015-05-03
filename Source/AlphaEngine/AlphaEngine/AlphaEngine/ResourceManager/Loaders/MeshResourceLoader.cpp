#include "MeshResourceLoader.h"
MeshResourceLoader::MeshResourceLoader()
{

}
MeshResourceLoader::~MeshResourceLoader()
{

}
string MeshResourceLoader::VGetPattern()
{
	return ".obj";
}
bool MeshResourceLoader::VLoadResource(string resName, unsigned char*& pBuffer, unsigned int& size)
{
	size = FileSize(resName.c_str());
	pBuffer = (unsigned char*)ReadFile(resName.c_str());
	return true;
}
unsigned int  MeshResourceLoader::VGetSize(string resName)
{
	//approximate size
	unsigned int  size = FileSize(resName.c_str());
	return size;
}