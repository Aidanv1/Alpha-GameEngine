#include "LineMesh.h"
unsigned int LineMesh::s_LineMeshNumber;
LineMesh::LineMesh() :
m_shaderProgram(NULL),
m_numVertices(0),
m_vertexBuffer()
{
	stringstream ss;
	ss << "LineMesh." << s_LineMeshNumber++;
	m_LineMeshName = ss.str();	
	m_nodeProperties.m_toWorld = translate(mat4(1.0f), m_positionInWorld);
}
// -----------------------------------------------------------------------
LineMesh::~LineMesh()
{
}
// -----------------------------------------------------------------------
bool LineMesh::Init()
{
	BasicShaderProgram* pShaderProgram = dynamic_cast<GLRenderer*>(GraphicsSystem::Get().GetRenderer())->GetBasicShaderProgram();
	if (!pShaderProgram)
	{
		return false;
	}
	m_shaderProgram = pShaderProgram;
	return true;
}
// -----------------------------------------------------------------------
int LineMesh::LoadLineMesh(float vertexArray[], int numVertices)
{
	m_numVertices = numVertices;
	return m_vertexBuffer.Init(m_numVertices, vertexArray, 3, m_LineMeshName);
}
// -----------------------------------------------------------------------
void LineMesh::VRender(Scene* pScene)
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
									projMat	);
	BindData();
	glDepthFunc(GL_ALWAYS);
	glDrawArrays(GL_LINES, 0, m_numVertices);
	glDepthFunc(GL_LESS);
}
// -----------------------------------------------------------------------
void LineMesh::BindData()
{
	m_vertexBuffer.SetVertexAttribPointer(m_shaderProgram->GetPositionID(), 3, 0, 0);	
}
// -----------------------------------------------------------------------
void LineMesh::VUpdateNode(Scene* pScene, float deltaMS)
{

}
