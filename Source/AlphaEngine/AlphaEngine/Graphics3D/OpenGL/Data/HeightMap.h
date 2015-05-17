#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H
#include "../../../AlphaStd.h"
#include "../Data/Texture.h"
#include "../../Scene/SceneNode.h"
#include "../GLRenderer.h"
#include "../Buffers/VertexBuffer.h"
#include "../Shader/HeightMapShaderProgram.h"
#include "../../../Actor/IActorComponent.h"
#include "Material.h"
//========================================================================
class HeightMap : public SceneNode, public IActorComponent
{
	const float X_SCALE = 500;
	const float Y_SCALE = 50;
	const float Z_SCALE = 500;
	
public:
	HeightMap();
	~HeightMap();
	void VRender(Scene* pScene) override;
	virtual bool VInitNode() override;
	virtual void VUpdateNode(Scene* pScene, float deltaMS) override;
	//IActorComponent functions
	virtual ComponentType VGetType() override;
	virtual void VUpdate() override;
	virtual bool VInitComponent(TiXmlElement* pElement) override;
	virtual bool VPostInit() override;
private:
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

#endif