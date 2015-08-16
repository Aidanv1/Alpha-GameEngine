#pragma once
#include "..\IResourceLoader.h"
//========================================================================
class SimpleResourceLoader : public IResourceLoader
{
public:
	SimpleResourceLoader();
	~SimpleResourceLoader();
	string VGetPattern() override = 0;
	bool VLoadResource(string resName, unsigned char*& pBuffer, unsigned int& size) override;
	unsigned int  VGetSize(string resName) override;
};
//========================================================================
//loader for custom model file format
class AMODResourceLoader : public SimpleResourceLoader
{
	string VGetPattern() override { return "amod"; }
};
//========================================================================