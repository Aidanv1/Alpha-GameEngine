#include "ShaderHelper.h"

// -----------------------------------------------------------------------
bool ShaderHelper::CompiledStatus(GLint shaderID)
{
	GLint compiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);
	if (compiled)
	{
		return true;
	}
	else 
	{
		GLint logLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
		char* msgBuffer = ALPHA_NEW char[logLength];
		glGetShaderInfoLog(shaderID, logLength, NULL, msgBuffer);
		ALPHA_ERROR(msgBuffer);
		SAFE_DELETE_ARRAY(msgBuffer);
		return false;
	}
}
// -----------------------------------------------------------------------
GLuint ShaderHelper::MakeVertexShader(const char* shaderSource) 
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (vertexShaderID == -1)
	{
		ALPHA_ERROR("Error creating vertex shader");
	}
	const char* shaderSourceContents = ReadFile(shaderSource);
	glShaderSource(vertexShaderID, 1, (const GLchar**)&shaderSourceContents, NULL);
	glCompileShader(vertexShaderID);
	bool compiledCorrectly = CompiledStatus(vertexShaderID);
	if (compiledCorrectly) 
	{
		return vertexShaderID;
	}
	ALPHA_ERROR("Error creating vertex shader");
	return -1;
}
// -----------------------------------------------------------------------
GLuint ShaderHelper::MakeFragmentShader(const char* shaderSource)
{
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (fragmentShaderID == -1)
	{
		ALPHA_ERROR("Error creating fragment shader");
	}
	const char* shaderSourceContents = ReadFile(shaderSource);
	glShaderSource(fragmentShaderID, 1, (const GLchar**)&shaderSourceContents, NULL);
	glCompileShader(fragmentShaderID);
	bool compiledCorrectly = CompiledStatus(fragmentShaderID);
	if (compiledCorrectly) 
	{
		return fragmentShaderID;
	}
	ALPHA_ERROR("Error creating fragment shader");
	return -1;
}
// -----------------------------------------------------------------------
GLuint ShaderHelper::MakeShaderProgram(const char* vertexShaderSourceCode, const char* fragmentShaderSourceCode) 
{
	GLuint shaderID = glCreateProgram();
	glAttachShader(shaderID, MakeVertexShader(vertexShaderSourceCode));
	glAttachShader(shaderID, MakeFragmentShader(fragmentShaderSourceCode));

	glLinkProgram(shaderID);
	return shaderID;
}
// -----------------------------------------------------------------------
void ShaderHelper::DeleteShaderProgram(GLuint programID)
{
	glDeleteProgram(programID);
}
// -----------------------------------------------------------------------