#pragma once
#include "..\IRenderer.h"
#include <GL/glew.h>
#include "..\..\AlphaStd.h"
#include "Shader/MeshShaderProgram.h"
#include "Shader/Text2DShaderProgram.h"
#include "Shader/SkyShaderProgram.h"
#include "Shader/BasicShaderProgram.h"
#include "Buffers/VertexBuffer.h"
typedef shared_ptr<Scene> StrongScenePtr;
class Text_GL;
//========================================================================
class Renderer_GL : public IRenderer
{
public:
	Renderer_GL();
	~Renderer_GL();
	bool VInit(TiXmlElement* pElement) override;
	void VRender(StrongScenePtr scene) override;
	void VOnRestore() override;
	void VSetBackGroundColour(vec4& backGroundColour) override;
	void VDepthBuffer(DepthBufferCommand depthMode) override;
	void VDrawLine(vec3& from, vec3& to, vec4& colour) override;
	void VDrawPoint(vec3& point, float size, vec4& colour) override;
	void VDrawText(string text) override;
	//Shaders
	MeshShaderProgram* GetMeshShaderProgram() { return m_meshShaderProgram.get(); }
	Text2DShaderProgram* GetText2DShaderProgram() { return m_text2DShaderProgram.get(); }
	SkyShaderProgram* GetSkyShaderProgram() { return m_skyShaderProgram.get(); }
	BasicShaderProgram* GetBasicShaderProgram() { return m_basicShaderProgram.get(); }
private:
	
private:
	vec4 m_backGroundColour;
	//Shader Programs
	shared_ptr<MeshShaderProgram> m_meshShaderProgram;
	shared_ptr<Text2DShaderProgram> m_text2DShaderProgram;
	shared_ptr<SkyShaderProgram> m_skyShaderProgram;
	shared_ptr<BasicShaderProgram> m_basicShaderProgram;
	//Vertex buffer for drawing lines
	VertexBuffer m_vertexBuffer;
	Text_GL* m_debugText;
};
//========================================================================
