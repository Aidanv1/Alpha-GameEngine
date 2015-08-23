#pragma once
#include <string>
#include <glm/glm.hpp>
using namespace glm;
using namespace std;

typedef unsigned int PtrOffset;
///*****************************MODEL***********************************
struct FixedString
{
	char m_fixedLengthString[32];
	FixedString()
	{
		for (unsigned int i = 0; i < 32; i++)
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
// -----------------------------------------------------------------------
struct MaterialInfo
{
	FixedString		m_diffuseTexture;
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
// -----------------------------------------------------------------------
struct KeyFrame
{
	float	m_time;
	vec4	m_value;
};
struct AnimationChannel
{
	FixedString m_nodeName;
	int			m_numPosKeys;
	int			m_numRotKeys;
	PtrOffset	m_posKeyFrames;
	PtrOffset	m_rotKeyFrames;

	KeyFrame* PosKeyFrames()
	{
		return (KeyFrame*)((PtrOffset)this + (PtrOffset)m_posKeyFrames);
	}
	KeyFrame* RotKeyFrames()
	{
		return (KeyFrame*)((PtrOffset)this + (PtrOffset)m_rotKeyFrames);
	}
};
struct AnimationInfo
{
	int					m_numChannels;
	PtrOffset			m_channels;

	AnimationChannel* Channels()
	{
		AnimationChannel* ref = (AnimationChannel*)((PtrOffset)this + (PtrOffset)m_channels);
		return ref;
	}
};
// -----------------------------------------------------------------------
struct BoneWeightData
{
	float		m_weight;
};
struct BoneInfo
{
	FixedString			m_parentName;
	FixedString			m_nodeName;
	int					m_numWeights;
	mat4				m_offsetMatrix;
	PtrOffset			m_weightsData;

	BoneWeightData* WeightData()
	{
		BoneWeightData* ref = (BoneWeightData*)((PtrOffset)this + (PtrOffset)m_weightsData);
		return ref;
	}
};
// -----------------------------------------------------------------------
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
	PtrOffset		m_data;
	unsigned int	m_dataSize;
	int				m_numberOfBones;
	bool			m_hasBones;
	PtrOffset		m_bones;

	float* Data()
	{
		float* ref = (float*)((PtrOffset)this + (PtrOffset)m_data);
		return ref;
	}

	BoneInfo* GetBoneInfo()
	{
		BoneInfo* ref = (BoneInfo*)((PtrOffset)this + (PtrOffset)m_bones);
		return ref;
	}

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
		m_dataSize(0),
		m_numberOfBones(0),
		m_hasBones(false),
		m_bones(NULL)
	{
	}
};
// -----------------------------------------------------------------------
struct SizeInfo
{
	int		m_numberOfMeshes;
	int		m_numberOfBones;
	int		m_numberOfAnimations;
	SizeInfo() :
		m_numberOfMeshes(0),
		m_numberOfBones(0),
		m_numberOfAnimations(0)
	{

	}
};
struct Model
{
	SizeInfo		m_sizeInfo;
	MeshInfo*		m_meshArray;
	AnimationInfo*	m_animationInfo;
};
// -----------------------------------------------------------------------
class ModelBufferReader
{
public:
	ModelBufferReader(unsigned char* buffer);
	ModelBufferReader();
	MeshInfo* GetMeshInfoArray(int& size);
	AnimationInfo* GetAnimationInfoArray(int& size);
	BoneInfo* GetBoneInfoArray(int& size);
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