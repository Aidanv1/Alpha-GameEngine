#include "HeightMap.h"
// -----------------------------------------------------------------------
HeightMap::HeightMap() :
m_shaderProgram(NULL),
m_vertexBuffer(),
m_numVertices(0)
{
}
// -----------------------------------------------------------------------
HeightMap::~HeightMap()
{
}
// -----------------------------------------------------------------------
bool HeightMap::VInitNode()
{
	bool success = true;
	if (Load() == -1)
	{
		success = false;
	}
	for (auto child = m_children.begin(); child != m_children.end(); child++)
	{
		if (!(*child)->VInitNode())
		{
			success = false;
		}
	}
	return success;
}
// -----------------------------------------------------------------------
void HeightMap::VRender(Scene* pScene)
{
	m_shaderProgram->VUseProgram();
	//get view and projection matrix from main camera
	mat4 viewMat;
	mat4 projMat;
	pScene->GetCamera()->GetViewMatrix(viewMat);
	pScene->GetCamera()->GetProjectionMatrix(projMat);
	mat4 scaleMat = scale(mat4(1.0f), vec3(X_SCALE, Y_SCALE, Z_SCALE));
	m_shaderProgram->SetUniforms((m_nodeProperties.m_toWorld*scaleMat),
		viewMat,
		projMat,
		m_nodeProperties.m_rotationMatrix,
		m_nodeProperties.m_lightVector,
		m_material->Texture()->VGetTextureID());
	BindData();
	glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
	VRenderChildren(pScene);
}
// -----------------------------------------------------------------------
int HeightMap::Load()
{
	if (!VLoadResource())
	{
		return -1;
	}
	if (!m_textureResource->Buffer())
	{
		return -1;
	}
	if (!m_material->LoadTexture())
	{
		return -1;
	}
	if (m_vertexBuffer.GetID() != -1)
	{
		return m_vertexBuffer.GetID();
	}
	BmpData* pBmpData = (BmpData*)m_textureResource->Buffer();

	unsigned char* pixelData = pBmpData->pixel_data.get();
	
	int height = pBmpData->height;
	int width = pBmpData->width;
	vector<float> pixels;
	vector<float> indexData;
	vector<vec3> vertexData;
	vector<vec3> normalData;
	vector<vec2> textureData;
	for (int i = 0; i < (height * width)*4; i+=4)
	{
		pixels.push_back(pixelData[i]);
	}

	//populate vertices
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			float xPosition = (((float)col / (float)(width - 1)) - 0.5f);
			float yPosition = (float)pixels.at(((row) * (height)) + col) / (float)255;
			float zPosition = ((float)row / (float)(height - 1)) - 0.5f;
			vec3 vertex(xPosition, yPosition, zPosition);
			vertexData.push_back(vertex);
			textureData.push_back(vec2(xPosition * X_SCALE / m_tileScale, zPosition* Z_SCALE / m_tileScale));

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

			top = vec3(top.x * X_SCALE, top.y * Y_SCALE, top.z * Z_SCALE);
			left = vec3(left.x * X_SCALE, left.y * Y_SCALE, left.z * Z_SCALE);
			right = vec3(right.x * X_SCALE, right.y * Y_SCALE, right.z * Z_SCALE);
			bottom = vec3(bottom.x * X_SCALE, bottom.y * Y_SCALE, bottom.z * Z_SCALE);

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
			m_numVertices++;

			verticesCoordArray.push_back(vertexData.at(bottomLeftIndexNum).x);
			verticesCoordArray.push_back(vertexData.at(bottomLeftIndexNum).y);
			verticesCoordArray.push_back(vertexData.at(bottomLeftIndexNum).z);
			m_numVertices++;

			verticesCoordArray.push_back(vertexData.at(topRightIndexNum).x);
			verticesCoordArray.push_back(vertexData.at(topRightIndexNum).y);
			verticesCoordArray.push_back(vertexData.at(topRightIndexNum).z);
			m_numVertices++;
			

			verticesCoordArray.push_back(vertexData.at(topRightIndexNum).x);
			verticesCoordArray.push_back(vertexData.at(topRightIndexNum).y);
			verticesCoordArray.push_back(vertexData.at(topRightIndexNum).z);
			m_numVertices++;

			verticesCoordArray.push_back(vertexData.at(bottomLeftIndexNum).x);
			verticesCoordArray.push_back(vertexData.at(bottomLeftIndexNum).y);
			verticesCoordArray.push_back(vertexData.at(bottomLeftIndexNum).z);
			m_numVertices++;

			verticesCoordArray.push_back(vertexData.at(bottomRightIndexNum).x);
			verticesCoordArray.push_back(vertexData.at(bottomRightIndexNum).y);
			verticesCoordArray.push_back(vertexData.at(bottomRightIndexNum).z);
			m_numVertices++;
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




	GLfloat* normals = &normalCoordArray[0];
	GLfloat* vertices = &verticesCoordArray[0];
	GLfloat* textures = &texCoordArray[0];
	int success = m_vertexBuffer.Init(m_numVertices, vertices, normals, textures,3, 3,2, m_textureFileName);
	return success;
}
// -----------------------------------------------------------------------
bool HeightMap::VLoadResource()
{
	//if no resource currently exists
	if (!m_textureResource)
	{
		//Request load Resource
		Resource* texResource = new Resource(m_textureFileName);
		texResource->RequestLoad();
		m_textureResource = StrongBitmapPtr(texResource);

		if (!m_textureResourceManager->AddResource(m_textureResource))
		{
			return false;
		}
	}
	else if (!m_textureResource->IsLoaded())
	{
		m_textureResource->RequestLoad();
	}
	else if (!m_textureResource->Buffer())
	{
		return false;
	}
	return true;

}
// -----------------------------------------------------------------------
void HeightMap::BindData()
{
	int offset = 3 * m_numVertices*sizeof(GLfloat);
	m_vertexBuffer.SetVertexAttribPointer(m_shaderProgram->GetPositionID(), 3, 0, 0);
	m_vertexBuffer.SetVertexAttribPointer(m_shaderProgram->GetNormalID(), 3, 0, (const void*)(offset));
	m_vertexBuffer.SetVertexAttribPointer(m_shaderProgram->GetTextureID(), 2, 0, (const void*)(offset*2));
}
// -----------------------------------------------------------------------
void HeightMap::VUpdateNode(Scene* pScene, float deltaMS)
{
	m_nodeProperties.m_lightVector = normalize(vec4(0.0f, 1.0f, 1.0f, 1.0f));
	m_nodeProperties.m_rotationMatrix = rotate(mat4(1.0f), 0.0f, vec3(1.0f, 0.0f, 0.0f));
	m_nodeProperties.m_toWorld = translate(mat4(1.0f), m_positionInWorld);
	m_nodeProperties.m_toWorld = m_nodeProperties.m_toWorld * m_nodeProperties.m_rotationMatrix;
}
//========================================================================
// IActorComponent Functions
//========================================================================
ComponentType HeightMap::VGetType()
{
	return 1;
}
// -----------------------------------------------------------------------
void HeightMap::VUpdate()
{

}
// -----------------------------------------------------------------------
bool HeightMap::VInitComponent(TiXmlElement* pElement)
{
	TiXmlElement* hMapElement = pElement->FirstChildElement();
	m_material = StrongMaterialPtr(ALPHA_NEW Material());
	while (hMapElement)
	{
		string value = hMapElement->Value();
		if (value == "Properties")
		{
			m_textureFileName = hMapElement->Attribute("mapFileName");

			//create material
			string texture = hMapElement->Attribute("textureFileName");
			Texture* tex = ALPHA_NEW Texture(texture.c_str());
			m_material->SetTexture(StrongTexturePtr(tex));

			hMapElement->QueryFloatAttribute("tileScale", &m_tileScale);
		
		}
		hMapElement = hMapElement->NextSiblingElement();
	}
	//init shader program
	HeightMapShaderProgram* pShaderProgram = dynamic_cast<GLRenderer*>(GraphicsSystem::Get().GetRenderer())->GetHeightMapShaderProgram();
	if (!pShaderProgram)
	{
		return false;
	}
	m_shaderProgram = pShaderProgram;
	m_textureResourceManager = GraphicsSystem::Get().GetTextureResourceManager();
	return true;
}
// -----------------------------------------------------------------------
bool HeightMap::VPostInit()
{
	return true;
}


