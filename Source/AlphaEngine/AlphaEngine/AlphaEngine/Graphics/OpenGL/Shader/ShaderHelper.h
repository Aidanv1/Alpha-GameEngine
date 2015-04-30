#ifndef SHADERHELPER_H
#define SHADERHELPER_H
#include "..\..\..\AlphaStd.h"
#include <GL/glew.h>
//========================================================================
class ShaderHelper
{
public:
	static GLuint MakeShaderProgram(const char* vertexShaderSourceCode, const char* fragmentShaderSourceCode);
	static void DeleteShaderProgram(GLuint programID);
private:
	static GLuint MakeVertexShader(const char* vertexShaderSourceCode);
	static GLuint MakeFragmentShader(const char* fragmentShaderSourceCode);
	static bool CompiledStatus(GLint shaderID);

private:
	static char* readFile(const char* filename);
};
//========================================================================
#endif