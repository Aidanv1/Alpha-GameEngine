#pragma once
#include "../../../AlphaStd.h"
#include "../../Scene/DrawableNode.h"
#include "../Data/Texture_GL.h"
#include "../../Scene/SceneNode.h"
#include "../Shader/Text2DShaderProgram.h"
#include "../Renderer_GL.h"
#include "../Buffers/VertexBuffer.h"
//========================================================================
class Text_GL : public IText
{
public:
	Text_GL();
	~Text_GL();
	bool VInitText(string fontTextureName, int maxChar = 10) override;
	void VPrintText(string text, float x, float y, float size, float aspectRatio = 16.0 / 9.0) override;
	void VCleanUpText() override;
	void VRender(Scene* pScene) override;
	virtual bool VInitNode() override;
private:
	int Load();
	bool LoadTexture();
	void BindData();
	void ConstructArrays();
	void UpdateText();
private:
	string m_name;
	string m_textureFileName;
	string m_text;
	float m_xOffset;
	float m_yOffset;
	float m_size;
	float m_aspectR;
	int m_maxChar;
	int m_numVertices;
	vector<GLfloat> m_texCoordArray;
	vector<GLfloat> m_verticesCoordArray;
	Text2DShaderProgram* m_shaderProgram;
	StrongTexturePtr m_fontTexture;
	VertexBuffer m_vertexBuffer;
};
//========================================================================

