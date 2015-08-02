#pragma once
#include "SceneNode.h"
//========================================================================
enum NodeType
{
	Node_Mesh,
	Node_Sky,
	Node_Text
};

class DrawableNodeFactory
{
public:
	ISceneNode* CreateDrawableNode(NodeType type);
};
//========================================================================
class IText : public SceneNode
{
public:
	virtual bool VInitText(string fontTexturePath, int maxChar) = 0;
	virtual void VPrintText(string text, float x, float y, float size, float aspectRatio = 16.0 / 9.0) = 0;
	virtual void VCleanUpText() = 0;
};
//========================================================================
struct MeshInfo;
class ModelNode;
class IMesh : public SceneNode
{
public:
	//
	virtual bool VInitMesh(string meshFile, ModelNode* parentModel) = 0;
	virtual int VLoadMesh(MeshInfo* pMesh) = 0;
	virtual bool VValidate() = 0;
	virtual void VFreeBuffer() = 0;
	virtual bool VLoadMaterial() = 0;
	virtual void VCullFace(bool cull = false) = 0;
}; 
//========================================================================

