#include "BaseShaderProgram.h"

// -----------------------------------------------------------------------
BaseShaderProgram::BaseShaderProgram() :
m_shaderProgramID(-1)
{
}
// -----------------------------------------------------------------------
BaseShaderProgram::~BaseShaderProgram()
{
	ShaderHelper::DeleteShaderProgram(m_shaderProgramID);
}
// -----------------------------------------------------------------------
bool BaseShaderProgram::VInit(const char* vertexShaderSourceCode, const char* fragmentShaderSourceCode)
{
	m_shaderProgramID = ShaderHelper::MakeShaderProgram(vertexShaderSourceCode, fragmentShaderSourceCode);
	return m_shaderProgramID != -1;
}
// -----------------------------------------------------------------------
void BaseShaderProgram::VUseProgram()
{
	glUseProgram(m_shaderProgramID);
}
// -----------------------------------------------------------------------