#include "ResourceManager.h"
// -----------------------------------------------------------------------
ResourceManager::ResourceManager(string ID) :
m_sizeAllocated(0),
m_sizeTotal(0),
m_resourceListLRU(),
m_resourceLoaderList(),
m_resIDCount(1),
m_resManagerID(ID)
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
	m_sizeTotal = sizeInMB * BYTES_PER_MB;
	//add default loader
	DefaultResourceLoader* defaultLoader = new DefaultResourceLoader();
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
	//need to add check to find correct loader
	shared_ptr<IResourceLoader> pResLoader =  m_resourceLoaderList.back();
	
	spResource->Init(m_resIDCount++, pResLoader, this);
	if (!spResource)
	{
		return false;
	}
	m_resourceListLRU.push_front(spResource);
	m_resIDMap[spResource->GetID()] = spResource;
	return true;
}
// -----------------------------------------------------------------------
void ResourceManager::FreeOneResource()
{
	//iterate from the back of the LRU list until a non-empty resource is found
	for (auto it = m_resourceListLRU.rbegin(); it != m_resourceListLRU.rend(); it++)
	{		
		if ((*it)->GetSize() != 0)
		{
			ALPHA_LOG("Resource Manager", "Freeing resource..");
			//unload the resource and set size to 0
			(*it)->Unload();
			m_sizeAllocated -= (*it)->GetSize();
			(*it)->FreeBuffer();
			return;
		}
	}
}
// -----------------------------------------------------------------------

void ResourceManager::Update()
{
	//load and free appropriate resources
	for (auto resIt = m_resourceListLRU.begin(); resIt != m_resourceListLRU.end(); resIt++)
	{
		if ((*resIt)->IsLoaded())
		{
			bool allocated = false;
			//attempt to free resources to make room
			while ((m_sizeTotal - m_sizeAllocated) > 0 &&
					!allocated)
			{
				//if the resource size is zero, then it is currently not loaded
				if ((*resIt)->GetSize() == 0)
				{
					if (!(*resIt)->LoadBuffer(m_sizeTotal - m_sizeAllocated))
					{
						FreeOneResource();
					}
					else
					{
						m_sizeAllocated += (*resIt)->GetSize();
						allocated = true;
					}
				}
				else
				{
					allocated = true;
				}
			}
			if (!allocated)
			{
				ALPHA_ERROR("Resource Manager Error.");
				(*resIt)->Unload();
			}
			
		}
		else
		{
			(*resIt)->FreeBuffer();
		}
	}
}
// -----------------------------------------------------------------------
void ResourceManager::Promote(Resource* spRes)
{
	//move resource to the front of the list
	//shared_ptr<Resource> spTempResPtr (spRes);
	m_resourceListLRU.remove(m_resIDMap[spRes->GetID()]);
	m_resourceListLRU.push_front(m_resIDMap[spRes->GetID()]);
}
// -----------------------------------------------------------------------
