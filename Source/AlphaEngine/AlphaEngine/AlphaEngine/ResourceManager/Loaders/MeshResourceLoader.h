#ifndef MESHRESOURCELOADER_H
#define MESHRESOURCELOADER_H
#include "..\IResourceLoader.h"
#include "assimp/scene.h"
#include "assimp/mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/matrix4x4.h"
#include "../Resources/Raw.h"
class MeshResourceLoader : public IResourceLoader
{
public:
	MeshResourceLoader();
	~MeshResourceLoader();
	string VGetPattern() override;
	bool VLoadResource(string resName, unsigned char*& pBuffer, unsigned int& size) override;
	unsigned int  VGetSize(string resName) override;
};


#endif MESHRESOURCELOADER_H