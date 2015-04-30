#include "SceneNode.h"
// -----------------------------------------------------------------------
SceneNode::SceneNode() :
m_drawable(NULL),
m_positionInWorld(0),
m_rotationInWorld(0),
m_nodeProperties()
{
}
// -----------------------------------------------------------------------
SceneNode::~SceneNode()
{

}
// -----------------------------------------------------------------------
bool SceneNode::Init(StrongDrawablePtr drawable)
{
	if (!drawable)
	{
		return false;
	}
	m_drawable = drawable;	
	return true;
}
// -----------------------------------------------------------------------
void SceneNode::Draw()
{
	if (m_drawable->VLoad())
	{
		m_drawable->VDraw();
	}
}
// -----------------------------------------------------------------------
void SceneNode::SetNodeProperties(NodeProperties& nodeProperties)
{
	m_nodeProperties = nodeProperties;
}

// -----------------------------------------------------------------------