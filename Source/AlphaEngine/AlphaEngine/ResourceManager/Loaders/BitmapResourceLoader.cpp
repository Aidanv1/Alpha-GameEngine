#include "BitmapResourceLoader.h"

BitmapResourceLoader::BitmapResourceLoader()
{

}
BitmapResourceLoader::~BitmapResourceLoader()
{

}
string BitmapResourceLoader::VGetPattern()
{
	return "bmp";
}
bool BitmapResourceLoader::VLoadResource(string resName, unsigned char*& pBuffer, unsigned int& size)
{
	BmpData* pBmpBuffer = new BmpData();
	if (!ilLoadImage(resName.c_str()))
	{
		//auto errorEnum = ilGetError();
		//iluErrorString(errorEnum);
		stringstream ss;
		ss << "Error reading texture file: ";
		ALPHA_ERROR(ss.str().c_str());
		return false;
	}
//	size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);

	pBmpBuffer->width = ilGetInteger(IL_IMAGE_WIDTH);
	pBmpBuffer->height = ilGetInteger(IL_IMAGE_HEIGHT);
	size = pBmpBuffer->width * pBmpBuffer->height * 4;
	unsigned char* pixels = new unsigned char[size];
	ILinfo ImageInfo;
	iluGetImageInfo(&ImageInfo);
	if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
	{
		iluFlipImage();
	}
	ilCopyPixels(0, 0, 0, pBmpBuffer->width, pBmpBuffer->height, 1, IL_BGRA, IL_UNSIGNED_BYTE, pixels);

	// Make pixel_data point to the pixels	
	(pBmpBuffer->pixel_data) = shared_ptr<unsigned char>(pixels);
	pBmpBuffer->size = size;

	pBuffer = (unsigned char*)pBmpBuffer;
	return true;
}
unsigned int BitmapResourceLoader::VGetSize(string resName)
{
	unsigned int size = FileSize(resName.c_str());
	return size;
}