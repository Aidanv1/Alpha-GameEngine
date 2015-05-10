#include "Texture.h"
// -----------------------------------------------------------------------
Texture::Texture(string texFileName) :
m_textureID(0),
m_textureResource(NULL),
m_textureFileName(texFileName)
{
	m_textureResourceManager = GraphicsSystem::Get().GetTextureResourceManager();
}
// -----------------------------------------------------------------------
Texture::~Texture()
{
	TextureHandler::Get().FreeTextureBuffer(m_textureID);
}
// -----------------------------------------------------------------------
bool Texture::LoadResource()
{
	//if no resource currently exists
	if (!m_textureResource)
	{
		//Request load Resource
		Resource* texResource = new Resource(m_textureFileName);
		texResource->RequestLoad();
		m_textureResource = StrongBitmapPtr(texResource);

		if (!m_textureResourceManager->AddResource(m_textureResource))
		{
			return false;
		}
	}
	else if (!m_textureResource->IsLoaded())
	{
		m_textureResource->RequestLoad();
	} 
	else if (!m_textureResource->Buffer())
	{
		return false;
	}
	return true;
	
}
// -----------------------------------------------------------------------
int Texture::LoadTexture()
{
	//first check if texture was already loaded in video memory
	if (m_textureID != -1)
	{
		//if the texture is still valid, return its ID.
		if (TextureHandler::Get().ValidateID(m_textureID, m_textureFileName))
		{
			return m_textureID;
		}
	}
	if (!m_textureResource)
	{
		return -1;
	}
	if (!m_textureResource->IsLoaded())
	{
		return -1;
	}
	if (!m_textureResource->Buffer())
	{
		return -1;
	}
	BmpData* pBmpData = (BmpData*)m_textureResource->Buffer();
	GLuint texBufferID;
	texBufferID = TextureHandler::Get().GetTextureHandle(pBmpData->size, m_textureFileName);			// Request texture handle	
	glBindTexture(GL_TEXTURE_2D, texBufferID);	// Bind that buffer so we can then fill it (in next line)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pBmpData->width, pBmpData->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, pBmpData->pixel_data.get());
	//texture options
	glEnable(GL_TEXTURE_2D);	// Turn on texturing
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);	// Set the preferences
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	m_textureID = texBufferID;	
	return texBufferID;
}
// -----------------------------------------------------------------------