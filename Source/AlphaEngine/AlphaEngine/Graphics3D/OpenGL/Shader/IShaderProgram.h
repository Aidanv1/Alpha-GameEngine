#pragma once
#include <GL/glew.h>
//========================================================================
class IShaderProgram
{
	virtual bool VInit(const char* vertexShaderSourceCode, const char* fragmentShaderSourceCode) = 0;
	virtual void VUseProgram() = 0;
	virtual GLuint VGetShaderProgramID() = 0;
};
//========================================================================
