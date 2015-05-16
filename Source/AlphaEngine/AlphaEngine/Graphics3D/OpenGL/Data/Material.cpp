#include "Material.h"
// -----------------------------------------------------------------------
Material::Material() :
m_matComponents(),
m_diffuseTexture(NULL)
{
}
// -----------------------------------------------------------------------
Material::~Material()
{

}
// -----------------------------------------------------------------------
void Material::SetAmbient(Colour& colour)
{
	m_matComponents.ambient = colour;
}
// -----------------------------------------------------------------------
void Material::SetDiffuse(Colour& colour)
{
	m_matComponents.diffuse = colour;
}
// -----------------------------------------------------------------------
void Material::SetSpecular(Colour& colour, float power)
{
	m_matComponents.specular = colour;
	m_matComponents.specpower = power;
}
// -----------------------------------------------------------------------
void Material::SetEmissive(Colour& colour)
{
	m_matComponents.emissive = colour;
}
// -----------------------------------------------------------------------
void Material::SetTexture(StrongTexturePtr pTex)
{
	m_diffuseTexture = pTex;
}
// -----------------------------------------------------------------------
bool Material::LoadTexture()
{
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