#include "MeshResourceLoader.h"

MeshResourceLoader::MeshResourceLoader()
{

}
// -----------------------------------------------------------------------
MeshResourceLoader::~MeshResourceLoader()
{

}
// -----------------------------------------------------------------------
void MeshResourceLoader::LoadMesh(MeshInfo*& meshInfo, aiMesh* pMesh, aiMaterial* pMaterial, unsigned char* pNextDataBlock)
{
	MaterialInfo matInfo;
	//-------------Create Material-------------------
	aiColor4D color(0.f, 0.f, 0.f, 0.f);
	//Diffuse
	pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	matInfo.m_diffuse = vec4(color.r, color.g, color.b, color.a);
	//Specular
	pMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
	float shininess;
	pMaterial->Get(AI_MATKEY_SHININESS, shininess);
	matInfo.m_shininess = glm::max(shininess, 1.0f);
	matInfo.m_specular = vec4(color.r, color.g, color.b, color.a);
	//Emissive
	pMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
	matInfo.m_emissive = vec4(color.r, color.g, color.b, color.a);
	//Ambient
	pMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
	matInfo.m_ambient = vec4(color.r, color.g, color.b, color.a);
	//Texture
	aiString textureLoc;
	pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &textureLoc);
	matInfo.m_diffuseTexture = textureLoc.C_Str();
	//-----------------------------------------------
	//-------------Create VertexBuffer---------------
	//convert the texture coords to 2D
	vector<float> varray;

	for (unsigned int i = 0; i < pMesh->mNumVertices; ++i)
	{
		varray.push_back(pMesh->mVertices[i].x);
		varray.push_back(pMesh->mVertices[i].y);
		varray.push_back(pMesh->mVertices[i].z);
	}

	for (unsigned int i = 0; i < pMesh->mNumVertices; ++i)
	{
		varray.push_back(pMesh->mNormals[i].x);
		varray.push_back(pMesh->mNormals[i].y);
		varray.push_back(pMesh->mNormals[i].z);
	}


	for (unsigned int i = 0; i < pMesh->mNumVertices; ++i)
	{
		varray.push_back(pMesh->mTextureCoords[0][i].x);
		varray.push_back(pMesh->mTextureCoords[0][i].y);
	}
	int numVertices = pMesh->mNumVertices;


	
	//construct mesh info
	meshInfo->m_materialInfo = matInfo;
	meshInfo->m_numberOfVertices = numVertices;
	meshInfo->m_positionComponentSize = 3;
	meshInfo->m_normalComponentSize = 3;
	meshInfo->m_uvComponentSize = 2;
	meshInfo->m_data = (float*)pNextDataBlock;
	meshInfo->m_dataSize = sizeof(float) * 8 * numVertices;
	memcpy(pNextDataBlock, &varray[0], meshInfo->m_dataSize);	
}
// -----------------------------------------------------------------------
void MeshResourceLoader::LoadModel(const aiScene* scene, aiNode* node, unsigned char* pNextBufferPos, unsigned char* pNextDataBlock)
{
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		//get relative transformation for child
		aiMatrix4x4 m = node->mChildren[i]->mTransformation;
		vec3 T(m.a4, m.b4, m.c4);
		mat4 trans = translate(mat4(1.0f), T);
		

		//if the child has a mesh
		if (node->mChildren[i]->mNumMeshes > 0)
		{
			if (node->mChildren[i]->mNumMeshes >1)
			{
				ALPHA_ERROR("Unusual model structure.");
			}
			for (int j = 0; j < node->mChildren[i]->mNumMeshes; j++)
			{
				unsigned int meshNum = node->mChildren[i]->mMeshes[j];
				MeshInfo* meshInfo = (MeshInfo*)pNextBufferPos;
				meshInfo->m_transform = trans;
				meshInfo->m_parentMeshNumber = -1;
				meshInfo->m_firstChildMeshNumber = -1;
				meshInfo->m_nextSiblingMeshNumber = -1;
				//if the mesh has a parent
				if (node->mNumMeshes > 0)
				{
					meshInfo->m_parentMeshNumber = node->mMeshes[0];
				}
				//if the mesh has siblings
				if (node->mNumChildren > i + 1)
				{
					if (node->mChildren[i + 1]->mNumMeshes > 0)
					{
						meshInfo->m_nextSiblingMeshNumber = node->mChildren[i + 1]->mMeshes[0];
					}
				}
				//if the mesh has siblings
				if (node->mChildren[i]->mNumChildren > 0)
				{
					if (node->mChildren[i]->mChildren[0]->mNumMeshes > 0)
					{
						meshInfo->m_firstChildMeshNumber = node->mChildren[i]->mChildren[0]->mMeshes[0];
					}
				}


				LoadMesh(meshInfo,
					scene->mMeshes[meshNum],
					scene->mMaterials[scene->mMeshes[meshNum]->mMaterialIndex],
					pNextDataBlock);

				pNextDataBlock = pNextDataBlock + meshInfo->m_dataSize;
				pNextBufferPos = pNextBufferPos + sizeof(MeshInfo);
			}
			
		}
		LoadModel(scene, node->mChildren[i], pNextDataBlock, pNextBufferPos);
	}
}
// -----------------------------------------------------------------------
bool MeshResourceLoader::VLoadResource(string resName, unsigned char*& pBuffer, unsigned int& size)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(resName, NULL);
	if (!scene)
	{
		stringstream ss;
		ss << "Assimp load error: " << importer.GetErrorString();
		ALPHA_ERROR(ss.str().c_str());
		return false;
	}
	int numberOfMeshes = scene->mNumMeshes;
	int totalMeshInfoSize = numberOfMeshes * sizeof(MeshInfo);

	int totalNumFloats = 0;
	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		totalNumFloats += 8 * scene->mMeshes[i]->mNumVertices;
	}
	
	int totalBufferSize = totalNumFloats * sizeof(float) + totalMeshInfoSize;

	unsigned char* modelBuffer = ALPHA_NEW unsigned char[totalBufferSize];
	int* pNumMeshes = (int*)modelBuffer;
	*pNumMeshes = numberOfMeshes;

	unsigned char* dataPtr = modelBuffer + totalMeshInfoSize;
	LoadModel(scene, scene->mRootNode, modelBuffer + sizeof(int), dataPtr);
	pBuffer = modelBuffer;
	return true;
}
// -----------------------------------------------------------------------
unsigned int  MeshResourceLoader::VGetSize(string resName)
{
	//approximate size
	unsigned int  size = FileSize(resName.c_str());
	return size;
}