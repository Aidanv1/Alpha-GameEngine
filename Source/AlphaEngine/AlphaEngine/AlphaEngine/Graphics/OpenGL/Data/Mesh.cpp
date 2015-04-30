#include "Mesh.h"
// -----------------------------------------------------------------------
Mesh::Mesh(string meshFile, StrongMaterialPtr material, StrongResourceManagerPtr pResManager) :
m_numVertices(0),
m_pMeshResourceManager(pResManager),
m_meshFileName(meshFile),
m_meshResource(NULL),
m_material(material),
m_shaderProgram(NULL),
m_vertexInfo(),
m_sceneNode(NULL),
m_meshID(-1)
{

}
// -----------------------------------------------------------------------
Mesh::~Mesh()
{
	//free blocks in video memory
	VertexBufferHandler::Get().FreeVertexBuffer(m_vertexInfo.m_vertexBufferID);
	glDeleteVertexArrays(1, &(m_vertexInfo.m_vertexArrayID));
}
// -----------------------------------------------------------------------
bool Mesh::VInit(SceneNode* pSceneNode)
{

	MeshShaderProgram* pShaderProgram = dynamic_cast<GLRenderer*>(GraphicsSystem::Get().GetRenderer())->GetMeshShaderProgram();
	if (!pShaderProgram)
	{
		return false;
	}
	if (!pSceneNode)
	{
		return false;
	}
	m_shaderProgram = pShaderProgram;
	m_sceneNode = pSceneNode;

	//set position and rotation to that of the owner scenenode
	if (!m_material)
	{
		return false;
	}
	//attempt to load resource
	

	return true;
}
// -----------------------------------------------------------------------
void Mesh::VDraw()
{
	if (m_meshID == -1)
	{
		return;
	}
	m_shaderProgram->VUseProgram();
	m_shaderProgram->SetUniforms(	m_sceneNode->GetNodeProperties().ModelMatrix,
									m_sceneNode->GetNodeProperties().ViewMatrix, 
									m_sceneNode->GetNodeProperties().ProjectionMatrix, 
									m_sceneNode->GetNodeProperties().RotationMatrix, 
									m_sceneNode->GetNodeProperties().LightVector, 
									m_material->Texture()->VGetTextureID());
	BindData();
	glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
}
// -----------------------------------------------------------------------
void Mesh::BindData()
{
	int offset = 3 * m_numVertices*sizeof(GLfloat);
	SetVertexAttribPointer(m_shaderProgram->GetPositionID(), 3, 0, 0);
	SetVertexAttribPointer(m_shaderProgram->GetNormalID(), 3, 0, (const void*)offset);
	SetVertexAttribPointer(m_shaderProgram->GetTextureID(), 2, 0, (const void*)(offset * 2));
}
// -----------------------------------------------------------------------
bool Mesh::VLoad()
{
	bool success = true;
	if (!LoadResource())
	{
		ALPHA_ERROR("Unable to load resource");
		return false;
	}
	//attempt to load mesh	
	m_meshID = LoadMesh();
	if (m_meshID == -1)
	{
		success = false;
	}
	

	//attempt to load texture
	if (!m_material->LoadTexture())
	{
		success = false;
	}

	return success;
}
// -----------------------------------------------------------------------
bool Mesh::LoadResource()
{
	//if no resource currently exists
	if (!m_meshResource)
	{
		//Request load Resource
		Resource* meshResource = new Resource(m_meshFileName);
		meshResource->RequestLoad();
		m_meshResource = StrongMeshPtr(meshResource);

		if (!m_pMeshResourceManager->AddResource(m_meshResource))
		{
			return false;
		}
	}
	else if (!m_meshResource->IsLoaded())
	{
		m_meshResource->RequestLoad();
	}
	return true;
}
// -----------------------------------------------------------------------
unsigned long Mesh::LoadMesh()
{
	//first check if vertex buffer was already loaded in video memory
	if (m_vertexInfo.m_vertexBufferID != -1)
	{
		//if the vertex buffer is still valid, return its ID.
		if (VertexBufferHandler::Get().ValidateID(m_vertexInfo.m_vertexBufferID, m_meshFileName))
		{
			return m_vertexInfo.m_vertexBufferID;
		}
	}
	if (!m_meshResource)
	{
		return -1;
	}
	if (!m_meshResource->Buffer())
	{
		return -1;
	}
	if (!m_meshResource->IsLoaded())
	{
		return -1;
	}

	MeshData* mesh = (MeshData*)m_meshResource->Buffer();
	

	GLfloat* vertexarr = &(mesh->m_vArray[0]);
	GLfloat* normarr = &(mesh->m_nArray[0]);
	GLfloat* textarr = &(mesh->m_tArray[0]);
	m_numVertices = mesh->m_numVertices;
	//initialize vertex buffer
	if (m_vertexInfo.m_vertexArrayID == -1)
	{
		glGenVertexArrays(1, &(m_vertexInfo.m_vertexArrayID));
	}
	glBindVertexArray(m_vertexInfo.m_vertexArrayID);
	m_vertexInfo.m_vertexBufferID = VertexBufferHandler::Get().GetVertexBufferHandle(m_vertexInfo.m_vertexBufferID, m_meshFileName);
	//glGenBuffers(1, &m_vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexInfo.m_vertexBufferID);
	// Create the buffer, but don't load anything yet
	glBufferData(GL_ARRAY_BUFFER, 8 * mesh->m_numVertices*sizeof(GLfloat), NULL, GL_STATIC_DRAW);

	// Load the vertex points ---THIS MUST BE CHANGED
	glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * mesh->m_numVertices*sizeof(GLfloat), vertexarr);
	// Load the colors right after that
	glBufferSubData(GL_ARRAY_BUFFER, 3 * mesh->m_numVertices*sizeof(GLfloat), 3 * mesh->m_numVertices*sizeof(GLfloat), normarr);
	glBufferSubData(GL_ARRAY_BUFFER, 6 * mesh->m_numVertices*sizeof(GLfloat), 2 * mesh->m_numVertices*sizeof(GLfloat), textarr);
	return m_vertexInfo.m_vertexBufferID;
}
// -----------------------------------------------------------------------
void Mesh::SetVertexAttribPointer(GLuint attribID, int componentCount, int stride, const void* dataoffset)
{
	glBindVertexArray(m_vertexInfo.m_vertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexInfo.m_vertexBufferID);
	glVertexAttribPointer(attribID, componentCount, GL_FLOAT, GL_FALSE, stride, dataoffset);
	glEnableVertexAttribArray(attribID);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
// -----------------------------------------------------------------------