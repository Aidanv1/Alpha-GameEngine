#ifndef BASESHADERPROGRAM_H
#define BASESHADERPROGRAM_H
#include "IShaderProgram.h"
#include "ShaderHelper.h"
//========================================================================
class BaseShaderProgram : IShaderProgram
{
public:
	BaseShaderProgram();
	~BaseShaderProgram();
	virtual bool VInit(const char* vertexShaderSourceCode, const char* fragmentShaderSourceCode) override;	
	virtual void VUseProgram() override;
	virtual GLuint VGetShaderProgramID() override { return m_shaderProgramID; }
	
protected:
	GLuint m_shaderProgramID;

};
//========================================================================
#endif