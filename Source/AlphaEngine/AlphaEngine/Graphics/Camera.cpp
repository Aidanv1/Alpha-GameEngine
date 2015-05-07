#include "Camera.h"
// -----------------------------------------------------------------------
Camera::Camera(float fovy, float aspectR, float near, float far, SceneNode* targetNode) :
m_fieldOfView(fovy),
m_aspectRatio(aspectR),
m_nearClip(near),
m_farClip(far),
m_viewMatrix(1.0f),
m_targetNode(NULL)
{
	m_projectionMatrix = perspective(fovy, aspectR, near, far);

	if (targetNode)
	{
		m_targetNode = targetNode;
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
	m_viewMatrix = rotate(mat4(1.0f), m_rotation.x, vec3(-1.0f, 0.0f, 0.0f));
	m_viewMatrix = rotate(m_viewMatrix, m_rotation.y, vec3(0.0f, -1.0f, 0.0f));
	m_viewMatrix = rotate(m_viewMatrix, m_rotation.z, vec3(1.0f, 0.0f, -1.0f));
	m_viewMatrix = translate(m_viewMatrix, m_positionInWorld);
}
// -----------------------------------------------------------------------

void Camera::SetRotation(vec3 rotation)
{
	m_rotation = rotation;
}
// -----------------------------------------------------------------------
void Camera::SetPosition(vec3 position)
{
	m_positionInWorld = position;
}
// -----------------------------------------------------------------------