#include "HeightFieldResourceLoader.h"
#include "IL/il.h"
#include "IL/ilu.h"
#include <IL/ilut.h>
#include "../Resources/Model.h"
// -----------------------------------------------------------------------
HeightFieldResourceLoader::HeightFieldResourceLoader()
{
}
// -----------------------------------------------------------------------
HeightFieldResourceLoader::~HeightFieldResourceLoader()
{
}
// -----------------------------------------------------------------------
bool HeightFieldResourceLoader::VLoadResource(string resName, unsigned char*& pBuffer, unsigned int& size)
{
	TiXmlDocument doc;
	doc.LoadFile(resName.c_str());
	TiXmlElement* heightMapElement = doc.FirstChildElement()->FirstChildElement();
	string filename = heightMapElement->Attribute("imageName");
	//Vertex info
	float xzScale = 0,
		yScale = 0,
		zScale = 0;	
	while (heightMapElement)
	{
		string value = heightMapElement->Value();
		if (value == "Properties")
		{
			heightMapElement->QueryFloatAttribute("xzScale", &xzScale);
			heightMapElement->QueryFloatAttribute("yScale", &yScale);
		}
		heightMapElement = heightMapElement->NextSiblingElement();
	}

	int end = resName.find_last_of("/");
	string path = resName.substr(0, end + 1).append(filename.c_str());

	if (!ilLoadImage(path.c_str()))
	{
		stringstream ss;
		ss << "Error reading texture file: ";
		ALPHA_ERROR(ss.str().c_str());
		return false;
	}
	int width = ilGetInteger(IL_IMAGE_WIDTH);
	int height = ilGetInteger(IL_IMAGE_HEIGHT);
	int dataSize = (width * height * 4);



	unsigned char* pixelData = ALPHA_NEW unsigned char[dataSize];
	ILinfo ImageInfo;
	iluGetImageInfo(&ImageInfo);
	if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
	{
		iluFlipImage();
	}
	ilCopyPixels(0, 0, 0, width, height, 1, IL_BGRA, IL_UNSIGNED_BYTE, pixelData);
	int numVertices = 0;
	vector <float> vertBufferData;
	float maxHeight = 0;
	CreateFloatArray(vertBufferData, pixelData, numVertices, yScale, height, width, maxHeight);
	SAFE_DELETE(pixelData);
	int totalBufferSize = numVertices*sizeof(float) + sizeof(HeightFieldDataInfo);
	unsigned char* pBmpBuffer = ALPHA_NEW unsigned char[totalBufferSize];
	int* numberOfMeshes = (int*)pBmpBuffer;
	*numberOfMeshes = 1;

	HeightFieldDataInfo* heightInfo = (HeightFieldDataInfo*)pBmpBuffer;
	heightInfo->m_numVertices = numVertices;
	heightInfo->m_scaleXZ = xzScale;
	heightInfo->m_scaleY = yScale;
	heightInfo->m_maxHeight = maxHeight;

	memcpy(&pBmpBuffer[sizeof(HeightFieldDataInfo)], &vertBufferData[0], numVertices*sizeof(float));
	heightInfo->m_data = (float*)&pBmpBuffer[sizeof(HeightFieldDataInfo)];
	pBuffer = pBmpBuffer;
	size = totalBufferSize;
	return true;
}
// -----------------------------------------------------------------------
unsigned int  HeightFieldResourceLoader::VGetSize(string resName)
{
	return 0;
}
// -----------------------------------------------------------------------
float* HeightFieldResourceLoader::CreateFloatArray(vector <float>& vertBufferData, unsigned char* pixelData, int& numVertices, float yScale, int height, int width, float& maxHeight)
{
	vector<float> pixels;
	for (int i = 0; i < (height * width) * 4; i += 4)
	{
		pixels.push_back(pixelData[i]);
	}

	//populate vertices
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{

			float yPosition = yScale * (float)pixels.at(((row)* (height)) + col) / (float)255;

			if ( maxHeight < yPosition )
			{
				maxHeight = yPosition;
			}

			vertBufferData.push_back(yPosition);

			numVertices++;
		}
	}

	return &vertBufferData[0];
}