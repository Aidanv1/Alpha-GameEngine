#ifndef SCENENODE_H
#define SCENENODE_H
#include"..\AlphaStd.h"
#include "IDrawable.h"

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
class SceneNode
{
public:
	SceneNode();
	~SceneNode();
	bool Init(StrongDrawablePtr drawable);
	void GetPosition(vec3& pos) const { pos = m_positionInWorld; }
	void GetRotation(vec3& rot) const { rot = m_rotationInWorld; }
	NodeProperties GetNodeProperties() const { return m_nodeProperties; }
	void SetNodeProperties(NodeProperties &nodeProperties);
	void Draw();
private:
	StrongDrawablePtr m_drawable;
	vec3 m_positionInWorld;
	vec3 m_rotationInWorld;
	NodeProperties m_nodeProperties;

};
//========================================================================
#endif