#include "ShaderHelper.h"
//preprocessor definitions
const string ShaderHelper::c_INCLUDE = "#include";
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
	//Preproc--
	string shaderSourceContents = ReadFile(shaderSource);

	//get file path
	string filePath = shaderSource;
	int end = filePath.find_last_of("/");
	string path = filePath.substr(0, end + 1);
	shaderSourceContents = Preprocessor(shaderSourceContents.c_str(), path);
	//-------
	const char* source = shaderSourceContents.c_str();
	glShaderSource(vertexShaderID, 1, (const GLchar**)&source, NULL);
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
	//Preproc--
	const char* contents = ReadFile(shaderSource);
	string shaderSourceContents = contents;
	//get file path
	string filePath = shaderSource;
	int end = filePath.find_last_of("/");
	string path = filePath.substr(0, end + 1);
	shaderSourceContents = Preprocessor(shaderSourceContents.c_str(), path);
	//-------
	const char* source = shaderSourceContents.c_str();
	glShaderSource(fragmentShaderID, 1, (const GLchar**)&source, NULL);
	glCompileShader(fragmentShaderID);
	bool compiledCorrectly = CompiledStatus(fragmentShaderID);
	if (compiledCorrectly) 
	{
		return fragmentShaderID;
	}
	SAFE_DELETE(source);
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
string ShaderHelper::Preprocessor(string shaderCode, string filePath)
{
	string code = shaderCode;
	string procCode;
	std::size_t found = code.find(c_INCLUDE);
	if (found != std::string::npos)
	{
		//code before preprocessor
		string firstPart = code.substr(0, found);	
		int codeIndex = found + c_INCLUDE.size();
		int openIndex = code.find("<", codeIndex);
		int closeIndex = code.find(">", openIndex);
		//continued code after preprocessor
		string secondPart = code.substr(closeIndex + 1);
		string nextFileName = code.substr(openIndex + 1, closeIndex - openIndex -1);		
		string nextFile = ReadFile(filePath.append(nextFileName).c_str());
		string included = Preprocessor(nextFile, filePath);
		code = firstPart.append(included).append(secondPart);
	}

	return code.c_str();
}
// -----------------------------------------------------------------------