#pragma once
#include "IResourceLoader.h"
//========================================================================
class DefaultResourceLoader : public IResourceLoader
{
public:
	DefaultResourceLoader();
	~DefaultResourceLoader();
	virtual string VGetPattern();
	virtual bool VLoadResource(string resName, unsigned char*& pBuffer, unsigned int& size);
	virtual unsigned int  VGetSize(string resName);
};

//========================================================================
