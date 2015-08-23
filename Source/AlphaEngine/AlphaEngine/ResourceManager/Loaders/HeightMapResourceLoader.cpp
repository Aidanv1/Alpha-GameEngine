#include "HeightMapResourceLoader.h"
#include "IL/il.h"
#include "IL/ilu.h"
#include <IL/ilut.h>
#include "../Resources/Model.h"
// -----------------------------------------------------------------------
HeightMapResourceLoader::HeightMapResourceLoader()
{
}
// -----------------------------------------------------------------------
HeightMapResourceLoader::~HeightMapResourceLoader()
{
}
// -----------------------------------------------------------------------
bool HeightMapResourceLoader::VLoadResource(string resName, unsigned char*& pBuffer, unsigned int& size)
{
	TiXmlDocument doc;
	doc.LoadFile(resName.c_str());
	TiXmlElement* heightMapElement = doc.FirstChildElement()->FirstChildElement();

	string filename = heightMapElement->Attribute("imageName");
	//Vertex info
	float xScale = 0,
		yScale = 0,
		zScale = 0;
	float tileScale = 0;
	//Material info
	MaterialInfo matInfo;
	while (heightMapElement)
	{
		string value = heightMapElement->Value();
		if (value == "Properties")
		{
			heightMapElement->QueryFloatAttribute("xzScale", &xScale);
			zScale = xScale;
			heightMapElement->QueryFloatAttribute("yScale", &yScale);
			heightMapElement->QueryFloatAttribute("tileScale", &tileScale);
		}
		if (value == "Material")
		{	
			matInfo.m_diffuseTexture = heightMapElement->Attribute("textureFileName");
			heightMapElement->QueryFloatAttribute("shininess", &matInfo.m_shininess);
			heightMapElement->QueryFloatAttribute("specularColourR", &matInfo.m_specular.x);
			heightMapElement->QueryFloatAttribute("specularColourR", &matInfo.m_specular.y);
			heightMapElement->QueryFloatAttribute("specularColourR", &matInfo.m_specular.z);
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
	CreateFloatArray(vertBufferData, pixelData, numVertices, xScale, yScale, zScale, tileScale, height, width);
	SAFE_DELETE(pixelData);
	int totalBufferSize = numVertices*sizeof(float) * 8 + sizeof(MeshInfo) + sizeof(int);
	unsigned char* pBmpBuffer = ALPHA_NEW unsigned char[totalBufferSize];
	int* numberOfMeshes = (int*)pBmpBuffer;
	*numberOfMeshes = 1;

	MeshInfo* meshInfo = (MeshInfo*)&pBmpBuffer[offsetof(Model, m_meshArray)];
	*meshInfo = MeshInfo();
	meshInfo->m_numberOfVertices = numVertices;
	meshInfo->m_dataSize = numVertices*sizeof(float) * 8;
	memcpy(&pBmpBuffer[sizeof(MeshInfo) + sizeof(int)], &vertBufferData[0], meshInfo->m_dataSize);
	meshInfo->m_data = (PtrOffset)&pBmpBuffer[sizeof(MeshInfo) + sizeof(int)] - (PtrOffset)meshInfo;
	meshInfo->m_materialInfo = matInfo;
	pBuffer = pBmpBuffer;
	size = totalBufferSize;
	return true;
}
// -----------------------------------------------------------------------
unsigned int  HeightMapResourceLoader::VGetSize(string resName)
{
	return 0;
}
// -----------------------------------------------------------------------
float* HeightMapResourceLoader::CreateFloatArray(vector <float>& vertBufferData, unsigned char* pixelData, int& numVertices, float xScale, float yScale, float zScale, float tileScale, int height, int width)
{
	vector<float> pixels;
	vector<float> indexData;
	vector<vec3> vertexData;
	vector<vec3> normalData;
	vector<vec2> textureData;
	for (int i = 0; i < (height * width) * 4; i += 4)
	{
		pixels.push_back(pixelData[i]);
	}

	//populate vertices
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			float xPosition = (((float)col / (float)(width - 1)) - 0.5f);
			float yPosition = (float)pixels.at(((row)* (height)) + col) / (float)255;
			float zPosition = ((float)row / (float)(height - 1)) - 0.5f;
			vec3 vertex(xPosition * xScale, yPosition * yScale, zPosition * zScale);
			vertexData.push_back(vertex);
			textureData.push_back(vec2(xPosition * xScale / tileScale, zPosition* zScale / tileScale));

		}
	}

	//populate normals
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			vec3 top = vertexData.at(clamp((row - 1), 0, width - 1) * width + clamp(col, 0, height - 1));
			vec3 left = vertexData.at(clamp((row), 0, width - 1)* width + clamp(col - 1, 0, height - 1));
			vec3 right = vertexData.at(clamp((row), 0, width - 1)* width + clamp(col + 1, 0, height - 1));
			vec3 bottom = vertexData.at(clamp((row + 1), 0, width - 1)* width + clamp(col, 0, height - 1));

			top = vec3(top.x, top.y, top.z);
			left = vec3(left.x, left.y, left.z);
			right = vec3(right.x, right.y, right.z);
			bottom = vec3(bottom.x, bottom.y, bottom.z);

			vec3 rightToLeft = left - right;
			vec3  topToBottom = bottom - top;
			vec3 normal = normalize(cross(rightToLeft, topToBottom));

			normalData.push_back(normal);
		}
	}

	vector<GLfloat> texCoordArray;
	vector<GLfloat> verticesCoordArray;
	vector<GLfloat> normalCoordArray;
	//populate vertices in triangle order
	for (int row = 0; row < height - 1; row++)
	{
		for (int col = 0; col < width - 1; col++)
		{
			int topLeftIndexNum = (int)(row * width + col);
			int topRightIndexNum = (int)(row * width + col + 1);
			int bottomLeftIndexNum = (int)((row + 1) * width + col);
			int bottomRightIndexNum = (int)((row + 1) * width + col + 1);
			// Write out two triangles.
			//Vertices-------------------
			verticesCoordArray.push_back(vertexData.at(topLeftIndexNum).x);
			verticesCoordArray.push_back(vertexData.at(topLeftIndexNum).y);
			verticesCoordArray.push_back(vertexData.at(topLeftIndexNum).z);
			numVertices++;

			verticesCoordArray.push_back(vertexData.at(bottomLeftIndexNum).x);
			verticesCoordArray.push_back(vertexData.at(bottomLeftIndexNum).y);
			verticesCoordArray.push_back(vertexData.at(bottomLeftIndexNum).z);
			numVertices++;

			verticesCoordArray.push_back(vertexData.at(topRightIndexNum).x);
			verticesCoordArray.push_back(vertexData.at(topRightIndexNum).y);
			verticesCoordArray.push_back(vertexData.at(topRightIndexNum).z);
			numVertices++;


			verticesCoordArray.push_back(vertexData.at(topRightIndexNum).x);
			verticesCoordArray.push_back(vertexData.at(topRightIndexNum).y);
			verticesCoordArray.push_back(vertexData.at(topRightIndexNum).z);
			numVertices++;

			verticesCoordArray.push_back(vertexData.at(bottomLeftIndexNum).x);
			verticesCoordArray.push_back(vertexData.at(bottomLeftIndexNum).y);
			verticesCoordArray.push_back(vertexData.at(bottomLeftIndexNum).z);
			numVertices++;

			verticesCoordArray.push_back(vertexData.at(bottomRightIndexNum).x);
			verticesCoordArray.push_back(vertexData.at(bottomRightIndexNum).y);
			verticesCoordArray.push_back(vertexData.at(bottomRightIndexNum).z);
			numVertices++;
			//Normals-------------------------
			normalCoordArray.push_back(normalData.at(topLeftIndexNum).x);
			normalCoordArray.push_back(normalData.at(topLeftIndexNum).y);
			normalCoordArray.push_back(normalData.at(topLeftIndexNum).z);


			normalCoordArray.push_back(normalData.at(bottomLeftIndexNum).x);
			normalCoordArray.push_back(normalData.at(bottomLeftIndexNum).y);
			normalCoordArray.push_back(normalData.at(bottomLeftIndexNum).z);

			normalCoordArray.push_back(normalData.at(topRightIndexNum).x);
			normalCoordArray.push_back(normalData.at(topRightIndexNum).y);
			normalCoordArray.push_back(normalData.at(topRightIndexNum).z);



			normalCoordArray.push_back(normalData.at(topRightIndexNum).x);
			normalCoordArray.push_back(normalData.at(topRightIndexNum).y);
			normalCoordArray.push_back(normalData.at(topRightIndexNum).z);


			normalCoordArray.push_back(normalData.at(bottomLeftIndexNum).x);
			normalCoordArray.push_back(normalData.at(bottomLeftIndexNum).y);
			normalCoordArray.push_back(normalData.at(bottomLeftIndexNum).z);

			normalCoordArray.push_back(normalData.at(bottomRightIndexNum).x);
			normalCoordArray.push_back(normalData.at(bottomRightIndexNum).y);
			normalCoordArray.push_back(normalData.at(bottomRightIndexNum).z);
			//Textures-------------------------
			texCoordArray.push_back(textureData.at(topLeftIndexNum).x);
			texCoordArray.push_back(textureData.at(topLeftIndexNum).y);

			texCoordArray.push_back(textureData.at(bottomLeftIndexNum).x);
			texCoordArray.push_back(textureData.at(bottomLeftIndexNum).y);

			texCoordArray.push_back(textureData.at(topRightIndexNum).x);
			texCoordArray.push_back(textureData.at(topRightIndexNum).y);

			texCoordArray.push_back(textureData.at(topRightIndexNum).x);
			texCoordArray.push_back(textureData.at(topRightIndexNum).y);

			texCoordArray.push_back(textureData.at(bottomLeftIndexNum).x);
			texCoordArray.push_back(textureData.at(bottomLeftIndexNum).y);

			texCoordArray.push_back(textureData.at(bottomRightIndexNum).x);
			texCoordArray.push_back(textureData.at(bottomRightIndexNum).y);
		}
	}
	vertexData.clear();
	normalData.clear();
	textureData.clear();

	//concatinate data into contigeous block
	for (int i = 0; i < verticesCoordArray.size(); i++)
	{
		vertBufferData.push_back(verticesCoordArray.at(i));
	}
	int start = verticesCoordArray.size();
	for (int i = 0; i < normalCoordArray.size(); i++)
	{
		vertBufferData.push_back(normalCoordArray.at(i));
	}
	start = normalCoordArray.size();
	for (int i = 0; i <  texCoordArray.size(); i++)
	{
		vertBufferData.push_back(texCoordArray.at(i));
	}
	return &vertBufferData[0];
}