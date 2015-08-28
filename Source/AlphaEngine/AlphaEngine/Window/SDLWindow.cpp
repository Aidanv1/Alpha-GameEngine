#include "SDLWindow.h"
#include <GL/glew.h>
#include "IL/il.h"
#include "IL/ilu.h"
#include "../EventManager/IEventManager.h"
#include "../EventManager/Events/Events.h"
#include "../Graphics3D/GraphicsSystem.h"
// -----------------------------------------------------------------------
SDLWindow::SDLWindow(const char* name, int x, int y)
{
	m_xRes = x;
	m_yRes = y;
	m_programName = name;
}
// -----------------------------------------------------------------------
SDLWindow::~SDLWindow()
{
	//quit
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}
// -----------------------------------------------------------------------
bool SDLWindow::VInit()
{
	//Initialize SDL's Video subsystem
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK) < 0)
	{
		ALPHA_ERROR("Unable to initialize SDL");
	}

	//Request opengl 4.4 context
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	// Turn on double buffering with a 24bit Z buffer.
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	//Anti Aliasing
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);
	//Create window
	m_window = SDL_CreateWindow(m_programName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
								m_xRes, m_yRes, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	//SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (!m_window)
	{ 
		stringstream ss;
		ss << "SDL Error: " << SDL_GetError();
		ALPHA_ERROR(ss.str().c_str());
		SDL_Quit();
		return false;
	}	
	//Create our opengl context and attach it to our window 
	m_glContext = SDL_GL_CreateContext(m_window);
	if (!m_glContext)
	{
		stringstream ss;
		ss << "SDL Error: " << SDL_GetError();
		ALPHA_ERROR(ss.str().c_str());
		return false;
	}
	GLenum rev;	
	glewExperimental = GL_TRUE;
	rev = glewInit();
	ilInit();
	if (GLEW_OK != rev)
	{
		ALPHA_ERROR("Failed initialize Glew");
		exit(1);
	}
	return true;
}
// -----------------------------------------------------------------------
bool SDLWindow::VUpdate(float deltaMs)
{
	if (!PollEvents(deltaMs))
	{
		return false;
	}
		
	//refresh
	SDL_GL_SwapWindow(m_window);
	return true;
}
// -----------------------------------------------------------------------
bool SDLWindow::PollEvents(float deltaMs)
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
	int mousescroll = 0;
	float mousex = 0, mousey = 0;
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent))
	{
		if (sdlEvent.type == SDL_QUIT)
		{
			return false;
		}		
		//------------------------------------------------------
		//Motion events
		//------------------------------------------------------
		if (sdlEvent.type == SDL_MOUSEMOTION)
		{
			//eventOccured = true;
			mousex -= sdlEvent.motion.yrel;
			mousey -= sdlEvent.motion.xrel;
			DoMotionCommand(Mouse, mousex, mousey);
		}

		if (sdlEvent.type == SDL_MOUSEWHEEL)
		{
			mousescroll = sdlEvent.wheel.y;
			DoMotionCommand(MouseWheel, mousescroll, 0);
		}
		//------------------------------------------------------
		//Key down events
		//------------------------------------------------------
		if (sdlEvent.type == SDL_KEYDOWN)
		{
			switch (sdlEvent.key.keysym.sym)
			{
			case SDLK_q:
				DoKeyCommand(Key_Q);
				break;
			case SDLK_w:
				DoKeyCommand(Key_W);
				break;
			case SDLK_e:
				DoKeyCommand(Key_E);
				break;
			case SDLK_r:
				DoKeyCommand(Key_R);
				break;
			case SDLK_t:
				DoKeyCommand(Key_T);
				break;
			case SDLK_y:
				DoKeyCommand(Key_Y);
				break;
			case SDLK_u:
				DoKeyCommand(Key_U);
				break;
			case SDLK_i:
				DoKeyCommand(Key_I);
				break;
			case SDLK_o:
				DoKeyCommand(Key_O);
				break;
			case SDLK_p:
				DoKeyCommand(Key_P);
				break;
			case SDLK_a:
				DoKeyCommand(Key_A);
				break;
			case SDLK_s:
				DoKeyCommand(Key_S);
				break;
			case SDLK_d:
				DoKeyCommand(Key_D);
				break;
			case SDLK_f:
				DoKeyCommand(Key_F);
				break;
			case SDLK_g:
				DoKeyCommand(Key_G);
				break;
			case SDLK_h:
				DoKeyCommand(Key_H);
				break;
			case SDLK_j:
				DoKeyCommand(Key_J);
				break;
			case SDLK_k:
				DoKeyCommand(Key_K);
				break;
			case SDLK_l:
				DoKeyCommand(Key_L);
				break;
			case SDLK_z:
				DoKeyCommand(Key_Z);
				break;
			case SDLK_x:
				DoKeyCommand(Key_X);
				break;
			case SDLK_c:
				DoKeyCommand(Key_C);
				break;
			case SDLK_v:
				DoKeyCommand(Key_V);
				break;
			case SDLK_b:
				DoKeyCommand(Key_B);
				break;
			case SDLK_n:
				DoKeyCommand(Key_N);
				break;
			case SDLK_m:
				DoKeyCommand(Key_M);
				break;
			}
		}		
	}
	return true;
}
// -----------------------------------------------------------------------
void SDLWindow::VSetKeyInputCommand(KeyName key, KeyCommand command, bool onPress)
{
	Key keyInput = key;
//	keyInput.m_name = key;
//	keyInput.m_onPress = onPress;
	m_keyCommandMap[keyInput] = command;
}
// -----------------------------------------------------------------------
void SDLWindow::VSetMotionInputCommand(MotionType motionType, MotionCommand command)
{
	m_motionCommandMap[motionType] = command;
}
// -----------------------------------------------------------------------
void SDLWindow::DoKeyCommand(Key key)
{
	//if a command is bound to the key then execute it
	auto findIt = m_keyCommandMap.find(key);
	if (findIt != m_keyCommandMap.end())
	{
		findIt->second();
	}
}
// -----------------------------------------------------------------------
void SDLWindow::DoMotionCommand(MotionType type, int x, int y)
{
	//if a command is bound to the key then execute it
	auto findIt = m_motionCommandMap.find(type);
	if (findIt != m_motionCommandMap.end())
	{
		findIt->second(MotionEvent(x,y));
	}
}
// -----------------------------------------------------------------------