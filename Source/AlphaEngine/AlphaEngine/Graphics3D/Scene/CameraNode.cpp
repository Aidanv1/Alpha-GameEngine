#include "CameraNode.h"
#include "../../Actor/Components/GraphicsComponent.h"
// -----------------------------------------------------------------------
CameraNode::CameraNode() :
m_camParam(),
m_viewMatrix(1.0f),
m_targetNode(NULL),
m_mode(),
m_orbitalRadius(20),
m_orbitalMax(45),
m_orbitalMin(5),
m_frustum(),
m_translationMatrix(1.0f),
m_rotationMatrix(1.0f)
{

}
// -----------------------------------------------------------------------
CameraNode::~CameraNode()
{

}
// -----------------------------------------------------------------------
void CameraNode::SetParameters(float fovy, float aspectR, float nearClip, float farClip)
{
	m_camParam.m_fieldOfView = fovy;
	m_camParam.m_aspectRatio = aspectR;
	m_camParam.m_nearClip = nearClip;
	m_camParam.m_farClip = farClip;
	m_frustum = Frustum(2, 2, m_camParam.m_fieldOfView, m_camParam.m_nearClip, m_camParam.m_farClip);
	m_projectionMatrix = perspective(radians<float>(fovy), aspectR, nearClip, farClip);
}
// -----------------------------------------------------------------------
void CameraNode::SetCameraTarget(SceneNode* targetNode)
{
	m_targetNode = targetNode;
}
// -----------------------------------------------------------------------
void CameraNode::VUpdateNode(Scene* scene, float deltaMs)
{
	switch (m_mode)
	{
	case FlyAround_Mode:
		m_rotationMatrix = rotate(mat4(1.0f), m_rotation.x, vec3(-1.0f, 0.0f, 0.0f));
		m_rotationMatrix = rotate(m_rotationMatrix, m_rotation.y, vec3(0.0f, -1.0f, 0.0f));		
		m_viewMatrix = m_rotationMatrix * translate(mat4(1.0f), - m_nodeProperties.m_toWorld.GetPosition());

		m_translationMatrix = translate(mat4(1.0f), m_nodeProperties.m_toWorld.GetPosition());
		
		break;
	case Orbital_Mode:
		//clamp xaxis rotation
		if (m_rotation.x > half_pi<float>() * 0.98)
		{
			m_rotation.x = half_pi<float>() * 0.98;
		}
		if (m_rotation.x < 0.2)
		{
			m_rotation.x = 0.2;
		}
		mat4 rotMat = rotate(mat4(1.0f), m_rotation.x, vec3(-1.0f, 0.0f, 0.0f));
		vec4 vectorPos = rotMat * vec4(0, 0, 1.0f, 0);
		rotMat = rotate(mat4(1.0f), m_rotation.y, vec3(0.0f, -1.0f, 0.0f));	
		mat4 targetRot;
		m_targetNode->GetRotationInWorld(targetRot);
		vectorPos = targetRot *rotMat * vectorPos;
		vec3 targetPos;
		m_targetNode->GetPositionInWorld(targetPos);
		m_nodeProperties.m_toWorld.SetPosition(vec3(vectorPos) * m_orbitalRadius + targetPos);
		LookAtTarget();
		
		break;
	}	
	Matrix4x4 transform = m_translationMatrix * inverse(m_rotationMatrix);
	m_frustum.VTransform(transform);
	m_frustum.VDraw(vec4(1.0));
}
// -----------------------------------------------------------------------
void CameraNode::SetMode(CameraMode mode)
{
	m_mode = mode;
}
// -----------------------------------------------------------------------
void CameraNode::LookAtTarget()
{
	vec3 targetPos;
	m_targetNode->GetPositionInWorld(targetPos);
	m_viewMatrix = lookAt(m_nodeProperties.m_toWorld.GetPosition(), targetPos, vec3(0.0f, 1.0f, 0.0f));
	m_rotationMatrix = translate(m_viewMatrix, m_nodeProperties.m_toWorld.GetPosition());
	m_translationMatrix = translate(mat4(1.0f), m_nodeProperties.m_toWorld.GetPosition());
}
// -----------------------------------------------------------------------
bool CameraNode::VInitNode()
{
	Register_Listener(EVENT_GRAPHICS_LOOKEVENT, this, &CameraNode::LookEventDelegate);
	return true;
}
// -----------------------------------------------------------------------
void CameraNode::LookEventDelegate(StrongEventPtr event)
{
	LookEvent* lookEvent = dynamic_cast<LookEvent*>(event.get());
	m_rotation.x += lookEvent->GetLookPos().m_dThetaX;
	m_rotation.y += lookEvent->GetLookPos().m_dThetaY;
	vec3 pos;
	pos.x = m_nodeProperties.m_toWorld.GetPosition().x;
	pos.y = m_nodeProperties.m_toWorld.GetPosition().y;
	pos.z = m_nodeProperties.m_toWorld.GetPosition().z;
	pos.x += lookEvent->GetLookPos().m_dX;
	pos.y += lookEvent->GetLookPos().m_dY;
	pos.z += lookEvent->GetLookPos().m_dZ;
	m_nodeProperties.m_toWorld.SetPosition(pos);
	if (m_mode == Orbital_Mode)
	{
		m_orbitalRadius -= lookEvent->GetLookPos().m_dY;
		//clamp radius
		if (m_orbitalRadius > m_orbitalMax)
		{
			m_orbitalRadius = m_orbitalMax;
		}
		if (m_orbitalRadius < m_orbitalMin)
		{
			m_orbitalRadius = m_orbitalMin;
		}
	}
}
// -----------------------------------------------------------------------
bool CameraNode::VConfigureXmlNodeData(TiXmlElement* pElement)
{
	TiXmlElement* nextElem = pElement->FirstChildElement();
	//loop through elements
	while (nextElem)
	{
		string val = nextElem->Value();
		if (val == "Properties")
		{
			string attrib = nextElem->Attribute("mode");
			if (attrib == "Orbital")
			{
				m_mode = Orbital_Mode;
			}
			if (attrib == "FlyAround")
			{
				m_mode = FlyAround_Mode;
			}
			//target
			if (nextElem->Attribute("targetName"))
			{
				attrib = nextElem->Attribute("targetName");
				if (RoleSystem::Get().GetActor(attrib))
				{
					GraphicsComponent* targetComponent = dynamic_cast<GraphicsComponent*>(RoleSystem::Get().GetActor(attrib)->GetComponent("Graphics"));
					SceneNode* targetNode = dynamic_cast<SceneNode*>(targetComponent->GetSceneNode());
					if (targetNode)
					{
						m_targetNode = targetNode;
					}
				}

			}

		}		

		if (val == "Parameters")
		{
			nextElem->QueryFloatAttribute("FieldOfView", &m_camParam.m_fieldOfView);
			nextElem->QueryFloatAttribute("AspectRatio", &m_camParam.m_aspectRatio);
			nextElem->QueryFloatAttribute("NearClip", &m_camParam.m_nearClip);
			nextElem->QueryFloatAttribute("FarClip", &m_camParam.m_farClip);
		}
		nextElem = nextElem->NextSiblingElement();
	}
	if (m_targetNode)
	{
		m_orbitalMin = m_targetNode->GetRadius();
		if (m_orbitalMax <= m_orbitalMin)
		{
			m_orbitalMax = 2 * m_orbitalMin;
		}
		if (m_orbitalRadius <= m_orbitalMin)
		{
			m_orbitalRadius = 1.2 * m_orbitalMin;
		}
	}
	else
	{
		m_mode = FlyAround_Mode;
	}
	m_frustum = Frustum(1, 1 * m_camParam.m_aspectRatio, radians<float>(m_camParam.m_fieldOfView), m_camParam.m_nearClip, m_camParam.m_farClip);
	m_projectionMatrix = perspective(radians<float>(m_camParam.m_fieldOfView), m_camParam.m_aspectRatio, m_camParam.m_nearClip, m_camParam.m_farClip);	
	return true;
}
// -----------------------------------------------------------------------
