#pragma once
#include "..\..\..\AlphaStd.h"
#include <GL/glew.h>
#include "BaseShaderProgram.h"
//========================================================================
class BasicShaderProgram : public BaseShaderProgram
{
public:
	BasicShaderProgram();
	~BasicShaderProgram();
	virtual bool VInit(const char* vertexShaderSourceCode, const char* fragmentShaderSourceCode) override;
	void SetUniforms(mat4& M, mat4& V, mat4& P, vec4& colour);
	//accessor methods for attribute locations
	GLuint GetPositionID() const { return m_positionID; }
private:
	GLuint m_positionID;
	GLuint m_modelMatrixID;
	GLuint m_perspectiveMatrixID;
	GLuint m_viewMatrixID;
	GLuint m_colourID;
};
//========================================================================
