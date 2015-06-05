#pragma once

#include "..\AlphaStd.h"

#include "DefaultResourceLoader.h"

class ResourceManager;
typedef shared_ptr<ResourceManager> StrongResourceManagerPtr;
using namespace std;


//forward declaration---------------------------------------------------
class Resource;
typedef string ResourceID;
#include "Resource.h"
//========================================================================
class ResourceManager
{
	static unsigned int s_totalMemoryUsed;
public:
	ResourceManager(string ID);
	~ResourceManager();
	bool Init(int sizeInMB);
	void AddResourceLoader(shared_ptr<IResourceLoader> spResLoader);
	bool AddResource(shared_ptr<Resource>& spResource);
	void FreeOneResource();
	void Update();//update once per regular time frame
	void Promote(Resource* spRes);
private:
	string m_resManagerID;
	//size allocated for this resource manager in MB
	unsigned int m_sizeAllocated; 
	//total size available for this resource manager in MB
	unsigned int m_sizeTotal;
	list <shared_ptr<Resource>> m_resourceListLRU;
	map <ResourceID, shared_ptr<Resource>> m_resIDMap;
	list <shared_ptr<IResourceLoader>> m_resourceLoaderList;
	unsigned int m_resIDCount;

};
//========================================================================
