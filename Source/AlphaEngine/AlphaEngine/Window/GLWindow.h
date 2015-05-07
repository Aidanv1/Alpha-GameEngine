#ifndef GLWINDOW_H
#define GLWINDOW_H
#include "..\AlphaStd.h"
#include "IWindow.h"
#include <sstream>
#include <GL/glew.h>
#include "IL/il.h"
#include "IL/ilu.h"
#include "../Graphics/GraphicsSystem.h"
//========================================================================
class GLWindow : public IWindow
{
public:
	GLWindow(const char* name, int x, int y);
	~GLWindow();
	virtual bool VInit() override;
	virtual bool VUpdate() override;
private:
	bool PollEvents();
private:
	int m_xRes;
	int m_yRes;
	SDL_Window* m_window;
	SDL_GLContext m_glContext;
	const char* m_programName;
	SDL_Event m_event;
};
//========================================================================
#endif