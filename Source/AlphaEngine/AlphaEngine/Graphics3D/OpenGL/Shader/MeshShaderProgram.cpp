#include "MeshShaderProgram.h"

// -----------------------------------------------------------------------
MeshShaderProgram::MeshShaderProgram() :
m_positionID(-1),
m_normalID(-1),
m_texCoordID(-1),
m_lightID(-1),
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
	if (!BaseShaderProgram::VInit(vertexShaderSourceCode, fragmentShaderSourceCode))
	{
		return false;
	}	
	//attributes
	m_positionID = glGetAttribLocation(m_shaderProgramID, "s_vPosition");
	m_normalID = glGetAttribLocation(m_shaderProgramID, "s_vNormal");
	m_lightID = glGetUniformLocation(m_shaderProgramID, "vLight");
	m_texCoordID = glGetAttribLocation(m_shaderProgramID, "s_vTexCoord");

	//uniforms
	m_perspectiveMatrixID = glGetUniformLocation(m_shaderProgramID, "mP");
	m_viewMatrixID = glGetUniformLocation(m_shaderProgramID, "mV");
	m_modelMatrixID = glGetUniformLocation(m_shaderProgramID, "mM");
	m_allRotsMatrixID = glGetUniformLocation(m_shaderProgramID, "mRotations");
	m_texID = glGetUniformLocation(m_shaderProgramID, "texture");

	//if any ID is equal to -1 initialization failed
	if (m_positionID == -1 ||
		m_normalID == -1 ||
		m_lightID == -1 ||
		m_texCoordID == -1 ||
		m_perspectiveMatrixID == -1 ||
		m_viewMatrixID == -1 ||
		m_modelMatrixID == -1 ||
		m_allRotsMatrixID == -1 ||
		m_texID == -1)
	{
		return false;
	}
	return true;
}
// -----------------------------------------------------------------------
void MeshShaderProgram::SetUniforms(mat4& M, mat4& V, mat4& P, mat4& R, vec4& light, GLuint textureid)
{
	glUniformMatrix4fv(m_modelMatrixID, 1, GL_FALSE, value_ptr(M));
	glUniformMatrix4fv(m_viewMatrixID, 1, GL_FALSE, value_ptr(V));
	glUniformMatrix4fv(m_perspectiveMatrixID, 1, GL_FALSE, value_ptr(P));
	glUniformMatrix4fv(m_allRotsMatrixID, 1, GL_FALSE, value_ptr(R));
	glUniform4fv(m_lightID, 1, value_ptr(light));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureid);
	glUniform1i(m_texID, 0);
}
// -----------------------------------------------------------------------