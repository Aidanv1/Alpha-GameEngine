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
	int offset = offsetof(Model, m_numberOfMeshes);
	size = *(int*)&m_pBuffer[offset];
	offset = offsetof(Model, m_meshArray);
	return (MeshInfo*)&m_pBuffer[offset];
}
// -----------------------------------------------------------------------
void ModelBufferReader::operator=(unsigned char* buffer)
{
	m_pBuffer = buffer;
}