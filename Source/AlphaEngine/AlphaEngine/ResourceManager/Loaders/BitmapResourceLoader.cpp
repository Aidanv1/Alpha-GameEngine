#include "BitmapResourceLoader.h"
#include "IL/il.h"
#include "IL/ilu.h"
#include <IL/ilut.h>
#include "..\Resources\Bitmap.h"
BitmapResourceLoader::BitmapResourceLoader()
{

}
BitmapResourceLoader::~BitmapResourceLoader()
{

}
bool BitmapResourceLoader::VLoadResource(string resName, unsigned char*& pBuffer, unsigned int& size)
{
	unsigned char* pBmpBuffer;
	if (!ilLoadImage(resName.c_str()))
	{
		stringstream ss;
		ss << "Error reading texture file: ";
		ALPHA_ERROR(ss.str().c_str());
		return false;
	}

	int width = ilGetInteger(IL_IMAGE_WIDTH);
	int height = ilGetInteger(IL_IMAGE_HEIGHT);
	int dataSize = (width * height * 4);
	size = dataSize + BMPINFO_SIZE;

	pBmpBuffer = ALPHA_NEW unsigned char[size];
	for (unsigned int i = 0; i < size; i++)
	{
		pBmpBuffer[i] = NULL;
	}
	int* pHeightLoc = (int*) &pBmpBuffer[BMPHEIGHT_LOC];
	int* pWidthLoc = (int*)&pBmpBuffer[BMPWIDTH_LOC];
	int* pSizeLoc = (int*)&pBmpBuffer[BMPSIZE_LOC];
	*pHeightLoc = height;
	*pWidthLoc = width;
	*pSizeLoc = size;

	unsigned char* pixels = pBmpBuffer + BMPPIXELS_LOC;
	ILinfo ImageInfo;
	iluGetImageInfo(&ImageInfo);
	if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
	{
		iluFlipImage();
	}
	ilCopyPixels(0, 0, 0, width, height, 1, IL_BGRA, IL_UNSIGNED_BYTE, pixels);
	pBuffer = (unsigned char*)pBmpBuffer;
	return true;
}
unsigned int BitmapResourceLoader::VGetSize(string resName)
{
	unsigned int size = FileSize(resName.c_str());
	return size;
}