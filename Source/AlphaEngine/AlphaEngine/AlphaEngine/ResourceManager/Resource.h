#ifndef RESOURCE_H
#define RESOURCE_H
//temp includes----
#include "IResourceLoader.h"
#include "ResourceManager.h"
#include <memory>
#include <string>
using namespace std;
//-----------------

//========================================================================
class Resource
{
	//the resource manager needs access to the LoadBuffer and FreeBuffer
	friend class ResourceManager;
public:
	Resource(string name);
	~Resource();
	bool Init(unsigned int ID, shared_ptr<IResourceLoader> loader, ResourceManager* resM);
	void SetName(string name);
	void SetLoaded(bool loaded);
	bool IsLoaded();
	string GetName();
	unsigned int GetSize();
	unsigned int GetID();
	unsigned char* Buffer();
private:
	bool LoadBuffer(int availableSpace);
	void FreeBuffer();
private:	
	unsigned int m_resID;
	unsigned char* m_pBuffer;
	shared_ptr<IResourceLoader> m_loader;
	unsigned int m_size;
	ResourceManager* m_resManager;
	string m_resName;
	bool m_isLoaded;
};

#endif
//========================================================================
