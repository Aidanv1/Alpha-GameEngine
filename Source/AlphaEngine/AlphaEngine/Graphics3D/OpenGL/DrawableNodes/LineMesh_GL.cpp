#include "LineMesh_GL.h"
unsigned int LineMesh_GL::s_LineMeshNumber;
LineMesh_GL::LineMesh_GL() :
m_shaderProgram(NULL),
m_numVertices(0),
m_vertexBuffer()
{
	stringstream ss;
	ss << "LineMesh_GL." << s_LineMeshNumber++;
	m_LineMeshName = ss.str();	
	m_nodeProperties.m_toWorld = translate(mat4(1.0f), m_positionInWorld);
}
// -----------------------------------------------------------------------
LineMesh_GL::~LineMesh_GL()
{
}
// -----------------------------------------------------------------------
bool LineMesh_GL::VInitLineMesh()
{
	BasicShaderProgram* pShaderProgram = dynamic_cast<Renderer_GL*>(GraphicsSystem::Get().GetRenderer())->GetBasicShaderProgram();
	if (!pShaderProgram)
	{
		return false;
	}
	m_shaderProgram = pShaderProgram;
	return true;
}
// -----------------------------------------------------------------------
int LineMesh_GL::VLoadLineMesh(float vertexArray[], int numVertices)
{
	m_numVertices = numVertices;
	return m_vertexBuffer.Init(m_numVertices, vertexArray, 3, m_LineMeshName);
}
// -----------------------------------------------------------------------
void LineMesh_GL::VRender(Scene* pScene)
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
void LineMesh_GL::BindData()
{
	m_vertexBuffer.SetVertexAttribPointer(m_shaderProgram->GetPositionID(), 3, 0, 0);	
}
// -----------------------------------------------------------------------
void LineMesh_GL::VUpdateNode(Scene* pScene, float deltaMS)
{

}
