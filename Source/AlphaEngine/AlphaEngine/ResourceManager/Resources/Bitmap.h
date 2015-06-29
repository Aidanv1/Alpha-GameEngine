#pragma once
#include "../../AlphaStd.h"
///***************************DEFINITIONS*********************************
//BITMAP BUFFER DEFINITIONS
#define BMPINFO_SIZE					3 * sizeof(int)
#define BMPSIZE_LOC						0
#define BMPHEIGHT_LOC					sizeof(int)
#define BMPWIDTH_LOC					BMPHEIGHT_LOC + sizeof(int)
#define BMPPIXELS_LOC					BMPWIDTH_LOC + sizeof(int)
///*****************************STRUCTS***********************************
struct BitmapInfo
{
	int m_height;
	int m_width;
	int m_size;
};
//========================================================================
//BITMAP BUFFER READER
class BitMapBufferReader
{
public:
	BitMapBufferReader(unsigned char* buffer);
	BitMapBufferReader();
	BitmapInfo GetInfo();
	unsigned char* PixelData();
	void operator=(unsigned char* buffer);
private:
	unsigned char* m_pBuffer;
};
