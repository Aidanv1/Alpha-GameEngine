#pragma once
#include "../../../AlphaStd.h"
#include "../Buffers/VertexBuffer.h"
#include <GL/glew.h>
#include "../Data/Texture_GL.h"
#include "../Shader/SkyShaderProgram.h"
#include "../Renderer_GL.h"
#include "../../../Actor/Components/GraphicsComponent.h"
//========================================================================
class SkyBox_GL : public SceneNode, public GraphicsComponent
{
	friend class DrawableNodeFactory;
public:
	~SkyBox_GL();
	void VRender(Scene* pScene) override;
	virtual bool VInitNode() override;
	//IActorComponent functions
	virtual void VUpdate() override;
	virtual bool VInitComponent(TiXmlElement* pElement) override;
	virtual bool VPostInit() override;
private:
	SkyBox_GL();
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

