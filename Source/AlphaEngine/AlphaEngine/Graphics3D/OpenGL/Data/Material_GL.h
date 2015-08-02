#pragma once
#include "..\..\..\AlphaStd.h"
#include "Texture_GL.h"
typedef vec4 Colour;
class Material_GL;
typedef shared_ptr<Material_GL> StrongMaterialPtr;
//========================================================================
class Material_GL
{
public:
	Material_GL();
	~Material_GL();
	//accessors
	Colour GetAmbient() const { return m_matComponents.ambient; }
	Colour GetDiffuse() const { return m_matComponents.diffuse; }
	void GetSpecular(Colour& colour, float& power) const {	colour = m_matComponents.specular; 
															power = m_matComponents.specpower; }
	Colour GetEmissive() const { return m_matComponents.emissive; }
	StrongTexturePtr Texture() const { return m_diffuseTexture; }	
	//mutators
	void SetAmbient(Colour& colour);
	void SetDiffuse(Colour& colour);
	void SetSpecular(Colour& colour, float power);
	void SetEmissive(Colour& colour);
	void SetTexture(StrongTexturePtr pTex);
	void SetHasTexture(bool hasTex);
	bool HasTexture() { return m_hasTexture; }
	//--
	bool LoadTexture();
private:
	struct MatComponents
	{
	public:
		Colour ambient;
		Colour diffuse;
		Colour specular;
		float specpower;
		Colour emissive;

		MatComponents() :
			ambient(1),
			diffuse(1),
			specular(0),
			specpower(1),
			emissive(0)
		{
		}


	};
	bool				m_hasTexture;
	MatComponents		m_matComponents;
	StrongTexturePtr	m_diffuseTexture;
};
//========================================================================
