#ifndef SCENENODE_H
#define SCENENODE_H
#include"..\AlphaStd.h"

class IDrawable;
typedef shared_ptr<IDrawable> StrongDrawablePtr;
//========================================================================
struct NodeProperties
{
	mat4 ModelMatrix;
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	mat4 RotationMatrix;
	vec4 LightVector;
	NodeProperties()
	{
		ModelMatrix = mat4(0);
		ViewMatrix = mat4(0);
		ProjectionMatrix = mat4(0);
		RotationMatrix = mat4(0);
		LightVector = vec4(0);
	}
};
//========================================================================
class ISceneNode
{
public:
	virtual void VUpdate() = 0;
	virtual bool VInit() = 0;
	virtual void VRender() = 0;
	virtual bool VLoad() = 0;
};
//========================================================================
class SceneNode: public ISceneNode
{
public:
	SceneNode();
	~SceneNode();
	NodeProperties GetNodeProperties() const { return m_nodeProperties; }
	void SetNodeProperties(NodeProperties &nodeProperties);
	virtual bool VInit() override { return true; }
	virtual void VUpdate() override {}
	virtual void VRender() override {}
	virtual bool VLoad(){ return true; }
protected:
	StrongDrawablePtr m_drawable;
	vec3 m_positionInWorld;
	vec3 m_rotationInWorld;
	NodeProperties m_nodeProperties;

};
//========================================================================
#endif