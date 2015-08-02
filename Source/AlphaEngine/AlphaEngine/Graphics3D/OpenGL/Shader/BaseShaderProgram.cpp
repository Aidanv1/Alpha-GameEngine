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
	//debugging
	GLint nUniforms, maxLen;
	GLfloat maxNum;
	
	glGetFloatv(GL_MAX_UNIFORM_LOCATIONS, &maxNum);
	glGetProgramiv(m_shaderProgramID, GL_ACTIVE_UNIFORM_MAX_LENGTH,	&maxLen);
	glGetProgramiv(m_shaderProgramID, GL_ACTIVE_UNIFORMS, &nUniforms);
	GLchar * name = (GLchar *)malloc(maxLen);
	GLint size, location;
	GLsizei written;
	GLenum type;
	stringstream ss;
	printf(" Location | Name\n");
	printf("------------------------------------------------\n");
	for (int i = 0; i < nUniforms; ++i) {
		glGetActiveUniform(m_shaderProgramID, i, maxLen, &written,
			&size, &type, name);
		location = glGetUniformLocation(m_shaderProgramID, name);
		printf(" %-8d | %s\n", location, name);
	}
	free(name);
	//
	return m_shaderProgramID != -1;
}
// -----------------------------------------------------------------------
void BaseShaderProgram::VUseProgram()
{
	glUseProgram(m_shaderProgramID);
}
// -----------------------------------------------------------------------