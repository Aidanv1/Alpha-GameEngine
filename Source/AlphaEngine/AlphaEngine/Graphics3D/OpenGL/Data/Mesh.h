#ifndef MESH_H
#define MESH_H

#include "Handler\VertexBufferHandler.h"
#include "../../../ResourceManager/Resource.h"
#include "../../../ResourceManager/Resources/Raw.h"
#include "../GLRenderer.h"
#include "../Shader/MeshShaderProgram.h"
#include "../../GraphicsSystem.h"
#include "assimp/scene.h"
#include "Material.h"
#include "../Buffers/VertexBuffer.h"
typedef shared_ptr<Resource> StrongMeshPtr;
//========================================================================
class Mesh : public SceneNode
{
public:
	Mesh(string meshFile);
	~Mesh();
	virtual bool Init();
	void VRender(Scene* pScene) override;
	int LoadMesh(aiMesh* pMesh, aiMaterial* pMaterial);
	StrongMaterialPtr GetMaterial() { return m_material; }
	bool Validate();
	void FreeVertexBuffer();
private:
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
#endif