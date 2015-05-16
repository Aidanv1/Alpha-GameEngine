#ifndef TEXT2DSHADERPROGRAM_H
#define TEXT2DSHADERPROGRAM_H
#include "..\..\..\AlphaStd.h"
#include <GL/glew.h>
#include "BaseShaderProgram.h"
//========================================================================
class Text2DShaderProgram : public BaseShaderProgram
{
public:
	Text2DShaderProgram();
	~Text2DShaderProgram();
	virtual bool VInit(const char* vertexShaderSourceCode, const char* fragmentShaderSourceCode) override;
	void SetUniforms(float size, GLuint textureid);
	//accessor methods for attribute locations
	GLuint GetPositionID() const { return m_positionID; }
	GLuint GetTextureID() const { return m_texCoordID; }
private:
	GLuint m_positionID;
	GLuint m_texCoordID;
	GLuint m_sizeID;
	GLuint m_texID;
};
//========================================================================
#endif