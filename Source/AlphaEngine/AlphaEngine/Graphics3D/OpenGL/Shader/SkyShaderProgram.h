#pragma once
#include "..\..\..\AlphaStd.h"
#include <GL/glew.h>
#include "BaseShaderProgram.h"
//========================================================================
class SkyShaderProgram : public BaseShaderProgram
{
public:
	SkyShaderProgram();
	~SkyShaderProgram();
	virtual bool VInit(const char* vertexShaderSourceCode, const char* fragmentShaderSourceCode) override;
	void SetUniforms(mat4& M, GLuint textureid);
	//accessor methods for attribute locations
	GLuint GetPositionID() const { return m_positionID; }
private:
	GLuint m_positionID;
	GLuint m_matrixID;
	GLuint m_texID;
};
//========================================================================
