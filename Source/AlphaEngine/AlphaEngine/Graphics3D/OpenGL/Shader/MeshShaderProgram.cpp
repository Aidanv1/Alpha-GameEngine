#include "MeshShaderProgram.h"

// -----------------------------------------------------------------------
MeshShaderProgram::MeshShaderProgram() :
m_positionID(-1),
m_normalID(-1),
m_texCoordID(-1),
m_modelMatrixID(-1),
m_perspectiveMatrixID(-1),
m_viewMatrixID(-1),
m_allRotsMatrixID(-1),
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
	m_positionID = glGetAttribLocation(m_shaderProgramID, "a_Position");
	m_normalID = glGetAttribLocation(m_shaderProgramID, "a_Normal");
	m_texCoordID = glGetAttribLocation(m_shaderProgramID, "a_TexCoord");

	//uniforms
	m_perspectiveMatrixID = glGetUniformLocation(m_shaderProgramID, "mP");
	m_viewMatrixID = glGetUniformLocation(m_shaderProgramID, "mV");
	m_modelMatrixID = glGetUniformLocation(m_shaderProgramID, "mM");
	m_allRotsMatrixID = glGetUniformLocation(m_shaderProgramID, "mRotations");
	m_texID = glGetUniformLocation(m_shaderProgramID, "texture");

	//if any ID is equal to -1 initialization failed
	if (m_positionID == -1 ||
		m_normalID == -1 ||
		m_texCoordID == -1 ||
		m_perspectiveMatrixID == -1 ||
		m_viewMatrixID == -1 ||
		m_modelMatrixID == -1 ||
		m_allRotsMatrixID == -1 ||
		m_texID == -1)
	{
		ALPHA_ASSERT(false);
		return false;
	}
	return true;
}
// -----------------------------------------------------------------------
void MeshShaderProgram::SetUniforms(mat4& M, mat4& V, mat4& P, mat4& R, GLuint textureid)
{
	glUniformMatrix4fv(m_modelMatrixID, 1, GL_FALSE, value_ptr(M));
	glUniformMatrix4fv(m_viewMatrixID, 1, GL_FALSE, value_ptr(V));
	glUniformMatrix4fv(m_perspectiveMatrixID, 1, GL_FALSE, value_ptr(P));
	glUniformMatrix4fv(m_allRotsMatrixID, 1, GL_FALSE, value_ptr(R));



	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureid);
	//get maximum anisotropy
	float anisoMax = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoMax);
	float aniso = min(anisoMax, GraphicsSettings::Anisotropy());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);
	glUniform1i(m_texID, 0);
}
// -----------------------------------------------------------------------