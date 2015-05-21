#pragma once

#include "..\AlphaStd.h"
#include "ResourceManager.h"
#include "IResourceLoader.h"

using namespace std;
//-----------------

//========================================================================
class Resource
{
	//the resource manager needs access to the LoadBuffer and FreeBuffer
	friend class ResourceManager;
	typedef shared_ptr<ResourceManager> StrongResourceManagerPtr;
public:
	Resource(ResourceID name);
	~Resource();
	bool Init(unsigned int ID, shared_ptr<IResourceLoader> loader, ResourceManager* resM);
	void RequestLoad();
	void Unload();
	bool IsLoaded();
	unsigned int GetSize() const;
	ResourceID GetID() const;
	unsigned char* Buffer();
private:	
	bool LoadBuffer(unsigned int availableSpace);
	void FreeBuffer();
private:	
	unsigned int m_resID;
	unsigned char* m_pBuffer;
	shared_ptr<IResourceLoader> m_loader;
	unsigned int m_size;
	ResourceManager* m_resManager;
	ResourceID m_resName;
	bool m_isLoaded;
};

//========================================================================
