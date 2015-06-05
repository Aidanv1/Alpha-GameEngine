#include "Mesh_GL.h"
// -----------------------------------------------------------------------
Mesh_GL::Mesh_GL() :
m_numVertices(0),
m_meshFileName(""),
m_material(NULL),
m_shaderProgram(NULL),
m_vertexBuffer(),
m_meshID(-1)
{
}
// -----------------------------------------------------------------------
Mesh_GL::~Mesh_GL()
{
}
// -----------------------------------------------------------------------
bool Mesh_GL::VInitMesh(string meshFile)
{
	m_meshFileName = meshFile;
	MeshShaderProgram* pShaderProgram = dynamic_cast<Renderer_GL*>(GraphicsSystem::Get().GetRenderer())->GetMeshShaderProgram();
	if (!pShaderProgram)
	{
		return false;
	}
	m_shaderProgram = pShaderProgram;
	return true;
}
// -----------------------------------------------------------------------
void Mesh_GL::VRender(Scene* pScene)
{
	m_shaderProgram->VUseProgram();
	//get view and projection matrix from main camera
	mat4 viewMat;
	mat4 projMat;
	pScene->GetCamera()->GetViewMatrix(viewMat);
	pScene->GetCamera()->GetProjectionMatrix(projMat);
	//set shader uniforms
	m_shaderProgram->SetUniforms(	m_nodeProperties.m_toWorld,
									viewMat,
									projMat,
									m_nodeProperties.m_rotationMatrix,									
									m_material->Texture()->VGetTextureID());
	//set lighting uniforms
	WeakLightArray lights;
	pScene->GetLightsInScene(lights);
	m_shaderProgram->SetLights(lights);
	m_shaderProgram->SetMaterial(m_material.get());
	//
	BindData();
	glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
	VRenderChildren(pScene);
}
// -----------------------------------------------------------------------
void Mesh_GL::BindData()
{
	int offset = 3 * m_numVertices*sizeof(GLfloat);
	m_vertexBuffer.SetVertexAttribPointer(m_shaderProgram->GetPositionID(), 3, 0, 0);
	m_vertexBuffer.SetVertexAttribPointer(m_shaderProgram->GetNormalID(), 3, 0, (const void*)offset);
	m_vertexBuffer.SetVertexAttribPointer(m_shaderProgram->GetTextureID(), 2, 0, (const void*)(offset * 2));
}
// -----------------------------------------------------------------------
//The messh object's Validate function is called by the Model container class
bool Mesh_GL::VValidate()
{
	return m_vertexBuffer.Validate();
}
// -----------------------------------------------------------------------
int Mesh_GL::VLoadMesh(aiMesh* pMesh, aiMaterial* pMaterial)
{
	//-------------Create Material-------------------
	aiColor4D color(0.f, 0.f, 0.f,0.f);

	m_material = StrongMaterialPtr(ALPHA_NEW Material_GL());
	//Diffuse
	pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	m_material->SetDiffuse(vec4(color.r, color.g, color.b , color.a));
	//Specular
	pMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
	float shininess;
	pMaterial->Get(AI_MATKEY_SHININESS, shininess);
	shininess = max(shininess, 1);
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
	int op = 0;
	pMaterial->Get(AI_MATKEY_BLEND_FUNC, op);
	string path = m_meshFileName;
	int end = path.find_last_of("/");
	path = path.substr(0, end + 1).append(textureLoc.C_Str());

	Texture_GL* tex = ALPHA_NEW Texture_GL(path.c_str());
	m_material->SetTexture(StrongTexturePtr(tex));
	//-----------------------------------------------
	//-------------Create VertexBuffer---------------
	//convert the texture coords to 2D
	vector<GLfloat> varray;
	vector<GLfloat> narray;
	vector<GLfloat> tarray;
	for (unsigned int i = 0; i < pMesh->mNumVertices; ++i)
	{
		varray.push_back(pMesh->mVertices[i].x);
		varray.push_back(pMesh->mVertices[i].y);
		varray.push_back(pMesh->mVertices[i].z);
	}

	for (unsigned int i = 0; i < pMesh->mNumVertices; ++i)
	{
		narray.push_back(pMesh->mNormals[i].x);
		narray.push_back(pMesh->mNormals[i].y);
		narray.push_back(pMesh->mNormals[i].z);
	}


	for (unsigned int i = 0; i < pMesh->mNumVertices; ++i)
	{
		tarray.push_back(pMesh->mTextureCoords[0][i].x);
		tarray.push_back(pMesh->mTextureCoords[0][i].y);
	}
	GLfloat* vectarr = &varray[0];
	GLfloat* normtarr = &narray[0];
	GLfloat* textarr = &tarray[0];
	m_numVertices = pMesh->mNumVertices;

	//initialize vertex buffer	
	return m_vertexBuffer.Init(m_numVertices, vectarr, normtarr, textarr,3,3,2, m_meshFileName);
}
// -----------------------------------------------------------------------
void Mesh_GL::VFreeBuffer()
{
	m_vertexBuffer.FreeVertexBuffer();
}
// -----------------------------------------------------------------------
bool Mesh_GL::VLoadMaterial()
{
	return m_material->LoadTexture();
}
// -----------------------------------------------------------------------