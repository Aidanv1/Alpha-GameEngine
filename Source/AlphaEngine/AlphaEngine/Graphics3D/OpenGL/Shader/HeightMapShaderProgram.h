#ifndef HEIGHTSHADERPROGRAM_H
#define HEIGHTSHADERPROGRAM_H
#include "..\..\..\AlphaStd.h"
#include <GL/glew.h>
#include "BaseShaderProgram.h"
//========================================================================
class HeightMapShaderProgram : public BaseShaderProgram
{
public:
	HeightMapShaderProgram();
	~HeightMapShaderProgram();
	virtual bool VInit(const char* vertexShaderSourceCode, const char* fragmentShaderSourceCode) override;
	void SetUniforms(mat4& M, mat4& V, mat4& P, mat4& R, vec4& light, GLuint textureid);
	//accessor methods for attribute locations
	int GetPositionID() const { return m_positionID; }
	int GetNormalID() const { return m_normalID; }
	int GetTextureID() const { return m_texCoordID; }

private:
	int m_positionID;
	int m_normalID;
	int m_texCoordID;
	int m_lightID;
	int m_modelMatrixID;
	int m_perspectiveMatrixID;
	int m_viewMatrixID;
	int m_allRotsMatrixID;
	int m_texID;
};
//========================================================================
#endif