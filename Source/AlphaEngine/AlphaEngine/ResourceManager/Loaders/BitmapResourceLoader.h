#pragma once
#include "..\IResourceLoader.h"
//========================================================================
class BitmapResourceLoader : public IResourceLoader
{
public:
	BitmapResourceLoader();
	~BitmapResourceLoader();
	virtual string VGetPattern() override = 0;
	bool VLoadResource(string resName, unsigned char*& pBuffer, unsigned int& size) override;
	unsigned int  VGetSize(string resName) override;
};
//========================================================================
class JPGResourceLoader : public BitmapResourceLoader
{
	string VGetPattern() override { return "jpg"; }
};
//========================================================================
class PNGResourceLoader : public BitmapResourceLoader
{
	string VGetPattern() override { return "png"; }
};
//========================================================================
class TGAResourceLoader : public BitmapResourceLoader
{
	string VGetPattern() override { return "tga"; }
};
//========================================================================
class BMPResourceLoader : public BitmapResourceLoader
{
	string VGetPattern() override { return "bmp"; }
};
//========================================================================
