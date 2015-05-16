#include "SkyShaderProgram.h"
// -----------------------------------------------------------------------
SkyShaderProgram::SkyShaderProgram() :
m_positionID(-1),
m_texID(-1)
{
}
// -----------------------------------------------------------------------
SkyShaderProgram::~SkyShaderProgram()
{

}
// -----------------------------------------------------------------------
bool SkyShaderProgram::VInit(const char* vertexShaderSourceCode, const char* fragmentShaderSourceCode)
{
	if (!BaseShaderProgram::VInit(vertexShaderSourceCode, fragmentShaderSourceCode))
	{
		return false;
	}
	//attributes
	m_positionID = glGetAttribLocation(m_shaderProgramID, "a_Position");
	//uniforms
	m_matrixID = glGetUniformLocation(m_shaderProgramID, "u_Matrix");
	m_texID = glGetUniformLocation(m_shaderProgramID, "u_TextureUnit");
	//if any ID is equal to -1 initialization failed
	if (m_positionID == -1 ||
		m_matrixID == -1 ||
		m_texID == -1)
	{
		return false;
	}
	return true;
}
// -----------------------------------------------------------------------
void SkyShaderProgram::SetUniforms(mat4& M, GLuint textureid)
{
	glUniformMatrix4fv(m_matrixID, 1, GL_FALSE, value_ptr(M));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureid);
	glUniform1i(m_texID, 0);
}
// -----------------------------------------------------------------------