#pragma once
#include "ResourceManager/Resources/Model.h"
#include "assimp/scene.h"
#include "assimp/mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/matrix4x4.h"
#include <map>
//========================================================================
typedef string Node;
typedef map < Node, Node > NodeParentMap;
class MeshResourceLoader
{
public:
	MeshResourceLoader();
	~MeshResourceLoader();
	bool LoadResource(string resName, unsigned char*& pBuffer, unsigned int& size);
private:
	void Error(string message);
	void LoadModel(	const aiScene* scene, 
					aiNode* node, 
					unsigned char* pNextMeshInfoPos,	//Start of meshinfo array in buffer
					unsigned char* pNextDataBlock,		//Start of meshdata in buffer
					unsigned char* pNextBoneInfoPos,	//start of boneinfo in buffer
					unsigned char* pNextBoneDataBlock); //start of bonedata in buffer

	void LoadMesh(	MeshInfo*& meshInfo, 
					aiMesh* pMesh, 
					aiMaterial* pMaterial, 
					unsigned char* pNextDataBlock,			//pointer to next data chunk
					unsigned char*& pNextBoneInfoPos,		//pointer to next bone info element
					unsigned char* pNextBoneDataBlock		//pointer to next bone data chunk
					);	

	//Conversions 
	mat4 ConvertToMat4(aiMatrix4x4& mat);
	vec4 ConvertToVec4(aiVector3D& vec);
	vec4 ConvertToVec4(aiQuaternion& vec);
private:
	NodeParentMap m_nodeParentMap;
};
//========================================================================