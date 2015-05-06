#ifndef MESHSHADERPROGRAM_H
#define MESHSHADERPROGRAM_H
#include "..\..\..\AlphaStd.h"
#include <GL/glew.h>
#include "BaseShaderProgram.h"
//========================================================================
class MeshShaderProgram : public BaseShaderProgram
{
public:
	MeshShaderProgram();
	~MeshShaderProgram();
	virtual bool VInit(const char* vertexShaderSourceCode, const char* fragmentShaderSourceCode) override;
	void SetUniforms(mat4& M, mat4& V, mat4& P, mat4& R, vec4& light, GLuint textureid);
	//accessor methods for attribute locations
	GLuint GetPositionID() const { return m_positionID; }
	GLuint GetNormalID() const { return m_normalID; }
	GLuint GetTextureID() const { return m_texCoordID; }

private:
	GLuint m_positionID;
	GLuint m_normalID;
	GLuint m_texCoordID;
	GLuint m_lightID;
	GLuint m_modelMatrixID;
	GLuint m_perspectiveMatrixID;
	GLuint m_viewMatrixID;
	GLuint m_allRotsMatrixID;
	GLuint m_texID;

};
//========================================================================

#endif