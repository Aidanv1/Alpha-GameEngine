#ifndef MESH_H
#define MESH_H
#include "Material.h"
#include "Handler\VertexBufferHandler.h"
#include "../../../ResourceManager/Resource.h"
#include "../../../ResourceManager/Resources/Raw.h"
#include "../GLRenderer.h"
#include "../Shader/MeshShaderProgram.h"
#include "../../GraphicsSystem.h"
#include "assimp/scene.h"
typedef shared_ptr<Resource> StrongMeshPtr;
typedef shared_ptr<Material> StrongMaterialPtr;
//========================================================================
class Mesh : public SceneNode
{
public:
	Mesh(string meshFile, StrongMaterialPtr material, StrongResourceManagerPtr pResManager);
	~Mesh();
	bool VInit() override;
	virtual bool VLoad();
	virtual void VUpdate() override;
	void VRender() override;
private:
	//loading methods
	bool LoadResource();
	unsigned long LoadMesh();
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
	StrongResourceManagerPtr m_pMeshResourceManager;
};
//========================================================================
#endif