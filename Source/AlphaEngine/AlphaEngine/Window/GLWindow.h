#pragma once
#include "IWindow.h"
//========================================================================
//forward declarations
struct SDL_Window;
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
	int			m_xRes;
	int			m_yRes;
	SDL_Window* m_window;
	void*		m_glContext;
	const char* m_programName;

};
//========================================================================
