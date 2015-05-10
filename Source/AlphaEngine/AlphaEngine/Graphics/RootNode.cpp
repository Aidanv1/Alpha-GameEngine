#include "RootNode.h"

void RootNode::VUpdateNode(Scene* pScene, float deltaMS)
{
	//update all child nodes
	for (auto child = m_children.begin(); child != m_children.end(); child++)
	{
		(*child)->VUpdateNode(pScene, deltaMS);
	}
}
void RootNode::VRender(Scene* pScene)
{
	VRenderChildren(pScene);
}

bool RootNode::VInitNode()
{
	return true;
}