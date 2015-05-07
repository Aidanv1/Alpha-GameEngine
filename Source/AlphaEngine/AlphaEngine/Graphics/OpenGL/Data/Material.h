#ifndef MATERIAL_H
#define MATERIAL_H
#include "..\..\..\AlphaStd.h"
#include "Texture.h"
typedef vec4 Colour;
typedef shared_ptr<Texture> StrongTexturePtr;
//========================================================================
class Material
{
public:
	Material();
	~Material();
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
	//--
	bool LoadTexture();
	//16 byte alignment
	//void* operator new(std::size_t sz)
	//{
	//	return _aligned_malloc(sz, 16);
	//}
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
			ambient(0),
			diffuse(0),
			specular(0),
			specpower(0),
			emissive(0)
		{
		}


	};
	MatComponents m_matComponents;
	StrongTexturePtr m_diffuseTexture;
};
//========================================================================
#endif