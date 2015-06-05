#pragma once

#include "..\Handler\VertexBufferHandler.h"
#include "../Renderer_GL.h"
#include "../Shader/BasicShaderProgram.h"
#include "../../GraphicsSystem.h"
#include "../Buffers/VertexBuffer.h"
//========================================================================
class LineMesh_GL : public ILineMesh
{
	friend class DrawableNodeFactory;
	static unsigned int s_LineMeshNumber;
public:
	~LineMesh_GL();
	void VRender(Scene* pScene) override;
	virtual void VUpdateNode(Scene* pScene, float deltaMS) override;
	//Line mesh functions
	int VLoadLineMesh(float vertexArray[], int numVertices) override;
	virtual bool VInitLineMesh() override;
private:
	LineMesh_GL();
	//drawing methods
	void BindData();
private:
	BasicShaderProgram* m_shaderProgram;
	int					m_numVertices;
	string				m_LineMeshName;
	VertexBuffer		m_vertexBuffer;
};

//========================================================================
