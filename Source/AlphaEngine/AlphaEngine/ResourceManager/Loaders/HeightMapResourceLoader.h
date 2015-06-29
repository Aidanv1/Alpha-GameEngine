#pragma once
#include "..\IResourceLoader.h"
//========================================================================
class HeightMapResourceLoader : public IResourceLoader
{
public:
	HeightMapResourceLoader();
	~HeightMapResourceLoader();
	virtual string VGetPattern() override { return "hmap"; }
	bool VLoadResource(string resName, unsigned char*& pBuffer, unsigned int& size) override;
	unsigned int  VGetSize(string resName) override;
private:
	float* CreateFloatArray(vector <float>& vertBufferData, unsigned char* pixelData, int& numVertices, float xScale, float yScale, float zScale, float tileScale, int height, int width);
};
//========================================================================