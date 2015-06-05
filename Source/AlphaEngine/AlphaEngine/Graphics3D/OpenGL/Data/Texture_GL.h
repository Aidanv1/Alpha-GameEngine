#pragma once
#include "..\..\..\AlphaStd.h"
#include <GL/glew.h>
#include "..\..\..\ResourceManager\Resources\Raw.h"
#include "..\..\..\ResourceManager\Resource.h"
#include "../Handler/TextureHandler.h"
#include "../../GraphicsSystem.h"
//========================================================================
class ITexture
{
public:
	virtual int VGetTextureID() const = 0;
	virtual bool VLoadResource() = 0;
	virtual int VLoadTexture() = 0;
};
//========================================================================
typedef shared_ptr<Resource> StrongBitmapPtr;
typedef shared_ptr<ITexture> StrongTexturePtr;
//========================================================================
class Texture_GL : public ITexture
{
public:
	Texture_GL(string textureFileName);
	~Texture_GL();
	virtual int VGetTextureID() const override { TextureHandler::Get().ValidateID(m_textureID, m_textureFileName); return m_textureID; }
	bool VLoadResource();
	int VLoadTexture();
private:
	StrongBitmapPtr m_textureResource;
	StrongResourceManagerPtr m_textureResourceManager;
	int m_textureID;
	string m_textureFileName;
};
//========================================================================
class TextureCubeMap_GL : public ITexture
{
public:
	TextureCubeMap_GL(string textureFileName[]);
	~TextureCubeMap_GL();
	virtual int VGetTextureID() const override { return m_textureID; }
	bool VLoadResource();
	int VLoadTexture();
private:
	StrongBitmapPtr m_textureResource[6];
	StrongResourceManagerPtr m_textureResourceManager;
	int m_textureID;
	string m_textureFileName[6];
};
//========================================================================
