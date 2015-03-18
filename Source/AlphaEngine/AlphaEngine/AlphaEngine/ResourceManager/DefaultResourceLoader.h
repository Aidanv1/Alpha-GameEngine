#ifndef DEFAULTRESOURCELOADER_H
#define DEFAULTRESOURCELOADER_H
#include "IResourceLoader.h"
//======================================================================
class DefaultResourceLoader : public IResourceLoader
{
public:
	DefaultResourceLoader();
	~DefaultResourceLoader();
	virtual string GetPattern();
	virtual bool LoadResource(string resName, unsigned char* pBuffer, unsigned int& size);
	virtual int GetSize(string resName);
};
#endif
//======================================================================