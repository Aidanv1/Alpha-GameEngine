#include "MeshShaderProgram.h"

// -----------------------------------------------------------------------
MeshShaderProgram::MeshShaderProgram() :
m_positionID(-1),
m_normalID(-1),
m_texCoordID(-1),
m_modelMatrixID(-1),
m_perspectiveMatrixID(-1),
m_viewMatrixID(-1),
m_texID(-1)
{
}
// -----------------------------------------------------------------------
MeshShaderProgram::~MeshShaderProgram()
{
}
// -----------------------------------------------------------------------
bool MeshShaderProgram::VInit(const char* vertexShaderSourceCode, const char* fragmentShaderSourceCode)
{
	if (!LightShaderProgram::VInit(vertexShaderSourceCode, fragmentShaderSourceCode))
	{
		return false;
	}	
	//attributes
	m_positionID = glGetAttribLocation(m_shaderProgramID, "i_Position");
	m_normalID = glGetAttribLocation(m_shaderProgramID, "i_Normal");
	m_texCoordID = glGetAttribLocation(m_shaderProgramID, "i_TexCoord");
	//uniforms
	m_perspectiveMatrixID = glGetUniformLocation(m_shaderProgramID, "u_P");
	m_viewMatrixID = glGetUniformLocation(m_shaderProgramID, "u_V");
	m_modelMatrixID = glGetUniformLocation(m_shaderProgramID, "u_M");
	m_texID = glGetUniformLocation(m_shaderProgramID, "texture");
	m_hasTextureID = glGetUniformLocation(m_shaderProgramID, "hasTexture");

	//if any ID is equal to -1 initialization failed
	if (m_positionID == -1 ||
		m_normalID == -1 ||
		m_texCoordID == -1 ||
		m_perspectiveMatrixID == -1 ||
		m_viewMatrixID == -1 ||
		m_modelMatrixID == -1 ||
		m_texID == -1 ||
		m_hasTextureID == -1 
		)
	{
		ALPHA_ASSERT(false);
		return false;
	}
	return true;
}
// -----------------------------------------------------------------------
void MeshShaderProgram::SetUniforms(mat4& M, mat4& V, mat4& P, GLuint textureid, bool hasTexture)
{
	glUniformMatrix4fv(m_modelMatrixID, 1, GL_FALSE, value_ptr(M));
	glUniformMatrix4fv(m_viewMatrixID, 1, GL_FALSE, value_ptr(V));
	glUniformMatrix4fv(m_perspectiveMatrixID, 1, GL_FALSE, value_ptr(P));

	GLuint hasT = 0;
	if (hasTexture)
	{
		hasT = 1;
	}
	glUniform1i(m_hasTextureID, hasT);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureid);
	//get maximum anisotropy
	float anisoMax = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoMax);
	float aniso = glm::min((int)anisoMax, GraphicsSettings::Anisotropy());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);
	glUniform1i(m_texID, 0);
}
// -----------------------------------------------------------------------