#ifndef DEFAULTRESOURCELOADER_H
#define DEFAULTRESOURCELOADER_H
#include "IResourceLoader.h"
//========================================================================
class DefaultResourceLoader : public IResourceLoader
{
public:
	DefaultResourceLoader();
	~DefaultResourceLoader();
	virtual string VGetPattern();
	virtual bool VLoadResource(string resName, unsigned char* pBuffer, unsigned int& size);
	virtual int VGetSize(string resName);
};
#endif
//========================================================================
