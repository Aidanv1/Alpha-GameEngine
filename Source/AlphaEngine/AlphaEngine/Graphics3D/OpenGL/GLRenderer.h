#ifndef GLRENDERER_H
#define GLRENDERER_H
#include "..\IRenderer.h"
#include <GL/glew.h>
#include "..\..\AlphaStd.h"
#include "Shader/MeshShaderProgram.h"
#include "Shader/Text2DShaderProgram.h"
#include "Shader/SkyShaderProgram.h"
#include "Text/Text2D_GL.h"
typedef shared_ptr<Scene> StrongScenePtr;
//========================================================================
class GLRenderer : public IRenderer
{
public:
	GLRenderer();
	~GLRenderer();
	bool VInit(TiXmlElement* pElement) override;
	void VRender(StrongScenePtr scene) override;
	void VOnRestore() override;
	void VSetBackGroundColour(vec4& backGroundColour) override;
	//for developement
	void VPrintText(string text) override;
	//Shaders
	MeshShaderProgram* GetMeshShaderProgram() { return m_meshShaderProgram.get(); }
	Text2DShaderProgram* GetText2DShaderProgram() { return m_text2DShaderProgram.get(); }
	SkyShaderProgram* GetSkyShaderProgram() { return m_skyShaderProgram.get(); }
private:
	
private:
	vec4 m_backGroundColour;
	shared_ptr<IText2D> m_text2DNode;
	//Shader Programs
	shared_ptr<MeshShaderProgram> m_meshShaderProgram;
	shared_ptr<Text2DShaderProgram> m_text2DShaderProgram;
	shared_ptr<SkyShaderProgram> m_skyShaderProgram;
};
//========================================================================
#endif