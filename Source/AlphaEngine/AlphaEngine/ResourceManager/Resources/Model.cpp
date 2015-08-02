#include "Model.h"
// -----------------------------------------------------------------------
ModelBufferReader::ModelBufferReader(unsigned char* buffer) :
m_pBuffer(buffer)
{

}
// -----------------------------------------------------------------------
ModelBufferReader::ModelBufferReader() :
m_pBuffer(NULL)
{

}
// -----------------------------------------------------------------------
MeshInfo* ModelBufferReader::GetMeshInfoArray(int& size)
{
	int offset = 0;
	SizeInfo* sinfo = (SizeInfo*)m_pBuffer;
	size = sinfo->m_numberOfMeshes;
	offset = offsetof(Model, m_meshArray);
	return (MeshInfo*)&m_pBuffer[offset];
}
// -----------------------------------------------------------------------
void ModelBufferReader::operator=(unsigned char* buffer)
{
	m_pBuffer = buffer;
}
// -----------------------------------------------------------------------
AnimationInfo* ModelBufferReader::GetAnimationInfoArray(int& size)
{
	SizeInfo* sizeInfo = (SizeInfo*)m_pBuffer;
	int offset =	sizeof(SizeInfo) +
					sizeInfo->m_numberOfMeshes * sizeof(MeshInfo) +
					sizeInfo->m_numberOfBones * sizeof(BoneInfo);
	size = sizeInfo->m_numberOfAnimations;
	return (AnimationInfo*)&m_pBuffer[offset];
}
// -----------------------------------------------------------------------
BoneInfo* ModelBufferReader::GetBoneInfoArray(int& size)
{
	SizeInfo* sizeInfo = (SizeInfo*)m_pBuffer;
	int offset =	sizeof(SizeInfo) +
					sizeInfo->m_numberOfMeshes * sizeof(MeshInfo);
	size = sizeInfo->m_numberOfBones;
	return (BoneInfo*)&m_pBuffer[offset];
}