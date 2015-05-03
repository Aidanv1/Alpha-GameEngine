#include "Mesh.h"
// -----------------------------------------------------------------------
Mesh::Mesh(string meshFile) :
m_numVertices(0),
m_meshFileName(meshFile),
m_material(NULL),
m_shaderProgram(NULL),
m_vertexInfo(),
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
bool Mesh::Init()
{
	MeshShaderProgram* pShaderProgram = dynamic_cast<GLRenderer*>(GraphicsSystem::Get().GetRenderer())->GetMeshShaderProgram();
	if (!pShaderProgram)
	{
		return false;
	}
	m_shaderProgram = pShaderProgram;
	return true;
}
// -----------------------------------------------------------------------
void Mesh::VRender()
{
	m_shaderProgram->VUseProgram();
	m_shaderProgram->SetUniforms(	m_nodeProperties.ModelMatrix,
									m_nodeProperties.ViewMatrix,
									m_nodeProperties.ProjectionMatrix,
									m_nodeProperties.RotationMatrix,
									m_nodeProperties.LightVector,
									m_material->Texture()->VGetTextureID());
	BindData();
	glDrawArrays(GL_TRIANGLES, 0, m_numVertices);

	VRenderChildren();
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
bool Mesh::Validate()
{
	if (m_vertexInfo.m_vertexBufferID != -1)
	{
		//if the vertex buffer is still valid, return its ID.
		if (VertexBufferHandler::Get().ValidateID(m_vertexInfo.m_vertexBufferID, m_meshFileName))
		{
			return true;
		}
	}
	return false;
}
// -----------------------------------------------------------------------
int Mesh::LoadMesh(aiMesh* pMesh, aiMaterial* pMaterial)
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
	//-----------------------------------------------
	//-------------Create Material-------------------
	aiColor4D color(0.f, 0.f, 0.f,0.f);

	m_material = StrongMaterialPtr(ALPHA_NEW Material());
	//Diffuse
	pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	m_material->SetDiffuse(vec4(color.r, color.g, color.b , color.a));
	//Specular
	pMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
	float shininess;
	pMaterial->Get(AI_MATKEY_SHININESS, shininess);
	m_material->SetSpecular(vec4(color.r, color.g, color.b, color.a), shininess);
	//Emissive
	pMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
	m_material->SetEmissive(vec4(color.r, color.g, color.b, color.a));
	//Ambient
	pMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
	m_material->SetAmbient(vec4(color.r, color.g, color.b, color.a));
	//Texture
	aiString textureLoc;
	pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &textureLoc);
	string path = m_meshFileName;
	int end = path.find_last_of("/");
	path = path.substr(0, end + 1).append(textureLoc.C_Str());

	Texture* tex = ALPHA_NEW Texture(path.c_str());
	m_material->SetTexture(StrongTexturePtr(tex));
	//-----------------------------------------------
	//-------------Create VertexBuffer---------------
	//convert the texture coords to 2D
	vector<GLfloat> tarray;
	for (unsigned int i = 0; i < pMesh->mNumVertices; ++i)
	{
		tarray.push_back(pMesh->mTextureCoords[0][i].x);
		tarray.push_back(pMesh->mTextureCoords[0][i].y);
	}
	GLfloat* textarr = &tarray[0];

	m_numVertices = pMesh->mNumVertices;

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
	glBufferData(GL_ARRAY_BUFFER, 8 * m_numVertices*sizeof(GLfloat), NULL, GL_STATIC_DRAW);

	// Load the vertex points
	glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * m_numVertices*sizeof(GLfloat), pMesh->mVertices);
	// Load the colors right after that
	glBufferSubData(GL_ARRAY_BUFFER, 3 * m_numVertices*sizeof(GLfloat), 3 * m_numVertices*sizeof(GLfloat), pMesh->mNormals);
	glBufferSubData(GL_ARRAY_BUFFER, 6 * m_numVertices*sizeof(GLfloat), 2 * m_numVertices*sizeof(GLfloat), textarr);
	//-----------------------------------------------
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