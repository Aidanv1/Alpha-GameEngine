#include "Material_GL.h"
// -----------------------------------------------------------------------
Material_GL::Material_GL() :
m_matComponents(),
m_diffuseTexture(NULL)
{
}
// -----------------------------------------------------------------------
Material_GL::~Material_GL()
{

}
// -----------------------------------------------------------------------
void Material_GL::SetAmbient(Colour& colour)
{
	m_matComponents.ambient = colour;
}
// -----------------------------------------------------------------------
void Material_GL::SetDiffuse(Colour& colour)
{
	m_matComponents.diffuse = colour;
}
// -----------------------------------------------------------------------
void Material_GL::SetSpecular(Colour& colour, float power)
{
	m_matComponents.specular = colour;
	m_matComponents.specpower = power;
}
// -----------------------------------------------------------------------
void Material_GL::SetEmissive(Colour& colour)
{
	m_matComponents.emissive = colour;
}
// -----------------------------------------------------------------------
void Material_GL::SetTexture(StrongTexturePtr pTex)
{
	m_diffuseTexture = pTex;
	m_hasTexture = true;
}
// -----------------------------------------------------------------------
bool Material_GL::LoadTexture()
{
	if (!m_hasTexture)
	{
		return true;
	}

	//this method takes at least two attempts to return true
	if (m_diffuseTexture->VLoadResource())
	{
		int textureID = m_diffuseTexture->VLoadTexture();
		if (textureID != -1)
		{
			return true;
		}
	}
	return false;

}
// -----------------------------------------------------------------------
void Material_GL::SetHasTexture(bool hasTex)
{
	m_hasTexture = hasTex;
}