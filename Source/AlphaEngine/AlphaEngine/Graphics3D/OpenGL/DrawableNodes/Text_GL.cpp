#include "Text_GL.h"
#include "../Renderer_GL.h"
// -----------------------------------------------------------------------
Text_GL::Text_GL() :
m_name("text"),
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
m_vertexBuffer(),
m_aspectR(0)
{
	m_nodeProperties.m_renderPass = RenderPass_NotRendered;
}
// -----------------------------------------------------------------------
Text_GL::~Text_GL()
{
}
// -----------------------------------------------------------------------
bool Text_GL::VInitText(string fontTextureName, int maxChar)
{
	m_textureFileName = fontTextureName;
	m_maxChar = maxChar;
	Text2DShaderProgram* pShaderProgram = dynamic_cast<Renderer_GL*>(GraphicsSystem::Get().GetRenderer())->GetText2DShaderProgram();
	if (!pShaderProgram)
	{
		return false;
	}
	m_shaderProgram = pShaderProgram;

	m_numVertices = 6 *maxChar;
	return true;
}
// -----------------------------------------------------------------------
void Text_GL::VPrintText(string text, float x, float y, float size, float aspectRatio)
{
	m_text = text;	
	m_xOffset = x - 1 + 0.01; // 0.01 padding
	m_yOffset = 1 - y - size - 0.01;
	m_size = size;
	m_aspectR = aspectRatio;
	ConstructArrays();
}
// -----------------------------------------------------------------------
void Text_GL::VCleanUpText()
{
	m_text = "";
}
// -----------------------------------------------------------------------
bool Text_GL::VInitNode()
{
	bool success = true;
	if (Load() == -1)
	{
		success = false;
	}
	if (!LoadTexture())
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
void Text_GL::VRender(Scene* pScene)
{
	if (!pScene->isAlphaPass())
	{
		pScene->AddTransparentNode(this);
		Matrix4x4 viewMat;
		pScene->GetCamera()->GetViewMatrix(viewMat);
		vec4 pos4 = viewMat*m_nodeProperties.m_toWorld*vec4(m_nodeProperties.m_toWorld.GetPosition(), 1.0f);
		m_screenZ = pos4.z;
		return;
	}
	//if the text has not been set, dont render it
	if (m_text == "")
	{
		return;
	}
	UpdateText();
	m_shaderProgram->VUseProgram();
	//set shader uniforms
	m_shaderProgram->SetUniforms(	m_size,
									m_fontTexture->VGetTextureID());
	BindData();
	glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
	VRenderChildren(pScene);
}
// -----------------------------------------------------------------------
void Text_GL::UpdateText()
{
	GLfloat* textures = &m_texCoordArray[0];
	GLfloat* vertices = &m_verticesCoordArray[0];
	m_vertexBuffer.BindSubData(0, m_numVertices * 2 *sizeof(GLfloat), vertices);
	m_vertexBuffer.BindSubData(m_numVertices * 2 * sizeof(GLfloat), m_numVertices * 2 * sizeof(GLfloat), textures);
}
// -----------------------------------------------------------------------
int Text_GL::Load()
{
	ConstructArrays();
	GLfloat* textures = &m_texCoordArray[0];
	GLfloat* vertices = &m_verticesCoordArray[0];
	int success = m_vertexBuffer.Init(m_numVertices, vertices, textures, 2, 2, m_name);
	UpdateText();
	//-----------------------------------------------
	return success;
}
// -----------------------------------------------------------------------
void Text_GL::ConstructArrays()
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
bool Text_GL::LoadTexture()
{
	if (!m_fontTexture)
	{
		Texture_GL* tex = ALPHA_NEW Texture_GL(m_textureFileName);
		m_fontTexture = StrongTexturePtr(tex);
	}
	if (m_fontTexture->VLoadResource())
	{
		int textureID = m_fontTexture->VLoadTexture();
		if (textureID != -1)
		{
			return true;
		}
	}
	return false;
}
// -----------------------------------------------------------------------
void Text_GL::BindData()
{
	int offset = 2 * m_numVertices*sizeof(GLfloat);
	m_vertexBuffer.SetVertexAttribPointer(m_shaderProgram->GetPositionID(), 2, 0, 0);
	m_vertexBuffer.SetVertexAttribPointer(m_shaderProgram->GetTextureID(), 2, 0, (const void*)(offset));
}
// -----------------------------------------------------------------------

