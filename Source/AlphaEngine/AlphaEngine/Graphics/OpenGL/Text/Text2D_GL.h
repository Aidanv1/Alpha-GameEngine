#ifndef TEXT2D_GL_H
#define TEXT2D_GL_H
#include "../../../AlphaStd.h"
#include "../../IText2D.h"
#include "../Data/Texture.h"
#include "../../SceneNode.h"
#include "../Shader/Text2DShaderProgram.h"
#include "../GLRenderer.h"
class Text2D_GL : public IText2D, public SceneNode
{
public:
	Text2D_GL();
	~Text2D_GL();
	bool VInitText2D(string fontTextureName, int maxChar = 10) override;
	void VPrintText2D(string text, float x, float y, float size, float aspectRatio = 16.0 / 9.0) override;
	void VCleanUpText2D() override;
	void VRender(Scene* pScene) override;
	
private:
	int Load();
	bool LoadTexture();
	void BindData();
	void SetVertexAttribPointer(GLuint attribID, int componentCount, int stride, const void* dataoffset);	
	void ConstructArrays();
	void UpdateText();
private:
	struct VertexBufferInfo
	{
		GLuint m_vertexBufferID;
		GLuint m_vertexArrayID;
		VertexBufferInfo() :
			m_vertexBufferID(-1),
			m_vertexArrayID(-1)
		{
		}
	};
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
	VertexBufferInfo m_vertexInfo;
};

#endif
