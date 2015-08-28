#pragma once
#include "IWindow.h"
#include <map>
//========================================================================
//forward declarations
struct SDL_Window;
//typedefs 
typedef std::map < Key, KeyCommand > KeyCommandMap;
typedef std::map < MotionType, MotionCommand > MotionCommandMap;
//========================================================================
class SDLWindow : public IWindow
{
public:
	SDLWindow(const char* name, int x, int y);
	~SDLWindow();
	virtual bool VInit() override;
	virtual bool VUpdate(float deltaMs) override;
	//for input bindings
	void VSetKeyInputCommand(KeyName key, KeyCommand command, bool onPress) override;
	void VSetMotionInputCommand(MotionType motionType, MotionCommand command) override;
private:
	bool PollEvents(float deltaMs);
	//for input bindings
	void DoKeyCommand(Key key);
	void DoMotionCommand(MotionType type, int x, int y);
private:
	int					m_xRes;
	int					m_yRes;
	SDL_Window*			m_window;
	void*				m_glContext;
	const char*			m_programName;
	KeyCommandMap		m_keyCommandMap;
	MotionCommandMap	m_motionCommandMap;
};
//========================================================================
