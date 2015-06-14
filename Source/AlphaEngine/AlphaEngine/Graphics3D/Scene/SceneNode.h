#pragma once
#include"../..\AlphaStd.h"
#include "../../Actor/Actor.h"
#include "../../Maths/LinearAlgebra.h"
class Scene;
class ISceneNode;
typedef shared_ptr<ISceneNode> StrongSceneNodePtr;
typedef vector<StrongSceneNodePtr> SceneNodeList;
//========================================================================
enum AlphaType
{
	tTRANSPARENT,
	tOPAQUE
};
enum RenderPass
{
	RenderPass_Static, // environments and level geometry
	RenderPass_Actor, // objects and things that can move
	RenderPass_Sky, // the background ‘behind’ everything
	RenderPass_NotRendered, // objects that don’t render but exist
	RenderPass_Overlay // overlay
};
struct NodeProperties
{
	ActorID m_ActorID;	
	string m_name;
	Matrix4x4 m_toWorld;
	AlphaType m_alphaType;
	RenderPass m_renderPass;
	NodeProperties() :
		m_ActorID(-1),
		m_name(""),
		m_toWorld(1.0f),
		m_alphaType(tOPAQUE),
		m_renderPass(RenderPass_NotRendered)
	{

	}
};

//========================================================================
class ISceneNode
{
public:
	virtual void VRender(Scene* pScene) = 0;
	virtual bool VInitNode() = 0;
	virtual void VAddChild(StrongSceneNodePtr sceneNode) = 0;
	virtual void VUpdateNode(Scene* pScene, float deltaMS) = 0;
	virtual float VGetScreenZ() const = 0;
	virtual void VSetTransform(Matrix4x4& ToWorld) = 0;
	virtual void VSetTransform(mat4& ToWorld) = 0;
	virtual NodeProperties VGetNodeProperties() const = 0;
	virtual void VSetNodeProperties(NodeProperties &nodeProperties) = 0;
	virtual bool VConfigureXmlNodeData(TiXmlElement* pConfigElement) = 0;
protected:
	virtual void VRenderChildren(Scene* pScene) = 0;
};
//========================================================================
class SceneNode: public ISceneNode
{	
public:
	SceneNode();
	SceneNode(string name);
	~SceneNode();
	NodeProperties VGetNodeProperties() const override{ return m_nodeProperties; }
	void VSetNodeProperties(NodeProperties &nodeProperties) override;
	virtual bool VInitNode() override;
	virtual void VUpdateNode(Scene* pScene, float deltaMS) override;
	virtual void VRender(Scene* pScene) override;
	virtual void VAddChild(StrongSceneNodePtr sceneNode) override;
	void GetPositionInWorld(vec3& pos) const { pos = m_nodeProperties.m_toWorld.GetPosition(); }
	void SetPositionInWorld(vec3& pos);
	void GetRotationInWorld(mat4& rot) const { rot = m_nodeProperties.m_toWorld.GetRotation(); }
	void SetRotationInWorld(vec3& rot);
	void VSetTransform(Matrix4x4& ToWorld) override;
	void VSetTransform(mat4& ToWorld) override;
	float GetRadius() const { return m_radius; }
	float VGetScreenZ() const override { return m_screenZ; }
	virtual bool VConfigureXmlNodeData(TiXmlElement* pConfigElement) override { return true; }
protected:
	virtual void VRenderChildren(Scene* pScene) override;
protected:
	string m_name;
	NodeProperties m_nodeProperties;
	SceneNodeList m_children;
	float m_radius;
	bool m_isVisible;
	float m_screenZ;

};
//========================================================================
