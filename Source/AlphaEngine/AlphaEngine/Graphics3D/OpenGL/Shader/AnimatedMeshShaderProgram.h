#pragma once
#include "MeshShaderProgram.h"
//========================================================================
class AnimatedMeshShaderProgram : public MeshShaderProgram
{
	static const int MAX_NUMBONES = 10;
public:
	AnimatedMeshShaderProgram();
	~AnimatedMeshShaderProgram();
	bool VInit(const char* vertexShaderSourceCode, const char* fragmentShaderSourceCode) override;
	void SetUniforms(mat4& M, mat4& V, mat4& P, GLuint textureid, bool hasTexture = true);
	void SetBoneUniforms(vector<mat4> boneOffsets);
	GLuint* GetBoneWeightsID(int& num) { num = MAX_NUMBONES; return m_boneWeightsID; }
private:
	GLuint m_boneOffsetsID[MAX_NUMBONES];
	GLuint m_numBonesID;
	GLuint m_boneWeightsID[MAX_NUMBONES];
};
//========================================================================