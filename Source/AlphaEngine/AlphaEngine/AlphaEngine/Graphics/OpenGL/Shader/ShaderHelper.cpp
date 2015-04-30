#include "ShaderHelper.h"
// -----------------------------------------------------------------------
char* ShaderHelper::readFile(const char* filename)
{
	// Open the file
	FILE* fp;
	fopen_s(&fp, filename, "r");


	// Move the file pointer to the end of the file and determing the length
	fseek(fp, 0, SEEK_END);
	long file_length = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* contents = new char[file_length + 1];
	// zero out memory
	for (int i = 0; i < file_length + 1; i++) {
		contents[i] = 0;
	}
	// Here's the actual read
	fread(contents, 1, file_length, fp);
	// This is how you denote the end of a string in C
	contents[file_length + 1] = '\0';
	fclose(fp);
	return contents;
}
// -----------------------------------------------------------------------
bool ShaderHelper::CompiledStatus(GLint shaderID){
	GLint compiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);
	if (compiled) {
		return true;
	}
	else {
		GLint logLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
		char* msgBuffer = ALPHA_NEW char[logLength];
		glGetShaderInfoLog(shaderID, logLength, NULL, msgBuffer);
		delete (msgBuffer);
		return false;
	}
}
// -----------------------------------------------------------------------
GLuint ShaderHelper::MakeVertexShader(const char* shaderSource) {
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (vertexShaderID == -1)
	{
		//error
	}
	const char* shaderSourceContents = readFile(shaderSource);
	glShaderSource(vertexShaderID, 1, (const GLchar**)&shaderSourceContents, NULL);
	glCompileShader(vertexShaderID);
	bool compiledCorrectly = CompiledStatus(vertexShaderID);
	if (compiledCorrectly) 
	{
		return vertexShaderID;
	}
	return -1;
}
// -----------------------------------------------------------------------
GLuint ShaderHelper::MakeFragmentShader(const char* shaderSource) {
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (fragmentShaderID == -1)
	{
		//error
	}
	const char* shaderSourceContents = readFile(shaderSource);
	glShaderSource(fragmentShaderID, 1, (const GLchar**)&shaderSourceContents, NULL);
	glCompileShader(fragmentShaderID);
	bool compiledCorrectly = CompiledStatus(fragmentShaderID);
	if (compiledCorrectly) 
	{
		return fragmentShaderID;
	}
	return -1;
}
// -----------------------------------------------------------------------
GLuint ShaderHelper::MakeShaderProgram(const char* vertexShaderSourceCode, const char* fragmentShaderSourceCode) {
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