#pragma once

#include "../../../AlphaStd.h"
#include "../Data/Texture_GL.h"
#include "../../Scene/SceneNode.h"
#include "../Renderer_GL.h"
#include "../Buffers/VertexBuffer.h"
#include "../Shader/HeightMapShaderProgram.h"
#include "../../../Actor/Components/GraphicsComponent.h"
#include "../Data/Material_GL.h"

//========================================================================
class HeightMap_GL : public SceneNode
{
	const float X_SCALE = 500;
	const float Y_SCALE = 50;
	const float Z_SCALE = 500;
	//
	friend class DrawableNodeFactory;
public:
	~HeightMap_GL();
	void VRender(Scene* pScene) override;
	virtual bool VInitNode() override;
	virtual void VUpdateNode(Scene* pScene, float deltaMS) override;
	//IActorComponent functions
	virtual bool VConfigureXmlNodeData(TiXmlElement* pElement) override;
private:
	HeightMap_GL();
	int Load();
	bool VLoadResource();
	void BindData();
private:
	float m_tileScale;
	string m_textureFileName;
	int m_numVertices;
	HeightMapShaderProgram* m_shaderProgram;
	StrongBitmapPtr m_textureResource;
	StrongMaterialPtr m_material;
	StrongResourceManagerPtr m_textureResourceManager;
	VertexBuffer m_vertexBuffer;
};
//========================================================================