#include "Camera.h"

// -----------------------------------------------------------------------
Camera::Camera() :
m_camParam(),
m_viewMatrix(1.0f),
m_targetNode(NULL),
m_mode(),
m_orbitalRadius(10),
m_orbitalMax(20),
m_orbitalMin(0),
m_frustum(),
m_translationMatrix(1.0f),
m_rotationMatrix(1.0f)
{

}
// -----------------------------------------------------------------------
Camera::~Camera()
{

}
// -----------------------------------------------------------------------
void Camera::SetParameters(float fovy, float aspectR, float nearClip, float farClip)
{
	m_camParam.m_fieldOfView = fovy;
	m_camParam.m_aspectRatio = aspectR;
	m_camParam.m_nearClip = nearClip;
	m_camParam.m_farClip = farClip;
	m_frustum = Frustum(2, 2, m_camParam.m_fieldOfView, m_camParam.m_nearClip, m_camParam.m_farClip);
	m_projectionMatrix = perspective(radians<float>(fovy), aspectR, nearClip, farClip);
}
// -----------------------------------------------------------------------
void Camera::SetCameraTarget(SceneNode* targetNode)
{
	m_targetNode = targetNode;
}
// -----------------------------------------------------------------------
void Camera::VUpdateNode(Scene* scene, float deltaMs)
{
	switch (m_mode)
	{
	case FlyAround_Mode:
		m_rotationMatrix = rotate(mat4(1.0f), m_rotation.x, vec3(-1.0f, 0.0f, 0.0f));
		m_rotationMatrix = rotate(m_rotationMatrix, m_rotation.y, vec3(0.0f, -1.0f, 0.0f));		
		m_viewMatrix = m_rotationMatrix * translate(mat4(1.0f), -m_positionInWorld);

		m_translationMatrix = translate(mat4(1.0f), m_positionInWorld);
		
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
		vectorPos = rotMat * vectorPos;
		vec3 targetPos;
		m_targetNode->GetPositionInWorld(targetPos);
		m_positionInWorld = vec3(vectorPos) * m_orbitalRadius + targetPos;
		LookAtTarget();
		
		break;
	}	
	m_frustum.VTransform(m_translationMatrix, inverse(m_rotationMatrix));
}
// -----------------------------------------------------------------------
void Camera::SetMode(CameraMode mode)
{
	m_mode = mode;
}
// -----------------------------------------------------------------------
void Camera::LookAtTarget()
{
	vec3 targetPos;
	m_targetNode->GetPositionInWorld(targetPos);
	m_viewMatrix = lookAt(m_positionInWorld, targetPos, vec3(0.0f, 1.0f, 0.0f));
	m_rotationMatrix = translate(m_viewMatrix, m_positionInWorld);
	m_translationMatrix = translate(mat4(1.0f), m_positionInWorld);
}
// -----------------------------------------------------------------------
bool Camera::VInitNode()
{
	Register_Listener(LookEvent::sEventType,this, &Camera::LookEventDelegate);
	return true;
}
// -----------------------------------------------------------------------
void Camera::LookEventDelegate(StrongEventPtr event)
{
	LookEvent* lookEvent = dynamic_cast<LookEvent*>(event.get());
	m_rotation.x += lookEvent->GetLookPos().m_dThetaX;
	m_rotation.y += lookEvent->GetLookPos().m_dThetaY;
	m_positionInWorld.x += lookEvent->GetLookPos().m_dX;
	m_positionInWorld.y += lookEvent->GetLookPos().m_dY;
	m_positionInWorld.z += lookEvent->GetLookPos().m_dZ;
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
//========================================================================
// IActorComponent Functions
//========================================================================

void Camera::VUpdate()
{

}
// -----------------------------------------------------------------------
bool Camera::VInitComponent(TiXmlElement* pElement)
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

				SceneNode* targetNode = dynamic_cast<SceneNode*>(RoleSystem::Get().GetActor(attrib)->GetComponent("Graphics"));
				if (targetNode)
				{
					m_targetNode = targetNode;
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
	m_frustum = Frustum(1 / m_camParam.m_aspectRatio, 1, m_camParam.m_fieldOfView, m_camParam.m_nearClip, m_camParam.m_farClip);
	m_projectionMatrix = perspective(radians<float>(m_camParam.m_fieldOfView), m_camParam.m_aspectRatio, m_camParam.m_nearClip, m_camParam.m_farClip);	
	return true;
}
// -----------------------------------------------------------------------
bool Camera::VPostInit()
{
	GraphicsComponent::VPostInit();
	return true;
}