#ifndef GLRENDERER_H
#define GLRENDERER_H
#include "..\IRenderer.h"
#include <GL/glew.h>
#include "..\..\AlphaStd.h"
#include "Shader/MeshShaderProgram.h"

typedef shared_ptr<Scene> StrongScenePtr;
//========================================================================
class GLRenderer : public IRenderer
{
public:
	GLRenderer();
	~GLRenderer();
	bool VInit(StrongScenePtr pScene, TiXmlElement* pElement) override;
	void VRender() override;
	void VOnRestore() override;
	void VSetBackGroundColour(vec4& backGroundColour) override;

	//Shaders
	MeshShaderProgram* GetMeshShaderProgram() { return m_meshShaderProgram.get(); }
private:
	
private:
	vec4 m_backGroundColour;
	StrongScenePtr m_pScene;
	shared_ptr<MeshShaderProgram> m_meshShaderProgram;
};
//========================================================================
#endif