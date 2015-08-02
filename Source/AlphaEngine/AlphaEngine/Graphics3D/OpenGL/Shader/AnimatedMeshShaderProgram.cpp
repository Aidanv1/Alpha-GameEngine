#include "AnimatedMeshShaderProgram.h"
// -----------------------------------------------------------------------
AnimatedMeshShaderProgram::AnimatedMeshShaderProgram() :
MeshShaderProgram(),
m_numBonesID(-1)
{
	for (int i = 0; i < MAX_NUMBONES; i++)
	{
		m_boneOffsetsID[i] = -1;
		m_boneWeightsID[i] = -1;
	}
}
// -----------------------------------------------------------------------
AnimatedMeshShaderProgram::~AnimatedMeshShaderProgram()
{

}
// -----------------------------------------------------------------------
void AnimatedMeshShaderProgram::SetUniforms(mat4& M, mat4& V, mat4& P, GLuint textureid, bool hasTexture)
{
	MeshShaderProgram::SetUniforms(M, V,  P, textureid, hasTexture);
	//default number of bones is zero
	glUniform1i(m_numBonesID, 0);
}
// -----------------------------------------------------------------------
bool AnimatedMeshShaderProgram::VInit(const char* vertexShaderSourceCode, const char* fragmentShaderSourceCode)
{
	if (!MeshShaderProgram::VInit(vertexShaderSourceCode, fragmentShaderSourceCode))
	{
		return false;
	}
	m_numBonesID = glGetUniformLocation(m_shaderProgramID, "u_numBones");
	string boneOffsetStr = "u_BoneOffsets[";
	string boneWeightStr = "i_boneWeights[";
	for (int i = 0; i < MAX_NUMBONES; i++)
	{
		string strO = boneOffsetStr;
		strO.append(to_string(i)).append("]");
		string strW = boneWeightStr;
		strW.append(to_string(i)).append("]");
		m_boneOffsetsID[i] = glGetUniformLocation(m_shaderProgramID, strO.c_str());
		m_boneWeightsID[i] = glGetAttribLocation(m_shaderProgramID, strW.c_str());

		if (m_boneOffsetsID[i] == -1 ||
			m_boneWeightsID[i] == -1)
		{
			ALPHA_ASSERT(false);
			return false;
		}
		strO = "";
		strW = "";
	}
	return true;

}
// -----------------------------------------------------------------------
void AnimatedMeshShaderProgram::SetBoneUniforms(vector<mat4> boneOffsets)
{
	int numberBones = glm::min((int)boneOffsets.size(), MAX_NUMBONES);
	glUniform1i(m_numBonesID, numberBones);
	for (int i = 0; i < numberBones; i++)
	{
		glUniformMatrix4fv(m_boneOffsetsID[i], 1, GL_FALSE, value_ptr(boneOffsets.at(i)));
	}
}
// -----------------------------------------------------------------------
