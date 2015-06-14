#include "Raw.h"
//========================================================================
//BITMAP BUFFER READER
BitMapBufferReader::BitMapBufferReader(unsigned char* buffer) :
m_pBuffer(buffer)
{
}
// -----------------------------------------------------------------------
BitmapInfo BitMapBufferReader::GetInfo()
{
	BitmapInfo info;
	info.m_size = *m_pBuffer;
	info.m_height = *(m_pBuffer + BMPHEIGHT_LOC);
	info.m_width = *(m_pBuffer + BMPWIDTH_LOC);
	return info;
}
// -----------------------------------------------------------------------
unsigned char* BitMapBufferReader::PixelData()
{
	return m_pBuffer + BMPPIXELS_LOC;
}
//========================================================================