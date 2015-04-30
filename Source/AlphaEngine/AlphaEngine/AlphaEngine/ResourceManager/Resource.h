#ifndef RESOURCE_H
#define RESOURCE_H

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
	Resource(string name);
	~Resource();
	bool Init(unsigned int ID, shared_ptr<IResourceLoader> loader, ResourceManager* resM);
	void SetName(string name);
	void RequestLoad();
	void Unload();
	bool IsLoaded();
	string GetName();
	unsigned int GetSize() const;
	unsigned int GetID() const;
	unsigned char* Buffer();
private:	
	bool LoadBuffer(unsigned int availableSpace);
	void FreeBuffer();
private:	
	ResourceID m_resID;
	unsigned char* m_pBuffer;
	shared_ptr<IResourceLoader> m_loader;
	unsigned int m_size;
	ResourceManager* m_resManager;
	string m_resName;
	bool m_isLoaded;
};

#endif
//========================================================================
