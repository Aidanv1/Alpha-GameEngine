#pragma once
#include "../../../ResourceManager/Resource.h"
#include "../Renderer_GL.h"
#include "../Shader/MeshShaderProgram.h"
#include "../../GraphicsSystem.h"
#include "assimp/scene.h"
#include "../Data/Material_GL.h"
#include "../Buffers/VertexBuffer.h"
#include "../../Scene/DrawableNode.h"

typedef shared_ptr<Resource> StrongMeshPtr;
//========================================================================
class Mesh_GL : public IMesh
{
	friend class DrawableNodeFactory;
public:
	~Mesh_GL();
	void VRender(Scene* pScene) override;
	//Mesh functions
	virtual bool VInitMesh(string meshFile) override;
	int VLoadMesh(aiMesh* pMesh, aiMaterial* pMaterial) override;
	bool VLoadMaterial() override;
	bool VValidate() override;
	void VFreeBuffer() override;
private:
	Mesh_GL();
	//loading methods
	bool LoadResource();	
	//drawing methods
	void BindData();	
private:
	MeshShaderProgram* m_shaderProgram;
	long m_meshID;	
	int m_numVertices;
	string m_meshFileName;
	StrongMaterialPtr m_material;
	VertexBuffer m_vertexBuffer;
};

//========================================================================
