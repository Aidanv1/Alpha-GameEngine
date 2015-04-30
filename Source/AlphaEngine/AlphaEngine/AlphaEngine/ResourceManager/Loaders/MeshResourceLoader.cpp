#include "MeshResourceLoader.h"
MeshResourceLoader::MeshResourceLoader()
{

}
MeshResourceLoader::~MeshResourceLoader()
{

}
string MeshResourceLoader::VGetPattern()
{
	return ".obj";
}
bool MeshResourceLoader::VLoadResource(string resName, unsigned char*& pBuffer, unsigned int& size)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(resName.c_str(), NULL);
	if (!scene) {
		//printf("Unable to laod mesh: %s\n", importer.GetErrorString());
		ALPHA_ERROR("Unable to load mesh");
		return false;
	}
	MeshData* meshData = new MeshData();

	aiMesh* mesh = scene->mMeshes[0];
	meshData->m_numVertices = mesh->mNumFaces * 3;
	//extract vertex data
	if (mesh->HasPositions()) {
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			meshData->m_vArray.push_back(mesh->mVertices[i].x);
			meshData->m_vArray.push_back(mesh->mVertices[i].y);
			meshData->m_vArray.push_back(mesh->mVertices[i].z);
		}
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			meshData->m_nArray.push_back(mesh->mNormals[i].x);
			meshData->m_nArray.push_back(mesh->mNormals[i].y);
			meshData->m_nArray.push_back(mesh->mNormals[i].z);
		}
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			meshData->m_tArray.push_back(mesh->mTextureCoords[0][i].x);
			meshData->m_tArray.push_back(mesh->mTextureCoords[0][i].y);
		}

	}
	size = 8 * meshData->m_numVertices * sizeof(GLfloat) + sizeof(MeshData);
	pBuffer = (unsigned char*)meshData;
	//size = FileSize(resName.c_str());
	aiMesh* test = (aiMesh*)pBuffer;
	return true;
}
unsigned int  MeshResourceLoader::VGetSize(string resName)
{
	//approximate size
	unsigned int  size = FileSize(resName.c_str());
	return size;
}