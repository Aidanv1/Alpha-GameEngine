#include "Text2DShaderProgram.h"
// -----------------------------------------------------------------------
Text2DShaderProgram::Text2DShaderProgram() :
m_positionID(-1),
m_texCoordID(-1),
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
	//if any ID is equal to -1 initialization failed
	if (m_positionID == -1 ||	
		m_texCoordID == -1 ||		
		m_texID == -1)
	{
		ALPHA_ASSERT(false);
		return false;
	}
	return true;
}
// -----------------------------------------------------------------------
void Text2DShaderProgram::SetUniforms(GLfloat size, GLuint textureid)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureid);
	glUniform1i(m_texID, 0);
}
// -----------------------------------------------------------------------