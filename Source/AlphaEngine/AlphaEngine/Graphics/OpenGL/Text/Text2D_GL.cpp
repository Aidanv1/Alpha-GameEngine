#include "Text2D_GL.h"
// -----------------------------------------------------------------------
Text2D_GL::Text2D_GL() :
m_text(""),
m_xOffset(0),
m_yOffset(0),
m_size(0),
m_numVertices(0),
m_maxChar(0),
m_shaderProgram(NULL),
m_fontTexture(NULL),
m_texCoordArray(),
m_verticesCoordArray(),
m_vertexInfo(),
m_aspectR(0)
{
}
// -----------------------------------------------------------------------
Text2D_GL::~Text2D_GL()
{
	//clear that memory
	glDeleteBuffers(1, &m_vertexInfo.m_vertexBufferID);
	glDeleteVertexArrays(1, &(m_vertexInfo.m_vertexArrayID));
}
// -----------------------------------------------------------------------
bool Text2D_GL::VInitText2D(string fontTextureName, int maxChar)
{
	m_textureFileName = fontTextureName;
	m_maxChar = maxChar;
	Text2DShaderProgram* pShaderProgram = dynamic_cast<GLRenderer*>(GraphicsSystem::Get().GetRenderer())->GetText2DShaderProgram();
	if (!pShaderProgram)
	{
		return false;
	}
	m_shaderProgram = pShaderProgram;

	m_numVertices = 6 *maxChar;
	return true;
}
// -----------------------------------------------------------------------
void Text2D_GL::VPrintText2D(string text, float x, float y, float size, float aspectRatio)
{
	m_text = text;	
	m_xOffset = x - 1 + 0.01; // 0.01 padding
	m_yOffset = 1 - y - size - 0.01;
	m_size = size;
	m_aspectR = aspectRatio;
	ConstructArrays();
}
// -----------------------------------------------------------------------
void Text2D_GL::VCleanUpText2D()
{
	m_text = "";
}
// -----------------------------------------------------------------------
void Text2D_GL::VRender(Scene* pScene)
{
	//if the text has not been set, dont render it
	if (m_text == "")
	{
		return;
	}
	bool render = true;
	if (!Load())
	{
		render = false;
	}
	if (!LoadTexture())
	{
		render = false;
	}
	if (!render)
	{
		return;
	}
	UpdateText();
	m_shaderProgram->VUseProgram();
	//set shader uniforms
	m_shaderProgram->SetUniforms(	m_xOffset, 
									m_yOffset, 
									m_size,
									m_fontTexture->VGetTextureID());
	BindData();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
	glDisable(GL_BLEND);
	VRenderChildren(pScene);
}
// -----------------------------------------------------------------------
void Text2D_GL::UpdateText()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexInfo.m_vertexBufferID);
	GLfloat* textures = &m_texCoordArray[0];
	glBufferSubData(GL_ARRAY_BUFFER, 2 * m_numVertices*sizeof(GLfloat), 2 * m_numVertices*sizeof(GLfloat), textures);	
}
// -----------------------------------------------------------------------
int Text2D_GL::Load()
{
	if (m_vertexInfo.m_vertexArrayID == -1)
	{
		glGenVertexArrays(1, &(m_vertexInfo.m_vertexArrayID));
	}
	else
	{
		return m_vertexInfo.m_vertexBufferID;
	}
	glBindVertexArray(m_vertexInfo.m_vertexArrayID);
	glGenBuffers(1, &(m_vertexInfo.m_vertexBufferID));
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexInfo.m_vertexBufferID);
	// Create the buffer, but don't load anything yet	
	glBufferData(GL_ARRAY_BUFFER, 4 * m_numVertices*sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	
	GLfloat* vertices = &m_verticesCoordArray[0];
	// Load the vertex points
	glBufferSubData(GL_ARRAY_BUFFER, 0, 2 * m_numVertices*sizeof(GLfloat), vertices);
	// Load the colors right after that
	UpdateText();
	//-----------------------------------------------
	return m_vertexInfo.m_vertexBufferID;
}
// -----------------------------------------------------------------------
void Text2D_GL::ConstructArrays()
{
	m_verticesCoordArray.clear();
	m_texCoordArray.clear();
	for (int i = 0; i < m_maxChar; i++)
	{

		//Temporary
		vec2 vertex_up_left = vec2(m_xOffset + i*m_size / m_aspectR, m_yOffset + m_size);
		vec2 vertex_up_right = vec2(m_xOffset + i*m_size / m_aspectR + m_size / m_aspectR, m_yOffset + m_size);
		vec2 vertex_down_right = vec2(m_xOffset + i*m_size / m_aspectR + m_size / m_aspectR, m_yOffset);
		vec2 vertex_down_left = vec2(m_xOffset + i*m_size / m_aspectR, m_yOffset);
		m_verticesCoordArray.push_back(vertex_down_left.x);
		m_verticesCoordArray.push_back(vertex_down_left.y);
		m_verticesCoordArray.push_back(vertex_up_right.x );
		m_verticesCoordArray.push_back(vertex_up_right.y);
		m_verticesCoordArray.push_back(vertex_up_left.x);
		m_verticesCoordArray.push_back(vertex_up_left.y);

		m_verticesCoordArray.push_back(vertex_down_left.x);
		m_verticesCoordArray.push_back(vertex_down_left.y);
		m_verticesCoordArray.push_back(vertex_down_right.x);
		m_verticesCoordArray.push_back(vertex_down_right.y);
		m_verticesCoordArray.push_back(vertex_up_right.x);
		m_verticesCoordArray.push_back(vertex_up_right.y);
		char character = m_text.c_str()[i];

		float numDiv = 16;

		float uv_x = (character % (int)numDiv) / numDiv;
		float uv_y = (character / (int)numDiv) / numDiv;
		float unit = 1.0f / numDiv;

		vec2 tex_up_left = vec2(uv_x, 1.0f - uv_y);
		vec2 tex_up_right = vec2(uv_x + unit, 1.0f - uv_y);
		vec2 tex_down_right = vec2(uv_x + unit, 1.0f - (uv_y + unit));
		vec2 tex_down_left = vec2(uv_x, 1.0f - (uv_y + unit));

		m_texCoordArray.push_back(tex_down_left.x);
		m_texCoordArray.push_back(tex_down_left.y);
		m_texCoordArray.push_back(tex_up_right.x);
		m_texCoordArray.push_back(tex_up_right.y);
		m_texCoordArray.push_back(tex_up_left.x);
		m_texCoordArray.push_back(tex_up_left.y);

		m_texCoordArray.push_back(tex_down_left.x);
		m_texCoordArray.push_back(tex_down_left.y);
		m_texCoordArray.push_back(tex_down_right.x);
		m_texCoordArray.push_back(tex_down_right.y);
		m_texCoordArray.push_back(tex_up_right.x);
		m_texCoordArray.push_back(tex_up_right.y);
	}
	

}
// -----------------------------------------------------------------------
bool Text2D_GL::LoadTexture()
{
	if (!m_fontTexture)
	{
		Texture* tex = ALPHA_NEW Texture(m_textureFileName);
		m_fontTexture = StrongTexturePtr(tex);
	}
	if (m_fontTexture->LoadResource())
	{
		int textureID = m_fontTexture->LoadTexture();
		if (textureID != -1)
		{
			return true;
		}
	}
	return false;
}
// -----------------------------------------------------------------------
void Text2D_GL::BindData()
{
	int offset = 2 * m_numVertices*sizeof(GLfloat);
	SetVertexAttribPointer(m_shaderProgram->GetPositionID(), 2, 0, 0);
	SetVertexAttribPointer(m_shaderProgram->GetTextureID(), 2, 0, (const void*)(offset));
}
// -----------------------------------------------------------------------
void Text2D_GL::SetVertexAttribPointer(GLuint attribID, int componentCount, int stride, const void* dataoffset)
{
	glBindVertexArray(m_vertexInfo.m_vertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexInfo.m_vertexBufferID);
	glVertexAttribPointer(attribID, componentCount, GL_FLOAT, GL_FALSE, stride, dataoffset);
	glEnableVertexAttribArray(attribID);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
// -----------------------------------------------------------------------

