#include "Resource.h"
// -----------------------------------------------------------------------
Resource::Resource(string name) :
m_resName(name),
m_resID(0),
m_pBuffer(NULL),
m_isLoaded(false),
m_loader(NULL),
m_size(0)
{

}
// -----------------------------------------------------------------------
Resource::~Resource()
{

}
// -----------------------------------------------------------------------
bool Resource::Init(unsigned int ID, shared_ptr<IResourceLoader> loader, ResourceManager* resM)
{
	m_resID = ID;
	m_loader = loader;
	if (!m_loader)
	{
		return false;
	}
	m_resManager = resM;
	if (!m_resManager)
	{
		return false;
	}
	m_isLoaded = true;
	return true;

}
// -----------------------------------------------------------------------
void Resource::SetName(string name)
{
	m_resName = name;
}
// -----------------------------------------------------------------------
void Resource::SetLoaded(bool loaded)
{
	m_isLoaded = loaded;
}
// -----------------------------------------------------------------------
string Resource::GetName()
{
	return m_resName;
}
// -----------------------------------------------------------------------
unsigned int Resource::GetID()
{
	return m_resID;
}
// -----------------------------------------------------------------------
unsigned char* Resource::Buffer()
{
	//promote this resource in the resource manager
	shared_ptr<Resource> thisRes(this);
	m_resManager->Promote(thisRes);
	return m_pBuffer;
}
// -----------------------------------------------------------------------
unsigned int Resource::GetSize()
{
	return m_size;
}
// -----------------------------------------------------------------------
bool Resource::LoadBuffer(int availableSpace)
{
	if (!m_pBuffer)
	{
		if (availableSpace > m_loader->VGetSize(m_resName))
		{
			m_loader->VLoadResource(m_resName, m_pBuffer, m_size);
			m_isLoaded = true;
		}
		else
		{
			return false;
		}
	}
	return true;
}
// -----------------------------------------------------------------------
void Resource::FreeBuffer()
{
	if (m_pBuffer)
	{
		delete m_pBuffer;
		m_pBuffer = NULL;
	}
}
// -----------------------------------------------------------------------
bool Resource::IsLoaded()
{
	return m_isLoaded;
}
// -----------------------------------------------------------------------