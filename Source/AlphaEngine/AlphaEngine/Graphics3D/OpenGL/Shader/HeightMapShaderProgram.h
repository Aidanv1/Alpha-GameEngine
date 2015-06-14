#pragma once
#include "..\..\..\AlphaStd.h"
#include <GL/glew.h>
#include "LightShaderProgram.h"
//========================================================================
class HeightMapShaderProgram : public LightShaderProgram
{
public:
	HeightMapShaderProgram();
	~HeightMapShaderProgram();
	virtual bool VInit(const char* vertexShaderSourceCode, const char* fragmentShaderSourceCode) override;
	void SetUniforms(mat4& M, mat4& V, mat4& P, GLuint textureid);
	//accessor methods for attribute locations
	int GetPositionID() const { return m_positionID; }
	int GetNormalID() const { return m_normalID; }
	int GetTextureID() const { return m_texCoordID; }

private:
	int m_positionID;
	int m_normalID;
	int m_texCoordID;
	int m_modelMatrixID;
	int m_perspectiveMatrixID;
	int m_viewMatrixID;
	int m_texID;
};
//========================================================================
