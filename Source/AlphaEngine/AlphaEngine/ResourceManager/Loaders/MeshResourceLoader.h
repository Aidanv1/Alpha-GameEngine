#pragma once
#include "../../AlphaStd.h"
#include "..\IResourceLoader.h"
#include "assimp/scene.h"
#include "assimp/mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/matrix4x4.h"
#include "../Resources/Model.h"
//========================================================================
class MeshResourceLoader : public IResourceLoader
{
public:
	MeshResourceLoader();
	~MeshResourceLoader();
	virtual string VGetPattern() override = 0;
	bool VLoadResource(string resName, unsigned char*& pBuffer, unsigned int& size) override;
	unsigned int  VGetSize(string resName) override;
private:
	void LoadModel(const aiScene* scene, aiNode* node, unsigned char* pNextBufferPos, unsigned char* pNextDataBlock);
	void LoadMesh(MeshInfo*& meshInfo, aiMesh* pMesh, aiMaterial* pMaterial, unsigned char* pNextDataBlock);
};

//========================================================================
class DAEResourceLoader : public MeshResourceLoader
{
	string VGetPattern() override { return "dae"; }
};
//========================================================================
class FBXResourceLoader : public MeshResourceLoader
{
	string VGetPattern() override { return "fbx"; }
};
//========================================================================
class ThreeDSResourceLoader : public MeshResourceLoader
{
	string VGetPattern() override { return "3ds"; }
};