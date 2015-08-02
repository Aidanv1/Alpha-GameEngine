#pragma once
#include "SceneNode.h"
#include "../../ResourceManager/Resource.h"
#include "../../Animation/IAnimatableObject.h"
#include "../../Animation/SkeletalAnimation.h"
#include "../../Animation/AnimationSystem.h"
//========================================================================
typedef shared_ptr<Resource> StrongModelPtr;
//========================================================================
class ModelNode :public SceneNode, public IAnimatableObject
{
public:
	ModelNode();
	~ModelNode();
	//SceneNode functions
	bool VConfigureXmlNodeData(TiXmlElement* pElement) override;
	bool VInitNode() override;
	void VRender(Scene* pScene) override;
	void VUpdateNode(Scene* pScene, float deltaMS) override;
	//IAnimatableObject functions
	bool VHasAnimation() override;
	bool VHasAnimation(AnimationID animationID) override;
	bool VPlayAnimation(AnimationID animationID) override;
	bool VStopAnimation(AnimationID animationID) override;
	bool VStopAnimation() override;
	bool VPauseAnimation(AnimationID animationID) override;
	IAnimation* GetCurrentAnimation() override;
	//ModelNode functions
	string GetModelFileName() { return m_modelFileName; }
	virtual bool Load();
	bool RequestLoadResource();	
private:
	virtual void VRenderChildren(Scene* pScene) override;
	bool ValidateBuffers();
private:
	AnimationMap				m_animationMap;
	int							m_modelID;	
	string						m_modelFileName;
	StrongResourceManagerPtr	m_modelResourceManager;
	StrongModelPtr				m_modelResource;
	SceneNodeList				m_meshChildren;
	bool						m_cullFace;
};
