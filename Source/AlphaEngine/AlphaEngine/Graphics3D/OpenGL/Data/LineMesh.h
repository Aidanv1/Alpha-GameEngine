#pragma once

#include "Handler\VertexBufferHandler.h"
#include "../GLRenderer.h"
#include "../Shader/BasicShaderProgram.h"
#include "../../GraphicsSystem.h"
#include "../Buffers/VertexBuffer.h"

//========================================================================
class LineMesh : public SceneNode
{
	static unsigned int s_LineMeshNumber;
public:
	LineMesh();
	~LineMesh();
	virtual bool Init();
	void VRender(Scene* pScene) override;
	virtual void VUpdateNode(Scene* pScene, float deltaMS) override;
	int LoadLineMesh(float vertexArray[], int numVertices);
private:
	//drawing methods
	void BindData();
private:
	BasicShaderProgram* m_shaderProgram;
	int					m_numVertices;
	string				m_LineMeshName;
	VertexBuffer		m_vertexBuffer;
};

//========================================================================
