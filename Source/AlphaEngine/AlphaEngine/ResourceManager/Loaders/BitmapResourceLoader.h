#ifndef BITMAP_RESOURCE_LOADER_H
#define BITMAP_RESOURCE_LOADER_H
#include "..\IResourceLoader.h"
#include "..\Resources\Raw.h"
#include "IL/il.h"
#include "IL/ilu.h"

#include <IL/ilut.h>
//========================================================================
class BitmapResourceLoader : public IResourceLoader
{
public:
	BitmapResourceLoader();
	~BitmapResourceLoader();
	string VGetPattern() override;
	bool VLoadResource(string resName, unsigned char*& pBuffer, unsigned int& size) override;
	unsigned int  VGetSize(string resName) override;
};
//========================================================================
#endif