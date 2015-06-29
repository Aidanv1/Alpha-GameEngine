#pragma once
#include "..\IResourceLoader.h"
//========================================================================
class HeightFieldResourceLoader : public IResourceLoader
{
public:
	HeightFieldResourceLoader();
	~HeightFieldResourceLoader();
	virtual string VGetPattern() override { return "hmap"; }
	bool VLoadResource(string resName, unsigned char*& pBuffer, unsigned int& size) override;
	unsigned int  VGetSize(string resName) override;
private:
	float* CreateFloatArray(vector <float>& vertBufferData, unsigned char* pixelData, int& numVertices, float yScale, int height, int width, float& maxHeight);
};
//========================================================================