#ifndef GLWINDOW_H
#define GLWINDOW_H
#include "..\AlphaStd.h"
#include "IWindow.h"
#include <sstream>
#include <GL/glew.h>
#include "IL/il.h"
#include "IL/ilu.h"
#include "../EventManager/IEventManager.h"
#include "../EventManager/Events/Events.h"
#include "../Graphics3D/GraphicsSystem.h"
//========================================================================
class GLWindow : public IWindow
{
public:
	GLWindow(const char* name, int x, int y);
	~GLWindow();
	virtual bool VInit() override;
	virtual bool VUpdate(float deltaMs) override;
private:
	bool PollEvents(float deltaMs);
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