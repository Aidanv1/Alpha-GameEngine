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
typedef shared_ptr<Resource> StrongMeshPtr;
typedef shared_ptr<Material> StrongMaterialPtr;
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
	//16 byte alignment
	//void* operator new(std::size_t sz)
	//{
	//	return _aligned_malloc(sz, 16);
	//}
private:
	//loading methods
	bool LoadResource();	
	//drawing methods
	void BindData();
	void SetVertexAttribPointer(GLuint attribID, int componentCount, int stride, const void* dataoffset);
private:
	struct VertexBufferInfo
	{
		GLuint m_vertexBufferID;
		GLuint m_vertexArrayID;
		VertexBufferInfo() :
			m_vertexBufferID(-1),
			m_vertexArrayID(-1)
		{
		}
	};
	MeshShaderProgram* m_shaderProgram;
	long m_meshID;	
	int m_numVertices;
	string m_meshFileName;
	string m_textureFileName;
	StrongMeshPtr m_meshResource;
	StrongMaterialPtr m_material;
	VertexBufferInfo m_vertexInfo;	
};

//========================================================================
#endif