#include "Text2DShaderProgram.h"
// -----------------------------------------------------------------------
Text2DShaderProgram::Text2DShaderProgram() :
m_positionID(-1),
m_texCoordID(-1),
m_xID(-1),
m_yID(-1),
m_sizeID(-1),
m_texID(-1)
{
}
// -----------------------------------------------------------------------
Text2DShaderProgram::~Text2DShaderProgram()
{

}
// -----------------------------------------------------------------------
bool Text2DShaderProgram::VInit(const char* vertexShaderSourceCode, const char* fragmentShaderSourceCode)
{
	if (!BaseShaderProgram::VInit(vertexShaderSourceCode, fragmentShaderSourceCode))
	{
		return false;
	}
	//attributes
	m_positionID = glGetAttribLocation(m_shaderProgramID, "s_vPosition");	
	m_texCoordID = glGetAttribLocation(m_shaderProgramID, "s_vTexCoord");

	//uniforms
	m_texID = glGetUniformLocation(m_shaderProgramID, "texture");
	//m_xID = glGetUniformLocation(m_shaderProgramID, "x_offset");
	//m_yID = glGetUniformLocation(m_shaderProgramID, "y_offset");
	//m_sizeID = glGetUniformLocation(m_shaderProgramID, "size");
	//if any ID is equal to -1 initialization failed
	if (m_positionID == -1 ||	
		m_texCoordID == -1 ||		
		m_texID == -1)
	{
		return false;
	}
	return true;
}
// -----------------------------------------------------------------------
void Text2DShaderProgram::SetUniforms(GLfloat x, GLfloat y, GLfloat size, GLuint textureid)
{
	//glUniform1i(m_xID, x);
	//glUniform1i(m_yID, y);
	//glUniform1i(m_sizeID, size);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureid);
	glUniform1i(m_texID, 0);
}
// -----------------------------------------------------------------------