#ifndef SKYBOX_H
#define SKYBOX_H
#include "../../../AlphaStd.h"
#include "../Buffers/VertexBuffer.h"
#include <GL/glew.h>
#include "Texture.h"
#include "../Shader/SkyShaderProgram.h"
#include "../GLRenderer.h"
#include "../../../Actor/IActorComponent.h"
#include "../../../Actor/IComponentCreator.h"
//========================================================================
class SkyBox : public SceneNode, public IActorComponent
{
public:
	SkyBox();
	~SkyBox();	
	void VRender(Scene* pScene) override;
	//IActorComponent functions
	virtual ComponentType VGetType() override;
	virtual void VUpdate() override;
	virtual bool VInitComponent(TiXmlElement* pElement) override;
	virtual bool VPostInit() override;
private:
	virtual bool VInitNode() override;
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
//========================================================================
//SkyBox Creator
//========================================================================
class SkyBoxComponentCreator : public IComponentCreator
{
public:
	virtual IActorComponent* CreateComponent(TiXmlElement* pElement) override;
};


#endif