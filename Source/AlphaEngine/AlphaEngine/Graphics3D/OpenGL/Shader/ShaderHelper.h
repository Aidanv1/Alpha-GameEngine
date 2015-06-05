#pragma once
#include "..\..\..\AlphaStd.h"
#include <GL/glew.h>
//========================================================================
class ShaderHelper
{
	//preprocessor constants
	static const string c_INCLUDE;
public:
	static GLuint MakeShaderProgram(const char* vertexShaderSourceCode, const char* fragmentShaderSourceCode);
	static void DeleteShaderProgram(GLuint programID);
private:
	static GLuint MakeVertexShader(const char* vertexShaderSourceCode);
	static GLuint MakeFragmentShader(const char* fragmentShaderSourceCode);
	static string Preprocessor(string shaderCode, string filePath);
	static bool CompiledStatus(GLint shaderID);
};
//========================================================================
