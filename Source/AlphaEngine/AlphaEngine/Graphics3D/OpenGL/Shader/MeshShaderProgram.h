#pragma once
#include "..\..\..\AlphaStd.h"
#include <GL/glew.h>
#include "LightShaderProgram.h"
//========================================================================
class MeshShaderProgram : public LightShaderProgram
{
public:
	MeshShaderProgram();
	~MeshShaderProgram();
	virtual bool VInit(const char* vertexShaderSourceCode, const char* fragmentShaderSourceCode) override;
	void SetUniforms(mat4& M, mat4& V, mat4& P, GLuint textureid);
	//accessor methods for attribute locations
	GLuint GetPositionID() const { return m_positionID; }
	GLuint GetNormalID() const { return m_normalID; }
	GLuint GetTextureID() const { return m_texCoordID; }

private:
	GLuint m_positionID;
	GLuint m_normalID;
	GLuint m_texCoordID;
	GLuint m_modelMatrixID;
	GLuint m_perspectiveMatrixID;
	GLuint m_viewMatrixID;
	GLuint m_texID;

};
//========================================================================
