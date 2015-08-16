#include "MeshResourceLoader.h"

MeshResourceLoader::MeshResourceLoader()
{

}
// -----------------------------------------------------------------------
MeshResourceLoader::~MeshResourceLoader()
{

}
// -----------------------------------------------------------------------
void MeshResourceLoader::LoadMesh(MeshInfo*& meshInfo, aiMesh* pMesh, aiMaterial* pMaterial, unsigned char* pNextDataBlock, unsigned char*& pNextBoneInfoPos, unsigned char* pNextBoneDataBlock)
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
	meshInfo->m_data = (float*)pNextDataBlock;
	int count = 0;
	for (unsigned int i = 0; i < pMesh->mNumVertices; ++i)
	{
		meshInfo->m_data[count++] = pMesh->mVertices[i].x;
		meshInfo->m_data[count++] = pMesh->mVertices[i].y;
		meshInfo->m_data[count++] = pMesh->mVertices[i].z;
	}

	for (unsigned int i = 0; i < pMesh->mNumVertices; ++i)
	{
		meshInfo->m_data[count++] = pMesh->mNormals[i].x;
		meshInfo->m_data[count++] = pMesh->mNormals[i].y;
		meshInfo->m_data[count++] = pMesh->mNormals[i].z;
	}

	if (pMesh->HasTextureCoords(0))
	{
		for (unsigned int i = 0; i < pMesh->mNumVertices; ++i)
		{
			meshInfo->m_data[count++] = (pMesh->mTextureCoords[0][i].x);
			meshInfo->m_data[count++] = (pMesh->mTextureCoords[0][i].y);
		}
	}

	int numVertices = pMesh->mNumVertices;


	
	//construct mesh info
	meshInfo->m_materialInfo = matInfo;
	meshInfo->m_numberOfVertices = numVertices;
	meshInfo->m_positionComponentSize = 3;
	meshInfo->m_normalComponentSize = 3;
	if (pMesh->mTextureCoords[0] != NULL)
	{
		meshInfo->m_dataSize = sizeof(float) * 8 * numVertices;
		meshInfo->m_uvComponentSize = 2;
	}
	else
	{
		meshInfo->m_dataSize = sizeof(float) * 6 * numVertices;
		meshInfo->m_uvComponentSize = 0;
	}
	//BONES-------------------------------------------------
	if (pMesh->HasBones() && 
		pMesh->mNumBones > 0)
	{
		meshInfo->m_hasBones = true;
		meshInfo->m_numberOfBones = pMesh->mNumBones;

		BoneInfo* boneArray = (BoneInfo*)pNextBoneInfoPos;
		meshInfo->m_bones = boneArray;
		//increment boneinfo ptr
		pNextBoneInfoPos += meshInfo->m_numberOfBones * sizeof(BoneInfo);
		for (int i = 0; i < meshInfo->m_numberOfBones; i++)
		{
			//name
			boneArray[i].m_nodeName = pMesh->mBones[i]->mName.C_Str();
			//parent name		
			boneArray[i].m_parentName = m_nodeParentMap[boneArray[i].m_nodeName.ToString()].c_str();
			//number of weights
			boneArray[i].m_numWeights = pMesh->mBones[i]->mNumWeights;
			//offset
			boneArray[i].m_offsetMatrix = ConvertToMat4(pMesh->mBones[i]->mOffsetMatrix);
			boneArray[i].m_weightsData = (BoneWeightData*)pNextBoneDataBlock;
			//increment bone data ptr
			if (i != meshInfo->m_numberOfBones)
			{
				pNextBoneDataBlock += numVertices * sizeof(BoneWeightData);
			}		
			//populate weight data
			//initially set all weights to 0
			for (int j = 0; j < numVertices; j++)
			{
				boneArray[i].m_weightsData[j].m_weight = 0;
			}
			for (int j = 0; j < boneArray[i].m_numWeights; j++)
			{
				int vertexIndex = pMesh->mBones[i]->mWeights[j].mVertexId;
				boneArray[i].m_weightsData[vertexIndex].m_weight = pMesh->mBones[i]->mWeights[vertexIndex].mWeight;
			}
		}

	}
}
// -----------------------------------------------------------------------
void MeshResourceLoader::LoadModel(	const aiScene* scene, 
									aiNode* node, 
									unsigned char* pNextMeshInfoPos, 
									unsigned char* pNextDataBlock, 
									unsigned char* pNextBoneInfoPos,	
									unsigned char* pNextBoneDataBlock)
{
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		//get relative transformation for child
		mat4 trans = ConvertToMat4(node->mChildren[i]->mTransformation);
		
		m_nodeParentMap[node->mChildren[i]->mName.C_Str()] = node->mName.C_Str();
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
				MeshInfo* meshInfo = (MeshInfo*)pNextMeshInfoPos;
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

				int boneDataOffset = 0;
				LoadMesh(	meshInfo,
							scene->mMeshes[meshNum],
							scene->mMaterials[scene->mMeshes[meshNum]->mMaterialIndex],
							pNextDataBlock, 
							pNextBoneInfoPos, 
							pNextBoneDataBlock);

				pNextDataBlock = pNextDataBlock + meshInfo->m_dataSize;
				pNextMeshInfoPos = pNextMeshInfoPos + sizeof(MeshInfo);
			}
			
		}
		LoadModel(scene, node->mChildren[i], pNextDataBlock, pNextMeshInfoPos, pNextBoneInfoPos, pNextBoneDataBlock);
	}
}
// -----------------------------------------------------------------------
bool MeshResourceLoader::VLoadResource(string resName, unsigned char*& pBuffer, unsigned int& size)
{
	Assimp::Importer importer;
	//Load Scene
	const aiScene* scene = importer.ReadFile(resName, NULL);
	if (!scene)
	{
		stringstream ss;
		ss << "Assimp load error: " << importer.GetErrorString();
		ALPHA_ERROR(ss.str().c_str());
		return false;
	}
	//Determine size of buffer required
	int numberOfMeshes = scene->mNumMeshes;
	int totalMeshInfoSize = numberOfMeshes * sizeof(MeshInfo);
	//size of mesh data chunk
	int totalNumFloats = 0;
	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		totalNumFloats += 8 * scene->mMeshes[i]->mNumVertices;
	}
	int meshDataSize = totalNumFloats * sizeof(float);
	//size of bone info and data chunk
	int boneDataSize = 0;
	int numberOfBones = 0;
	int totalBoneInfoSize = 0;
	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		if (scene->mMeshes[i]->HasBones())
		{
			for (int j = 0; j < scene->mMeshes[i]->mNumBones; j++, numberOfBones++)
			{
				//boneDataSize += scene->mMeshes[i]->mBones[j]->mNumWeights * sizeof(BoneWeightData);
				boneDataSize += scene->mMeshes[i]->mNumVertices * sizeof(BoneWeightData);
			}
		}	
	}
	totalBoneInfoSize = numberOfBones * sizeof(BoneInfo);
	//size of animation info and data chunk
	int numberOfAnimations = 0;
	int totalAnimInfoSize = 0;
	int animDataSize = 0;
	int totalChannelSize = 0;
	if (scene->HasAnimations())
	{
		numberOfAnimations = scene->mNumAnimations;
		totalAnimInfoSize = numberOfAnimations * sizeof(AnimationInfo);
		int totalNumChannels = 0;
		for (int i = 0; i < numberOfAnimations; i++)
		{
			totalNumChannels += scene->mAnimations[i]->mNumChannels;
			for (int j = 0; j < scene->mAnimations[i]->mNumChannels; j++)
			{
				int numPosKeys = scene->mAnimations[i]->mChannels[j]->mNumPositionKeys;
				int numRotKeys = scene->mAnimations[i]->mChannels[j]->mNumRotationKeys;
				animDataSize += (numPosKeys + numRotKeys) * sizeof(KeyFrame);
			}
		}
		totalChannelSize = totalNumChannels * sizeof(AnimationChannel);
	}
	
	//total size
	int totalBufferSize =	sizeof(SizeInfo) +
							meshDataSize + totalMeshInfoSize +
							boneDataSize + totalBoneInfoSize +
							animDataSize + totalAnimInfoSize + totalChannelSize;
	//allocate memory buffer
	unsigned char* modelBuffer = ALPHA_NEW unsigned char[totalBufferSize];
	unsigned char* endOfBuffer = modelBuffer + totalBufferSize;//for testing
	//Model size info header
	SizeInfo* pSizes = (SizeInfo*)modelBuffer;
	(*pSizes).m_numberOfMeshes = numberOfMeshes;
	(*pSizes).m_numberOfBones = numberOfBones;
	(*pSizes).m_numberOfAnimations = numberOfAnimations;

	/*
	All Model data is stored in a contiguous block of memory
	divided into a header section and dat section as illustrated below.
	 ___________________________
	|			HEADER			|
	|---------------------------|
	|	Model SizeInfo			|
	|	MeshInfo Array			|
	|	BoneInfo Array			|
	|	AnimationInfo Array		|
	|___________________________|
	|			DATA			|
	|---------------------------|
	|	Mesh Data (vertices)	|
	|	Bone Data (weights)		|
	|	Animation Channels		|
	|	Animation KeyFrames		|
	|___________________________|
	
	
	*/
	//Header
	unsigned char* meshInfoPtr = modelBuffer + sizeof(SizeInfo);					//pointer to the start of the mesh info array
	unsigned char* boneInfoPtr = meshInfoPtr + totalMeshInfoSize;					//pointer to the start of the bone info array
	unsigned char* animInfoPtr = boneInfoPtr + totalBoneInfoSize;					//pointer to the start of the animation info array
	//Data
	unsigned char* dataPtr = animInfoPtr + totalAnimInfoSize;						//pointer to the start of the mesh data in the buffer
	unsigned char* boneDataPtr = dataPtr + meshDataSize;							//pointer to the start of the bone data in the buffer
	unsigned char* animChannelsPtr = boneDataPtr + boneDataSize;					//pointer to the start of the animation channels buffer
	unsigned char* animKeysPtr = animChannelsPtr + totalChannelSize;				//pointer to the start of the animation keys data

	AnimationInfo* animInfoArray = NULL;
	AnimationChannel* animChannelsArray = NULL;
	KeyFrame* animKeysArray = NULL;
	//Load model data (meshes and bones)
	LoadModel(scene, scene->mRootNode, meshInfoPtr, dataPtr, boneInfoPtr, boneDataPtr);
	//Load animations
	int channelCount = 0;
	int keyCount = 0;
	if (scene->HasAnimations())
	{
		animInfoArray = (AnimationInfo*)(animInfoPtr);
		animChannelsArray = (AnimationChannel*)(animChannelsPtr);
		animKeysArray = (KeyFrame*)(animKeysPtr);
		for (int i = 0; i < numberOfAnimations; i++)
		{
			animInfoArray[i].m_numChannels = scene->mAnimations[i]->mNumChannels;
			animInfoArray[i].m_channels = &animChannelsArray[channelCount];
			for (int j = 0; j < scene->mAnimations[i]->mNumChannels; j++)
			{
				animChannelsArray[channelCount].m_numPosKeys = scene->mAnimations[i]->mChannels[j]->mNumPositionKeys;
				animChannelsArray[channelCount].m_numRotKeys = scene->mAnimations[i]->mChannels[j]->mNumRotationKeys;
				animChannelsArray[channelCount].m_nodeName = scene->mAnimations[i]->mChannels[j]->mNodeName.C_Str();
				animChannelsArray[channelCount].m_posKeyFrames = &animKeysArray[keyCount];
				for (int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumPositionKeys; k++)
				{
					animKeysArray[keyCount].m_time = scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mTime;
					animKeysArray[keyCount].m_value = ConvertToVec4(scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue);
					keyCount++;
				}
				animChannelsArray[channelCount].m_rotKeyFrames = &animKeysArray[keyCount];
				for (int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumRotationKeys; k++)
				{
					animKeysArray[keyCount].m_time = scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mTime;
					animKeysArray[keyCount].m_value = ConvertToVec4(scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue);
					keyCount++;
				}
				channelCount++;
			}
		}
	}


	pBuffer = modelBuffer;
	size = totalBufferSize;
	return true;
}
// -----------------------------------------------------------------------
unsigned int  MeshResourceLoader::VGetSize(string resName)
{
	//approximate size
	unsigned int  size = FileSize(resName.c_str());
	return size;
}
// -----------------------------------------------------------------------
mat4 MeshResourceLoader::ConvertToMat4(aiMatrix4x4& mat)
{
	mat4 matrix;
	//matrix[0] = vec4(mat.a1, mat.a2, mat.a3, mat.a4);
	//matrix[1] = vec4(mat.b1, mat.b2, mat.b3, mat.b4);
	//matrix[2] = vec4(mat.c1, mat.c2, mat.c3, mat.c4);
	//matrix[3] = vec4(mat.d1, mat.d2, mat.d3, mat.d4);
	matrix[0] = vec4(mat.a1, mat.b1, mat.c1, mat.d1);
	matrix[1] = vec4(mat.a2, mat.b2, mat.c2, mat.d2);
	matrix[2] = vec4(mat.a3, mat.b3, mat.c3, mat.d3);
	matrix[3] = vec4(mat.a4, mat.b4, mat.c4, mat.d4);
	return matrix;
}
// -----------------------------------------------------------------------
vec4 MeshResourceLoader::ConvertToVec4(aiVector3D& vec)
{
	return vec4(vec.x, vec.y, vec.z, 1.0);
}
// -----------------------------------------------------------------------
vec4 MeshResourceLoader::ConvertToVec4(aiQuaternion& vec)
{
	//return vec4(vec.x, vec.z, vec.y, vec.w);
	return vec4(vec.w,vec.x, vec.y, vec.z);
}