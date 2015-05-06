#include "TextureHandler.h"
// -----------------------------------------------------------------------
TextureHandler::TextureHandler() :
m_textureHandlerMap(),
m_handleLRUList(),
m_sizeTotal(268435456), //256 MB
m_sizeAllocated(0)
{

}
// -----------------------------------------------------------------------
TextureHandler::~TextureHandler()
{

}
// -----------------------------------------------------------------------
TextureHandler& TextureHandler::Get()
{
	static TextureHandler* instance = new TextureHandler();
	return *instance;

}
// -----------------------------------------------------------------------
TextureID TextureHandler::GetTextureHandle(int size, string textureFileName)
{
	Handle texHandle;
	//if there is enough space, generate a new buffer
	if (size < (m_sizeTotal - m_sizeAllocated))
	{
		glGenTextures(1, &(texHandle.m_textureID));
		m_sizeAllocated += size;
		texHandle.m_size = size;
		m_handleLRUList.push_front(texHandle);
		texHandle.m_textureFileName = textureFileName;
		//map this id to the filename		
		m_textureHandlerMap[texHandle.m_textureID] = texHandle;
		return texHandle.m_textureID;
	}
	else
	{
		texHandle.m_textureID = -1;
		//if there is no space check from the back of the LRU list
		auto it = m_handleLRUList.rbegin();
		while (it != m_handleLRUList.rend())
		{
			//delete texture
			m_sizeAllocated -= (*it).m_size;
			//clear that memory
			glDeleteTextures(1, &((*it).m_textureID));
			//remove from list and map
			m_handleLRUList.remove(*it);
			m_textureHandlerMap.erase((*it).m_textureID);
			
			//check if enough space has been freed
			if (size < (m_sizeTotal - m_sizeAllocated))
			{
				glGenTextures(1, &(texHandle.m_textureID));
				m_sizeAllocated += size;
				texHandle.m_size = size;
				m_handleLRUList.push_front(texHandle);
				texHandle.m_textureFileName = textureFileName;
				//map this id to the handle			
				m_textureHandlerMap[texHandle.m_textureID] = texHandle;
				return texHandle.m_textureID;
			}
		}

		if (texHandle.m_textureID == -1)
		{
			ALPHA_ERROR("Could not allocate texture buffer. Texture may be too large.");
		}
		return texHandle.m_textureID;
	}

	
}
// -----------------------------------------------------------------------
bool TextureHandler::ValidateID(TextureID textureID, string textureFileName)
{
	if (m_textureHandlerMap.find(textureID) == m_textureHandlerMap.end())
	{
		return false;
	}
	if (m_textureHandlerMap[textureID].m_textureFileName == textureFileName)
	{
		return true;
	}
	return false;
}
// -----------------------------------------------------------------------
void TextureHandler::FreeTextureBuffer(TextureID textureID)
{
	//delete texture
	m_sizeAllocated -= m_textureHandlerMap[textureID].m_size;
	m_handleLRUList.remove(m_textureHandlerMap[textureID]);
	m_textureHandlerMap.erase(textureID);	

	//clear that memory
	glDeleteTextures(1, &(m_textureHandlerMap[textureID].m_textureID));
}
// -----------------------------------------------------------------------