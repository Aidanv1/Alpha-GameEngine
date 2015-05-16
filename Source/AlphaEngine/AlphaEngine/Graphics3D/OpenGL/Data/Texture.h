#ifndef TEXTURE_H
#define TEXTURE_H
#include "..\..\..\AlphaStd.h"
#include <GL/glew.h>
#include "..\..\..\ResourceManager\Resources\Raw.h"
#include "..\..\..\ResourceManager\Resource.h"
#include "Handler/TextureHandler.h"
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
class Texture : public ITexture
{
public:
	Texture(string textureFileName);
	~Texture();
	virtual int VGetTextureID() const override { return m_textureID; }
	bool VLoadResource();
	int VLoadTexture();
private:
	StrongBitmapPtr m_textureResource;
	StrongResourceManagerPtr m_textureResourceManager;
	int m_textureID;
	string m_textureFileName;
};
//========================================================================
class TextureCubeMap : public ITexture
{
public:
	TextureCubeMap(string textureFileName[]);
	~TextureCubeMap();
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
#endif