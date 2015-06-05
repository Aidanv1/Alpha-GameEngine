#include "LightNode.h"
#include "Scene.h"
// -----------------------------------------------------------------------
LightNode::LightNode() :
m_lightProperties(),
m_lightID(0),
m_distanceToCamera(0),
m_shape(NULL)
{

}
// -----------------------------------------------------------------------
LightNode::~LightNode()
{
	SAFE_DELETE(m_shape);
}
// -----------------------------------------------------------------------
void LightNode::SetLightProperties(LightNodeProperties prop)
{
	m_lightProperties = prop;
}
// -----------------------------------------------------------------------
void LightNode::VUpdateNode(Scene* pScene, float deltaMS)
{
	if (m_shape)
	{
		mat4 trans = translate(mat4(1.0f), m_lightProperties.m_positionVector);
		mat4 rot = mat4(1.0);
		if (m_lightProperties.m_lightType != LightType_Point)
		{
			rot = inverse(lookAt(vec3(0.0), m_lightProperties.m_directionVector, vec3(0.0, 1.0, 0.0)));
		}
		m_shape->VTransform(trans, rot);
	}
	//set distance to camera
	mat4 viewMat;
	pScene->GetCamera()->GetViewMatrix(viewMat);
	vec4 pos4 = viewMat*m_nodeProperties.m_toWorld*vec4(m_positionInWorld, 1.0f);
	m_distanceToCamera = length(pos4);
}
// -----------------------------------------------------------------------
bool LightNode::VInitComponent(TiXmlElement* pElement)
{
	TiXmlElement* nextElem = pElement->FirstChildElement();
	//loop through elements
	while (nextElem)
	{
		string val = nextElem->Value();
		if (val == "Properties")
		{
			if (nextElem->Attribute("dirX"))
			{
				float dirX = 0, dirY = 0, dirZ = 0;
				nextElem->QueryFloatAttribute("dirX", &dirX);
				nextElem->QueryFloatAttribute("dirY", &dirY);
				nextElem->QueryFloatAttribute("dirZ", &dirZ);
				m_lightProperties.m_directionVector = normalize(vec3(dirX, dirY, dirZ));
			}

			if (nextElem->Attribute("posX"))
			{
				float posX = 0, posY = 0, posZ = 0;
				nextElem->QueryFloatAttribute("posX", &posX);
				nextElem->QueryFloatAttribute("posY", &posY);
				nextElem->QueryFloatAttribute("posZ", &posZ);
				m_lightProperties.m_positionVector = vec3(posX, posY, posZ);
			}

			if (nextElem->Attribute("intenR"))
			{
				float intenR = 0, intenG = 0, intenB = 0;
				nextElem->QueryFloatAttribute("intenR", &intenR);
				nextElem->QueryFloatAttribute("intenG", &intenG);
				nextElem->QueryFloatAttribute("intenB", &intenB);
				m_lightProperties.m_intensity = vec3(intenR, intenG, intenB);
			}

			if (nextElem->Attribute("attenuation"))
			{
				nextElem->QueryFloatAttribute("attenuation", &m_lightProperties.m_attinuation);				
			}


			if (nextElem->Attribute("ambientR"))
			{
				nextElem->QueryFloatAttribute("ambientR", &m_lightProperties.m_ambient.x);
				nextElem->QueryFloatAttribute("ambientG", &m_lightProperties.m_ambient.y);
				nextElem->QueryFloatAttribute("ambientB", &m_lightProperties.m_ambient.z);
			}

			if (nextElem->Attribute("theta"))
			{
				nextElem->QueryFloatAttribute("theta", &m_lightProperties.m_theta);
				m_lightProperties.m_theta = radians<float>(m_lightProperties.m_theta);
			}

			if (nextElem->Attribute("type"))
			{
				string type = nextElem->Attribute("type");

				if (type == "Point")
				{ 
					m_lightProperties.m_lightType = LightType_Point; 
					m_shape = ALPHA_NEW Sphere(m_lightProperties.m_positionVector, 0.5, 10);
				}
				if (type == "Sun")
				{ 
					m_lightProperties.m_lightType = LightType_Sun; 
					m_shape = ALPHA_NEW Cone(m_lightProperties.m_positionVector, 0.5, 10);
				}
				if (type == "SpotLight")
				{
					m_lightProperties.m_lightType = LightType_SpotLight; 
					m_shape = ALPHA_NEW Cone(m_lightProperties.m_positionVector, 0.5, 1);
				}
			}

		}

		nextElem = nextElem->NextSiblingElement();
	}

	return true;
}