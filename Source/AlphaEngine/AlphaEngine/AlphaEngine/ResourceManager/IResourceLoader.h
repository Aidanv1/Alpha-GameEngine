#ifndef IRESOURCELOADER_H
#define IRESOURCELOADER_H
//temp includes ------
#include <string>
using namespace std;
//------------
//========================================================================
class IResourceLoader
{
public:
	virtual string VGetPattern() = 0;
	virtual bool VLoadResource(string resName, unsigned char* pBuffer, unsigned int& size) = 0;
	virtual int VGetSize(string resName) = 0;
};
#endif
//========================================================================