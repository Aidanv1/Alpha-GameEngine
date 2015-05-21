#pragma once
#include "../../AlphaStd.h"
#include "..\IResourceLoader.h"
#include "assimp/scene.h"
#include "assimp/mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/matrix4x4.h"

class MeshResourceLoader : public IResourceLoader
{
public:
	MeshResourceLoader();
	~MeshResourceLoader();
	string VGetPattern() override;
	bool VLoadResource(string resName, unsigned char*& pBuffer, unsigned int& size) override;
	unsigned int  VGetSize(string resName) override;
private:

};

//========================================================================
