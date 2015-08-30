#pragma once
#include "../../AlphaStd.h"
#include "SceneNode.h"
#include "../../EventManager/EventManager.h"
#include "../GraphicsEvents.h"
#include "../../Actor/Components/GraphicsComponent.h"
#include "../../Actor/RoleSystem.h"
#include	"../Geometry/Geometry.h"
//========================================================================
enum CameraMode
{
	FlyAround_Mode,
	Orbital_Mode
};
//========================================================================
class CameraNode : public SceneNode
{
	struct CameraParameters
	{
		float m_fieldOfView;
		float m_aspectRatio;
		float m_nearClip;
		float m_farClip;

		CameraParameters(float fovy, float aspectR, float nearClip, float farClip) :
			m_fieldOfView(fovy),
			m_aspectRatio(aspectR),
			m_nearClip(nearClip),
			m_farClip(farClip)
		{
		}
		CameraParameters() :
			m_fieldOfView(0),
			m_aspectRatio(0),
			m_nearClip(0),
			m_farClip(0)
		{
		}
	};
public:
	//Set camera parameters and optional target
	CameraNode();
	~CameraNode();
	void SetParameters(float fovy, float aspectR, float nearClip, float farClip);

	SceneNode* GetCameraTarget() const { return m_targetNode; }
	void GetProjectionMatrix(mat4& projMat) const { projMat = m_projectionMatrix; }
	void GetViewMatrix(mat4& viewMat) const { viewMat = m_viewMatrix; };
	void GetViewMatrix(Matrix4x4& viewMat) const { viewMat = m_viewMatrix; };
	void GetTranslationMatrix(mat4& transMat) const { transMat = m_translationMatrix; }
	void GetRotationMatrix(mat4& rotMat) const { rotMat = m_rotationMatrix; }

	void SetCameraTarget(SceneNode* targetNode);
	CameraMode GetMode() const { return m_mode; }
	void SetMode(CameraMode mode);
	CameraParameters GetCameraParameters() const{ return m_camParam; }
	Frustum GetFrustum() const { return m_frustum; }
	//Scene Node functions
	virtual bool VInitNode() override;
	void VUpdateNode(Scene* scene, float deltaMs) override;
	virtual bool VConfigureXmlNodeData(TiXmlElement* pElement) override;

	//Look event delegate listens reacts to lookevents ie. any camera movement
	void LookEventDelegate(StrongEventPtr e);
private:
	void LookAtTarget();
private:	
	mat4 m_projectionMatrix;
	mat4 m_translationMatrix;
	mat4 m_rotationMatrix;
	mat4 m_viewMatrix;
	SceneNode* m_targetNode;
	vec3 m_rotation;
	float m_orbitalRadius;
	float m_orbitalMax;
	float m_orbitalMin;
	CameraParameters m_camParam;
	CameraMode m_mode;
	Frustum m_frustum;
};
//========================================================================
