#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
//temp includes----------
#include <list>
#include <map>
#include "Resource.h"
#include "DefaultResourceLoader.h"
#include <memory>
using namespace std;
//-----------------------

//forward declaration---------------------------------------------------
class Resource;
//========================================================================
class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();
	bool Init(int sizeInMB);
	void AddResourceLoader(shared_ptr<IResourceLoader> spResLoader);
	bool AddResource(shared_ptr<Resource> spResource);
	void FreeOneResource();
	void Update();//update once per regular time frame
	void Promote(shared_ptr<Resource> psRes);
private:
	//size allocated for this resource manager in MB
	unsigned int m_sizeAllocated; 
	//total size available for this resource manager in MB
	unsigned int m_sizeTotal;
	list <shared_ptr<Resource>> m_resourceListLRU;
	list <shared_ptr<IResourceLoader>> m_resourceLoaderList;
	unsigned int m_resIDCount;

};
#endif
//========================================================================
