#ifndef IRESOURCELOADER_H
#define IRESOURCELOADER_H
//temp includes ------
#include <string>
using namespace std;
//------------
//======================================================================
class IResourceLoader
{
public:
	virtual string GetPattern() = 0;
	virtual bool LoadResource(string resName, unsigned char* pBuffer, unsigned int& size) = 0;
	virtual int GetSize(string resName) = 0;
};
#endif
//======================================================================