#ifndef TEXTURE_H
#define TEXTURE_H
#include "..\..\..\AlphaStd.h"
#include <GL/glew.h>
#include "..\..\..\ResourceManager\Resources\Raw.h"
#include "..\..\..\ResourceManager\Resource.h"
#include "Handler/TextureHandler.h"
//========================================================================
class ITexture
{
public:
	virtual int VGetTextureID() const = 0;
};
//========================================================================
typedef shared_ptr<Resource> StrongBitmapPtr;
class Texture : public ITexture
{
	friend class Material;
public:
	Texture(string textureFileName, StrongResourceManagerPtr resManager);
	~Texture();
	virtual int VGetTextureID() const override { return m_textureID; }

private:
	bool LoadResource();
	int LoadTexture();
	StrongBitmapPtr m_textureResource;
	StrongResourceManagerPtr m_pTextureResourceManager;
	int m_textureID;
	string m_textureFileName;
};
//========================================================================
#endif