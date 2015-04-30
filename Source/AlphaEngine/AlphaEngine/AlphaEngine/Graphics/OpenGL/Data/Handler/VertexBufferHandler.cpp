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
	static VertexBufferHandler* instance = new VertexBufferHandler();
	return *instance;

}
// -----------------------------------------------------------------------
VertexBufferID VertexBufferHandler::GetVertexBufferHandle(int size, string VertexBufferFileName)
{
	Handle texHandle;
	//if there is enough space, generate a new buffer
	if (size < (m_sizeTotal - m_sizeAllocated))
	{
		glGenBuffers(1, &(texHandle.m_vertexBufferID));
		m_sizeAllocated += size;
		texHandle.m_size = size;
		m_handleLRUList.push_front(texHandle);
		texHandle.m_meshFileName = VertexBufferFileName;
		//map this id to the filename		
		m_vBufferHandlerMap[texHandle.m_vertexBufferID] = texHandle;
		return texHandle.m_vertexBufferID;
	}
	else
	{
		texHandle.m_vertexBufferID = -1;
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
				glGenBuffers(1, &(texHandle.m_vertexBufferID));
				m_sizeAllocated += size;
				texHandle.m_size = size;
				m_handleLRUList.push_front(texHandle);
				texHandle.m_meshFileName = VertexBufferFileName;
				//map this id to the handle			
				m_vBufferHandlerMap[texHandle.m_vertexBufferID] = texHandle;
				return texHandle.m_vertexBufferID;
			}
		}

		if (texHandle.m_vertexBufferID == -1)
		{
			ALPHA_ERROR("Could not allocate VertexBuffer buffer. VertexBuffer may be too large.");
		}
		return texHandle.m_vertexBufferID;
	}


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