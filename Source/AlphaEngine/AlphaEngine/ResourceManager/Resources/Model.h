#pragma once
#include "../../AlphaStd.h"
///*****************************MODEL***********************************
struct TextureName
{
	char m_fixedLengthString[32];
	TextureName()
	{
		for (unsigned int i = 0 ; i < 32; i++)
		{
			m_fixedLengthString[i] = ' ';
		}
	}

	string ToString()
	{
		string temp = m_fixedLengthString;
		return temp;
	}
	void operator=(const char* charArray)
	{
		string temp = charArray;
		for (unsigned int i = 0; i < temp.length(); i++)
		{
			m_fixedLengthString[i] = temp[i];
		}
		m_fixedLengthString[temp.length()] = '\0';
		for (unsigned int i = temp.length() + 1; i < 32; i++)
		{
			m_fixedLengthString[i] = ' ';
		}
	}
};
struct MaterialInfo
{
	TextureName		m_diffuseTexture;
	vec4			m_ambient;
	vec4			m_diffuse;
	vec4			m_specular;
	float			m_shininess;
	vec4			m_emissive;
	MaterialInfo() :
		m_diffuseTexture(),
		m_ambient(1.0),
		m_diffuse(1.0),
		m_specular(0.0),
		m_shininess(1.0),
		m_emissive(0.0)
	{
	}
};
struct MeshInfo
{
	int				m_numberOfVertices;
	int				m_positionComponentSize;
	int				m_normalComponentSize;
	int				m_uvComponentSize;
	MaterialInfo	m_materialInfo;
	mat4			m_transform;
	//hierarchy
	int				m_parentMeshNumber;
	int				m_firstChildMeshNumber;
	int				m_nextSiblingMeshNumber;
	float*			m_data;
	unsigned int	m_dataSize;

	MeshInfo() :
		m_numberOfVertices(0),
		m_positionComponentSize(3),
		m_normalComponentSize(3),
		m_uvComponentSize(2),
		m_materialInfo(),
		m_transform(1.0f),
		m_parentMeshNumber(-1),
		m_firstChildMeshNumber(-1),
		m_nextSiblingMeshNumber(-1),
		m_data(NULL),
		m_dataSize(0)
	{
	}
};
typedef int NumberOfMeshes;
struct Model
{
	NumberOfMeshes	m_numberOfMeshes;
	MeshInfo*		m_meshArray;
};

class ModelBufferReader
{
public:
	ModelBufferReader(unsigned char* buffer);
	ModelBufferReader();
	MeshInfo* GetMeshInfoArray(int& size);
	void operator=(unsigned char* buffer);
private:
	unsigned char* m_pBuffer;
};
///*****************************HeightField*******************************
struct HeightFieldDataInfo
{
	float	m_numVertices;
	float	m_scaleXZ;
	float	m_scaleY;
	float	m_maxHeight;
	float*	m_data;

	HeightFieldDataInfo() :
		m_numVertices(0),
		m_scaleXZ(0),
		m_scaleY(0),
		m_maxHeight(0)
	{

	}
};
//========================================================================