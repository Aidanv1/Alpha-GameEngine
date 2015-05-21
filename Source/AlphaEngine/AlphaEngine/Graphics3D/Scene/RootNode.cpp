#include "RootNode.h"
// -----------------------------------------------------------------------
RootNode::RootNode()
{
	//pass 0
	StrongSceneNodePtr actorNode(ALPHA_NEW SceneNode("Actor Group"));
	m_children.push_back(actorNode);
	//pass 1
	StrongSceneNodePtr staticNode(ALPHA_NEW SceneNode("Static Group"));
	m_children.push_back(staticNode);
	//pass 2
	StrongSceneNodePtr skyNode(ALPHA_NEW SceneNode("Sky Group"));
	m_children.push_back(skyNode);
	//pass 3
	StrongSceneNodePtr invisibleNode(ALPHA_NEW SceneNode("Invisible Group"));
	m_children.push_back(invisibleNode);
	//pass 4
	StrongSceneNodePtr overLayNode(ALPHA_NEW SceneNode("Overlay Group"));
	m_children.push_back(overLayNode);
}
// -----------------------------------------------------------------------
RootNode::~RootNode()
{

}
// -----------------------------------------------------------------------
void RootNode::VUpdateNode(Scene* pScene, float deltaMS)
{
	//update all child nodes
	for (auto child = m_children.begin(); child != m_children.end(); child++)
	{
		(*child)->VUpdateNode(pScene, deltaMS);
	}
}
// -----------------------------------------------------------------------
void RootNode::VRender(Scene* pScene)
{
	//render passes
	for (int pass = 0; pass < m_children.size(); pass++)
	{
		if (pass != RenderPass_NotRendered)
		{
			StrongSceneNodePtr node = m_children[pass];
			node->VRender(pScene);
		}
	}
}
// -----------------------------------------------------------------------
bool RootNode::VInitNode()
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
void RootNode::VAddChild(StrongSceneNodePtr sceneNode)
{
	RenderPass pass = sceneNode->GetNodeProperties().m_renderPass;
	if ((unsigned)pass >= m_children.size() || !m_children[pass])
	{
		ALPHA_ERROR("There is no such render pass");		
	}
	StrongSceneNodePtr node = m_children[pass];
	node->VAddChild(sceneNode);
}
// -----------------------------------------------------------------------
