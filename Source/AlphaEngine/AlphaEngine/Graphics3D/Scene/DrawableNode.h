#pragma once
#include "SceneNode.h"
//========================================================================
enum NodeType
{
	Node_Mesh,
	Node_LineMesh,
	Node_Sky,
	Node_HeightMap,
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
struct aiMesh;
struct aiMaterial;
class IMesh :public SceneNode
{
public:
	IMesh() : SceneNode(){};
	//
	virtual bool VInitMesh(string meshFile) = 0;
	virtual int VLoadMesh(aiMesh* pMesh, aiMaterial* pMaterial) = 0;
	virtual bool VValidate() = 0;
	virtual void VFreeBuffer() = 0;
	virtual bool VLoadMaterial() = 0;
}; 
//========================================================================
class ILineMesh : public SceneNode
{
public:
	virtual int VLoadLineMesh(float vertexArray[], int numVertices) = 0;
	virtual bool VInitLineMesh() = 0;
};
//========================================================================