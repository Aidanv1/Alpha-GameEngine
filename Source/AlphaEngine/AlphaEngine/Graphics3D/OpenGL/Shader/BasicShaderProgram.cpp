#include "BasicShaderProgram.h"

// -----------------------------------------------------------------------
BasicShaderProgram::BasicShaderProgram() :
m_positionID(-1),
m_modelMatrixID(-1),
m_perspectiveMatrixID(-1),
m_viewMatrixID(-1)
{
}
// -----------------------------------------------------------------------
BasicShaderProgram::~BasicShaderProgram()
{
}
// -----------------------------------------------------------------------
bool BasicShaderProgram::VInit(const char* vertexShaderSourceCode, const char* fragmentShaderSourceCode)
{
	if (!BaseShaderProgram::VInit(vertexShaderSourceCode, fragmentShaderSourceCode))
	{
		return false;
	}
	//attributes
	m_positionID = glGetAttribLocation(m_shaderProgramID, "a_Position");
	//uniforms
	m_perspectiveMatrixID = glGetUniformLocation(m_shaderProgramID, "mP");
	m_viewMatrixID = glGetUniformLocation(m_shaderProgramID, "mV");
	m_modelMatrixID = glGetUniformLocation(m_shaderProgramID, "mM");
	
	//if any ID is equal to -1 initialization failed
	if (m_positionID == -1 ||		
		m_perspectiveMatrixID == -1 ||
		m_viewMatrixID == -1 ||
		m_modelMatrixID == -1)		
	{
		ALPHA_ASSERT(false);
		return false;
	}
	return true;
}
// -----------------------------------------------------------------------
void BasicShaderProgram::SetUniforms(mat4& M, mat4& V, mat4& P)
{
	glUniformMatrix4fv(m_modelMatrixID, 1, GL_FALSE, value_ptr(M));
	glUniformMatrix4fv(m_viewMatrixID, 1, GL_FALSE, value_ptr(V));
	glUniformMatrix4fv(m_perspectiveMatrixID, 1, GL_FALSE, value_ptr(P));	
}
// -----------------------------------------------------------------------