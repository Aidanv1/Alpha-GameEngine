#pragma once
#include "SceneNode.h"
#include "../../Actor/Components/GraphicsComponent.h"
#include "../Geometry/Geometry.h"
class LightNode;
typedef shared_ptr<LightNode> StrongLightPtr;
typedef unsigned int LightID;
//========================================================================
enum LightType
{
	LightType_Invalid,
	LightType_Sun,
	LightType_Point,
	LightType_SpotLight
};
struct LightNodeProperties
{
	vec3		m_directionVector;
	vec3		m_positionVector;
	float		m_attinuation;
	vec3		m_intensity;
	vec3		m_ambient;
	float		m_theta;
	LightType	m_lightType;	
	LightNodeProperties() :
		m_directionVector(0),
		m_positionVector(0),
		m_attinuation(0),
		m_intensity(1),
		m_ambient(0),
		m_theta(0),
		m_lightType(LightType_Sun)
	{
	}
};
//========================================================================
class LightNode : public SceneNode
{
public:
	LightNode();
	~LightNode();
	void GetLightProperties(LightNodeProperties& prop) const { prop = m_lightProperties; }
	void SetLightProperties(LightNodeProperties prop);
	LightID GetID() const { return m_lightID; }
	float GetDistanceToCamera() const { return m_distanceToCamera; }
	//SceneNode functions
	virtual void VUpdateNode(Scene* pScene, float deltaMS) override;
	virtual bool VConfigureXmlNodeData(TiXmlElement* pElement) override;
private:
	LightNodeProperties m_lightProperties;
	LightID				m_lightID;
	float				m_distanceToCamera;
	IGeometry*			m_shape;
};
//========================================================================