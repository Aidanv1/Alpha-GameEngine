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
};
//========================================================================
typedef shared_ptr<Resource> StrongBitmapPtr;
class Texture;
typedef shared_ptr<Texture> StrongTexturePtr;
//========================================================================
class Texture : public ITexture
{
public:
	Texture(string textureFileName);
	~Texture();
	virtual int VGetTextureID() const override { return m_textureID; }
	bool LoadResource();
	int LoadTexture();
private:
	StrongBitmapPtr m_textureResource;
	StrongResourceManagerPtr m_textureResourceManager;
	int m_textureID;
	string m_textureFileName;
};
//========================================================================
#endif