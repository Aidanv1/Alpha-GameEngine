#include "Bitmap.h"
//========================================================================
//BITMAP BUFFER READER
BitMapBufferReader::BitMapBufferReader(unsigned char* buffer) :
m_pBuffer(buffer)
{
}
// -----------------------------------------------------------------------
BitMapBufferReader::BitMapBufferReader()
{

}
// -----------------------------------------------------------------------
BitmapInfo BitMapBufferReader::GetInfo()
{
	BitmapInfo info;
	info.m_size = *(int*)&m_pBuffer[0];
	info.m_height = *(int*)&m_pBuffer[BMPHEIGHT_LOC];
	info.m_width = *(int*)&m_pBuffer[BMPWIDTH_LOC];
	return info;
}
// -----------------------------------------------------------------------
unsigned char* BitMapBufferReader::PixelData()
{
	return m_pBuffer + BMPPIXELS_LOC;
}
// -----------------------------------------------------------------------
void BitMapBufferReader::operator=(unsigned char* buffer)
{
	m_pBuffer = buffer;
}
//========================================================================