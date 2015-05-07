#ifndef CAMERA_H
#define CAMERA_H
#include "../AlphaStd.h"
#include "SceneNode.h"
//========================================================================
class Camera : public SceneNode
{
public:
	//Set camera parameters and optional target
	Camera(float fovy, float aspectR, float near, float far, SceneNode* targetNode = NULL);
	~Camera();
	void SetParameters(float fovy, float aspectR, float near, float far);
	SceneNode* GetCameraTarget() const { return m_targetNode; }
	void GetProjectionMatrix(mat4& projMat) const { projMat = m_projectionMatrix; }
	void GetViewMatrix(mat4& viewMat) const { viewMat = m_viewMatrix; };
	void SetCameraTarget(SceneNode* targetNode);
	void VUpdateNode(Scene* scene, float deltaMs) override;

	void SetRotation(vec3 rotation);
	void SetPosition(vec3 position);
	void GetRotation(vec3& rotation) const{ rotation = m_rotation; }
	void GetPosition(vec3& position) const{ position = m_positionInWorld; }
	//16 byte alignment
	void* operator new(std::size_t sz)
	{
		return _aligned_malloc(sz, 16);
	}
private:
	mat4 m_projectionMatrix;
	mat4 m_viewMatrix;
	SceneNode* m_targetNode;
	vec3 m_rotation;
	float m_fieldOfView;
	float m_aspectRatio;
	float m_nearClip;
	float m_farClip;

};
//========================================================================
#endif