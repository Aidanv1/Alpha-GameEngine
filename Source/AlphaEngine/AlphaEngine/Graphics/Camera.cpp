#include "Camera.h"
// -----------------------------------------------------------------------
Camera::Camera(float fovy, float aspectR, float near, float far, CameraMode mode, SceneNode* targetNode) :
m_fieldOfView(fovy),
m_aspectRatio(aspectR),
m_nearClip(near),
m_farClip(far),
m_viewMatrix(1.0f),
m_targetNode(NULL),
m_mode(mode),
m_orbitalRadius(10),
m_orbitalMax(20),
m_orbitalMin(0)
{
	m_projectionMatrix = perspective(fovy, aspectR, near, far);

	if (targetNode)
	{
		m_targetNode = targetNode;
		m_orbitalMin = targetNode->GetRadius();
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
}
// -----------------------------------------------------------------------
Camera::~Camera()
{

}
// -----------------------------------------------------------------------
void Camera::SetParameters(float fovy, float aspectR, float near, float far)
{
	m_fieldOfView = fovy;
	m_aspectRatio = aspectR;
	m_nearClip = near;
	m_farClip = far;
	m_projectionMatrix = perspective(fovy, aspectR, near, far);
}
// -----------------------------------------------------------------------
void Camera::SetCameraTarget(SceneNode* targetNode)
{
	m_targetNode = targetNode;
}
// -----------------------------------------------------------------------
void Camera::VUpdateNode(Scene* scene, float deltaMs)
{
	if (m_mode == FlyAround_Mode)
	{
		m_viewMatrix = rotate(mat4(1.0f), m_rotation.x, vec3(-1.0f, 0.0f, 0.0f));
		m_viewMatrix = rotate(m_viewMatrix, m_rotation.y, vec3(0.0f, -1.0f, 0.0f));		
		m_viewMatrix = translate(m_viewMatrix, m_positionInWorld);
	}
	else if (m_mode == Orbital_Mode)
	{
		mat4 rotMat = rotate(mat4(1.0f), m_rotation.x - half_pi<float>(), vec3(-1.0f, 0.0f, 0.0f));
		vec4 vectorPos = rotMat * vec4(0, 1.0f, 0, 0);
		rotMat = rotate(mat4(1.0f), m_rotation.y, vec3(0.0f, -1.0f, 0.0f));
		vectorPos = rotMat * vectorPos;
		//mat4 rotMat = rotate(mat4(1.0f), m_rotation.y, vec3(0.0f, -1.0f, 0.0f));
		vec3 targetPos;
		m_targetNode->GetPositionInWorld(targetPos);

		
		//vec4 vectorPos =  vec4(1.0f);
		m_positionInWorld = vec3(vectorPos) * m_orbitalRadius + targetPos;
		LookAtTarget();
	}
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
	if (Orbital_Mode)
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
		//clamp xaxis rotation
		if (m_rotation.x > half_pi<float>() * 0.98)
		{
			m_rotation.x = half_pi<float>() * 0.98;
		}
		if (m_rotation.x < -half_pi<float>() * 0.98)
		{
			m_rotation.x = -half_pi<float>() * 0.98;
		}

	}
}
// -----------------------------------------------------------------------