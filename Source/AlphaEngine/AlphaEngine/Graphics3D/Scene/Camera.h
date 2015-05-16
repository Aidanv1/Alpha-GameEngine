#ifndef CAMERA_H
#define CAMERA_H
#include "../../AlphaStd.h"
#include "SceneNode.h"
#include "../../EventManager/EventManager.h"
#include "../../EventManager/Events/Events.h"
//========================================================================
enum CameraMode
{
	FlyAround_Mode,
	Orbital_Mode
};
//========================================================================
class Camera : public SceneNode
{
	struct CameraParameters
	{
		float m_fieldOfView;
		float m_aspectRatio;
		float m_nearClip;
		float m_farClip;

		CameraParameters(float fovy, float aspectR, float near, float far) :
			m_fieldOfView(fovy),
			m_aspectRatio(aspectR),
			m_nearClip(near),
			m_farClip(far)
		{

		}
	};
public:
	//Set camera parameters and optional target
	Camera(float fovy, float aspectR, float near, float far, CameraMode mode = FlyAround_Mode, SceneNode* targetNode = NULL);
	~Camera();
	void SetParameters(float fovy, float aspectR, float near, float far);
	virtual bool VInitNode() override;
	SceneNode* GetCameraTarget() const { return m_targetNode; }
	void GetProjectionMatrix(mat4& projMat) const { projMat = m_projectionMatrix; }
	void GetViewMatrix(mat4& viewMat) const { viewMat = m_viewMatrix; };
	void SetCameraTarget(SceneNode* targetNode);
	void VUpdateNode(Scene* scene, float deltaMs) override;
	CameraMode GetMode() const { return m_mode; }
	void SetMode(CameraMode mode);	
	CameraParameters GetCameraParameters() const{ return m_camParam; }

	//Look event delegate listens reacts to lookevents ie. any camera movement
	void LookEventDelegate(StrongEventPtr event);
private:
	void LookAtTarget();
private:	
	mat4 m_projectionMatrix;
	mat4 m_viewMatrix;
	SceneNode* m_targetNode;
	vec3 m_rotation;
	float m_orbitalRadius;
	float m_orbitalMax;
	float m_orbitalMin;
	CameraParameters m_camParam;
	CameraMode m_mode;
};
//========================================================================
#endif