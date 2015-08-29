#include "SceneNode.h"
#include "Scene.h"
// -----------------------------------------------------------------------
SceneNode::SceneNode() :
m_nodeProperties(),
m_radius(0),
m_isVisible(true),
m_screenZ(0),
m_isActive(true)
{
}
// -----------------------------------------------------------------------
SceneNode::SceneNode(string name) :
m_nodeProperties(),
m_radius(0),
m_name(name),
m_screenZ(0),
m_isActive(true)
{
}
// -----------------------------------------------------------------------
SceneNode::~SceneNode()
{

}

// -----------------------------------------------------------------------
void SceneNode::VSetNodeProperties(NodeProperties& nodeProperties)
{
	m_nodeProperties = nodeProperties;
}
// -----------------------------------------------------------------------
void SceneNode::VRender(Scene* pScene)
{
	VRenderChildren(pScene);	
}
// -----------------------------------------------------------------------
void SceneNode::VRenderChildren(Scene* pScene)
{
	//iterate through all children and call their render function
	for (auto child = m_children.begin(); child != m_children.end(); child++)
	{
		(*child)->VRender(pScene);
	}
}
// -----------------------------------------------------------------------
void SceneNode::VAddChild(StrongSceneNodePtr sceneNode)
{
	m_children.push_back(sceneNode);
}
// -----------------------------------------------------------------------
void SceneNode::SetPositionInWorld(vec3& pos)
{
	m_nodeProperties.m_toWorld.SetPosition(pos);
}
// -----------------------------------------------------------------------
void SceneNode::SetRotationInWorld(vec3& rot)
{
	m_nodeProperties.m_toWorld.SetRotation(rot);
}
// -----------------------------------------------------------------------
bool SceneNode::VInitNode()
{
	bool success = true;
	for (auto child = m_children.begin(); child != m_children.end(); child++)
	{
		if (!(*child)->VInitNode())
		{
			success = false;
		}
	}
	return success;
}
// -----------------------------------------------------------------------
void SceneNode::VUpdateNode(Scene* pScene, float deltaMS)
{
	auto child = m_children.begin();
	while (child != m_children.end())
	{
		if ((*child)->VIsActive())
		{
			(*child)->VUpdateNode(pScene, deltaMS);
			child++;
		}
		else
		{
			child = m_children.erase(child);
		}
	}
}
// -----------------------------------------------------------------------
void SceneNode::VSetTransform(Matrix4x4& ToWorld)
{
	m_nodeProperties.m_toWorld = ToWorld;
}
// -----------------------------------------------------------------------
void SceneNode::VSetTransform(mat4& ToWorld)
{
	m_nodeProperties.m_toWorld = ToWorld;
}
// -----------------------------------------------------------------------
void SceneNode::VDestroyNode()
{
	m_isActive = false;
}
// -----------------------------------------------------------------------
bool SceneNode::VIsActive()
{
	return m_isActive;
}