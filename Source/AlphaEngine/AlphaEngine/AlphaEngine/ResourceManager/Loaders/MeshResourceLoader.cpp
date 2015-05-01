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

//========================================================================
//This is the definition of Assimp's aiScene constructor and destructor
//which were not provided 
aiScene::aiScene() {
	mFlags = 0;
	mRootNode = NULL;
	mNumMeshes = 0;
	mMeshes = NULL;
	mNumMaterials = 0;
	mMaterials = NULL;
	mNumAnimations = 0;
	mAnimations = NULL;
	mNumTextures = 0;
	mTextures = NULL;
	mNumLights = 0;
	mLights = NULL;
	mNumCameras = 0;
	mCameras = NULL;
}
aiScene::~aiScene() {
	delete mRootNode;
	delete mMeshes;
	delete mMaterials;
	delete mAnimations;
	delete mTextures;
	delete mLights;
	delete mCameras;
}


//========================================================================

unsigned int SizeOf(aiScene* scene)
{
	unsigned int size = sizeof(aiScene) +
						_msize(scene->mRootNode) +
						_msize(scene->mMeshes) +
						_msize(scene->mMaterials) +
						_msize(scene->mAnimations) +
						_msize(scene->mTextures) +
						_msize(scene->mLights) + 
						_msize(scene->mCameras);
	return size;
}
bool MeshResourceLoader::VLoadResource(string resName, unsigned char*& pBuffer, unsigned int& size)
{
	size = FileSize(resName.c_str());
	pBuffer = (unsigned char*)ReadFile(resName.c_str());
	return true;
}
unsigned int  MeshResourceLoader::VGetSize(string resName)
{
	//approximate size
	unsigned int  size = FileSize(resName.c_str());
	return size;
}