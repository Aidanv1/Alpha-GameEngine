#include "ResourceManager.h"
// -----------------------------------------------------------------------
ResourceManager::ResourceManager() :
m_sizeAllocated(0),
m_sizeTotal(0),
m_resourceListLRU(),
m_resourceLoaderList(),
m_resIDCount(1)
{

}
// -----------------------------------------------------------------------
ResourceManager::~ResourceManager()
{
	m_resourceLoaderList.clear();
	m_resourceListLRU.clear();
}
// -----------------------------------------------------------------------
bool ResourceManager::Init(int sizeInMB)
{
	m_sizeTotal = sizeInMB;
	//add default loader
	DefaultResourceLoader* defaultLoader = new DefaultResourceLoader;
	shared_ptr<DefaultResourceLoader> spDefaultLoader(defaultLoader);
	m_resourceLoaderList.push_back(spDefaultLoader);
	return true;
}
// -----------------------------------------------------------------------

void ResourceManager::AddResourceLoader(shared_ptr<IResourceLoader> spResLoader)
{
	m_resourceLoaderList.push_back(spResLoader);
}
// -----------------------------------------------------------------------
bool ResourceManager::AddResource(shared_ptr<Resource> spResource)
{
	//get resource type and intialiaze with appropriate loader
	//spResource.Init(m_resIDCount++, );

	shared_ptr<IResourceLoader> pResLoader =  m_resourceLoaderList.back();
	spResource->Init(m_resIDCount++, pResLoader, this);
	if (!spResource)
	{
		return false;
	}
	m_resourceListLRU.push_front(spResource);
	return true;
}
// -----------------------------------------------------------------------
void ResourceManager::FreeOneResource()
{
	list <shared_ptr<Resource>>::iterator lastResource = m_resourceListLRU.end();
	(*lastResource)->SetLoaded(false);
}
// -----------------------------------------------------------------------

void ResourceManager::Update()
{
	//load and free appropriate resources
	list <shared_ptr<Resource>>::iterator resIt = m_resourceListLRU.begin();
	for (resIt; resIt != m_resourceListLRU.end(); resIt++)
	{
		if ((*resIt)->IsLoaded())
		{
			if (!(*resIt)->LoadBuffer(m_sizeTotal - m_sizeAllocated))
			{
				//ERROR
				(*resIt)->SetLoaded(false);
			}
		}
		else
		{
			(*resIt)->FreeBuffer();
		}
	}
}
// -----------------------------------------------------------------------
void ResourceManager::Promote(shared_ptr<Resource> spRes)
{
	//move resource to the front of the list
	shared_ptr<Resource> spTempResPtr = spRes;
	m_resourceListLRU.remove(spRes);
	m_resourceListLRU.push_front(spRes);
}
// -----------------------------------------------------------------------
