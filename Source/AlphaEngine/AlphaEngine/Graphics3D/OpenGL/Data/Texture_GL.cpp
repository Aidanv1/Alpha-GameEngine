#include "Texture_GL.h"
#include <GL/glew.h>
//========================================================================
//Texture
//========================================================================
Texture_GL::Texture_GL(string texFileName) :
m_textureID(0),
m_textureResource(NULL),
m_textureFileName(texFileName)
{
	m_textureResourceManager = GraphicsSystem::Get().GetTextureResourceManager();
}
// -----------------------------------------------------------------------
Texture_GL::~Texture_GL()
{
	TextureHandler::Get().FreeTextureBuffer(m_textureID);
}
// -----------------------------------------------------------------------
bool Texture_GL::VLoadResource()
{
	//if no resource currently exists
	if (!m_textureResource)
	{
		//Request load Resource
		Resource* texResource = ALPHA_NEW Resource(m_textureFileName);
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
int Texture_GL::VLoadTexture()
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
	unsigned char* pBmpData = m_textureResource->Buffer();
	BitMapBufferReader bmpReader(pBmpData);
	BitmapInfo info = bmpReader.GetInfo();

	GLuint texBufferID;
	texBufferID = TextureHandler::Get().GetTextureHandle(info.m_size, m_textureFileName);			// Request texture handle	
	glBindTexture(GL_TEXTURE_2D, texBufferID);	// Bind that buffer so we can then fill it (in next line)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info.m_width, info.m_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bmpReader.PixelData());
	//texture options
	glEnable(GL_TEXTURE_2D);	// Turn on texturing
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);	// Set the preferences
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	m_textureID = texBufferID;	
	return texBufferID;
}
//========================================================================
//Texture Cube Map
//========================================================================

TextureCubeMap_GL::TextureCubeMap_GL(string texFileName[]) :
m_textureID(0),
m_textureResource(),
m_textureFileName()
{
	for (int i = 0; i < 6; i++)
	{
		m_textureResource[i] = NULL;
		m_textureFileName[i] = texFileName[i];
	}
	m_textureResourceManager = GraphicsSystem::Get().GetTextureResourceManager();
}
// -----------------------------------------------------------------------
TextureCubeMap_GL::~TextureCubeMap_GL()
{
	TextureHandler::Get().FreeTextureBuffer(m_textureID);
}
// -----------------------------------------------------------------------
bool TextureCubeMap_GL::VLoadResource()
{
	bool loaded = true;
	//if no resource currently exists
	for (int i = 0; i < 6; i++)
	{
		if (!m_textureResource[i])
		{
			//Request load Resource
			Resource* texResource = ALPHA_NEW Resource(m_textureFileName[i]);
			texResource->RequestLoad();
			m_textureResource[i] = StrongBitmapPtr(texResource);

			if (!m_textureResourceManager->AddResource(m_textureResource[i]))
			{
				return false;
			}
		}
		else if (!m_textureResource[i]->IsLoaded())
		{
			m_textureResource[i]->RequestLoad();
		}
		else if (!m_textureResource[i]->Buffer())
		{
			loaded = false;
		}
	}
	return loaded;

}
// -----------------------------------------------------------------------
int TextureCubeMap_GL::VLoadTexture()
{
	//first check if texture was already loaded in video memory
	if (m_textureID != -1)
	{
		//if the texture is still valid, return its ID.
		if (TextureHandler::Get().ValidateID(m_textureID, m_textureFileName[0]))
		{
			return m_textureID;
		}
	}
	for (int i = 0; i < 6; i++)
	{
		if (!m_textureResource[i])
		{
			return -1;
		}
		if (!m_textureResource[i]->IsLoaded())
		{
			return -1;
		}
		if (!m_textureResource[i]->Buffer())
		{
			return -1;
		}
	}

	BitMapBufferReader bmpReader[6];	
	for (int i = 0; i < 6; i++)
	{
		bmpReader[i] = m_textureResource[i]->Buffer();
	}
	
	GLuint texBufferID;
	texBufferID = TextureHandler::Get().GetTextureHandle(bmpReader[0].GetInfo().m_size * 6, m_textureFileName[0]);			// Request texture handle	


	// Linear filtering for minification and magnification
	glBindTexture(GL_TEXTURE_CUBE_MAP, texBufferID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, bmpReader[0].GetInfo().m_width, bmpReader[0].GetInfo().m_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bmpReader[0].PixelData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, bmpReader[1].GetInfo().m_width, bmpReader[1].GetInfo().m_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bmpReader[1].PixelData());

	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, bmpReader[2].GetInfo().m_width, bmpReader[2].GetInfo().m_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bmpReader[2].PixelData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, bmpReader[3].GetInfo().m_width, bmpReader[3].GetInfo().m_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bmpReader[3].PixelData());

	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, bmpReader[4].GetInfo().m_width, bmpReader[4].GetInfo().m_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bmpReader[4].PixelData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, bmpReader[5].GetInfo().m_width, bmpReader[5].GetInfo().m_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bmpReader[5].PixelData());
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	m_textureID = texBufferID;
	return texBufferID;
}