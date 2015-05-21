#pragma once
#include "../../../AlphaStd.h"
#include "../Buffers/VertexBuffer.h"
#include <GL/glew.h>
#include "Texture.h"
#include "../Shader/SkyShaderProgram.h"
#include "../GLRenderer.h"
#include "../../../Actor/Components/GraphicsComponent.h"
//========================================================================
class SkyBox : public SceneNode, public GraphicsComponent
{
public:
	SkyBox();
	~SkyBox();	
	void VRender(Scene* pScene) override;
	virtual bool VInitNode() override;
	//IActorComponent functions
	virtual void VUpdate() override;
	virtual bool VInitComponent(TiXmlElement* pElement) override;
	virtual bool VPostInit() override;
private:
	//loading methods
	int Load();
	bool LoadTexture();
	//drawing methods
	void BindData();
private:
	string				m_textureFileName[6];
	SkyShaderProgram*	m_shaderProgram;
	long				m_meshID;
	int					m_numVertices;
	string				m_skyName;
	StrongTexturePtr	m_skyTexture;
	VertexBuffer		m_vertexBuffer;
};

