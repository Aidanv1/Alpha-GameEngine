#include "Mesh_GL.h"
// -----------------------------------------------------------------------
Mesh_GL::Mesh_GL() :
m_numVertices(0),
m_meshFileName(""),
m_material(NULL),
m_shaderProgram(NULL),
m_vertexBuffer(),
m_meshID(-1),
m_cullFace(false)
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
	//push to stack
	mat4 toWorldTransform = pScene->Stack()->Top() * m_nodeProperties.m_toWorld;
	pScene->Stack()->Push(toWorldTransform);

	m_shaderProgram->VUseProgram();
	//get view and projection matrix from main camera
	mat4 viewMat;
	mat4 projMat;
	pScene->GetCamera()->GetViewMatrix(viewMat);
	pScene->GetCamera()->GetProjectionMatrix(projMat);
	//set shader uniforms
	m_shaderProgram->SetUniforms(	toWorldTransform,
									viewMat,
									projMat,
									m_material->Texture()->VGetTextureID());
	//set lighting uniforms
	WeakLightArray lights;
	pScene->GetLightsInScene(lights);
	m_shaderProgram->SetLights(lights);
	m_shaderProgram->SetMaterial(m_material.get());
	//
	BindData();
	if (m_cullFace)
	{
		glEnable(GL_CULL_FACE);
		glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
		glDisable(GL_CULL_FACE);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
	}




	VRenderChildren(pScene);

	//pop from stack
	pScene->Stack()->Pop();
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
int Mesh_GL::VLoadMesh(MeshInfo* pMesh)
{
	m_numVertices = pMesh->m_numberOfVertices;
	//create material---
	m_material = StrongMaterialPtr(ALPHA_NEW Material_GL());

	string path = m_meshFileName;
	int end = path.find_last_of("/");
	string textureLoc = pMesh->m_materialInfo.m_diffuseTexture.ToString();
	path = path.substr(0, end + 1).append(textureLoc.c_str());

	Texture_GL* tex = ALPHA_NEW Texture_GL(path.c_str());
	m_material->SetTexture(StrongTexturePtr(tex));
	m_material->SetAmbient(pMesh->m_materialInfo.m_ambient);
	m_material->SetEmissive(pMesh->m_materialInfo.m_emissive);
	m_material->SetDiffuse(pMesh->m_materialInfo.m_diffuse);
	m_material->SetSpecular(pMesh->m_materialInfo.m_specular, pMesh->m_materialInfo.m_shininess);
	//---
	//transform 
	m_nodeProperties.m_toWorld = pMesh->m_transform;
	//initialize vertex buffer	
	return m_vertexBuffer.Init(pMesh->m_dataSize, pMesh->m_data, m_meshFileName);
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
void Mesh_GL::VCullFace(bool cull)
{
	m_cullFace = cull;
}