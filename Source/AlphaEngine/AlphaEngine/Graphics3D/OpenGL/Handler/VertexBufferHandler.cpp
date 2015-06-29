#include "VertexBufferHandler.h"
// -----------------------------------------------------------------------
VertexBufferHandler::VertexBufferHandler() :
m_vBufferHandlerMap(),
m_handleLRUList(),
m_sizeTotal(268435456), //256 MB
m_sizeAllocated(0)
{

}
// -----------------------------------------------------------------------
VertexBufferHandler::~VertexBufferHandler()
{

}
// -----------------------------------------------------------------------
VertexBufferHandler& VertexBufferHandler::Get()
{
	static VertexBufferHandler* instance = ALPHA_NEW VertexBufferHandler();
	return *instance;

}
// -----------------------------------------------------------------------
VertexBufferID VertexBufferHandler::GetVertexBufferHandle(int size, string VertexBufferFileName)
{
	Handle vertHandle;

	//if the handle already exists
	for (auto it = m_handleLRUList.begin(); it != m_handleLRUList.end(); it++)
	{
		//if an ID already exists which maps to that filename, then return that
		if ((*it).m_meshFileName == VertexBufferFileName)
		{
			return (*it).m_vertexBufferID;
		}
	}
	//if there is enough space, generate a new buffer
	if (size < (m_sizeTotal - m_sizeAllocated))
	{
		glGenBuffers(1, &(vertHandle.m_vertexBufferID));
		m_sizeAllocated += size;
		vertHandle.m_size = size;
		vertHandle.m_meshFileName = VertexBufferFileName;
		m_handleLRUList.push_front(vertHandle);		
		//map this id to the filename		
		m_vBufferHandlerMap[vertHandle.m_vertexBufferID] = vertHandle;
		return vertHandle.m_vertexBufferID;
	}
	else
	{
		vertHandle.m_vertexBufferID = -1;
		//if there is no space check from the back of the LRU list
		auto it = m_handleLRUList.rbegin();
		while (it != m_handleLRUList.rend())
		{
			//delete VertexBuffer
			m_sizeAllocated -= (*it).m_size;
			//clear that memory
			glDeleteBuffers(1, &((*it).m_vertexBufferID));
			//remove from list and map
			m_handleLRUList.remove(*it);
			m_vBufferHandlerMap.erase((*it).m_vertexBufferID);

			//check if enough space has been freed
			if (size < (m_sizeTotal - m_sizeAllocated))
			{
				glGenBuffers(1, &(vertHandle.m_vertexBufferID));
				m_sizeAllocated += size;
				vertHandle.m_size = size;
				m_handleLRUList.push_front(vertHandle);
				vertHandle.m_meshFileName = VertexBufferFileName;
				//map this id to the handle			
				m_vBufferHandlerMap[vertHandle.m_vertexBufferID] = vertHandle;
				return vertHandle.m_vertexBufferID;
			}
		}
	}		

	if (vertHandle.m_vertexBufferID == -1)
	{
		ALPHA_ERROR("Could not allocate VertexBuffer buffer. VertexBuffer may be too large.");
	}
	return vertHandle.m_vertexBufferID;
}
// -----------------------------------------------------------------------
bool VertexBufferHandler::ValidateID(VertexBufferID VertexBufferID, string VertexBufferFileName)
{
	if (m_vBufferHandlerMap.find(VertexBufferID) == m_vBufferHandlerMap.end())
	{
		return false;
	}
	if (m_vBufferHandlerMap[VertexBufferID].m_meshFileName == VertexBufferFileName)
	{
		return true;
	}
	return false;
}
// -----------------------------------------------------------------------
void VertexBufferHandler::FreeVertexBuffer(VertexBufferID vertexBufferID)
{
	//delete VertexBuffer
	m_sizeAllocated -= m_vBufferHandlerMap[vertexBufferID].m_size;
	m_handleLRUList.remove(m_vBufferHandlerMap[vertexBufferID]);
	m_vBufferHandlerMap.erase(vertexBufferID);

	//clear that memory
	glDeleteBuffers(1, &(m_vBufferHandlerMap[vertexBufferID].m_vertexBufferID));
}
// -----------------------------------------------------------------------