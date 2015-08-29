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
	m_BindingSetHandler = StrongBindingSetHandlerPtr(ALPHA_NEW BindingSetHandler());
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

	SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
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
#include <stdio.h>
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
			auto k = sdlEvent.key.keysym.sym;
			if( k == SDLK_q )	DoKeyCommand(Key_Q);
			if( k == SDLK_w )	DoKeyCommand(Key_W);
			if( k == SDLK_e )	DoKeyCommand(Key_E);
			if( k == SDLK_r )	DoKeyCommand(Key_R);
			if( k == SDLK_t )	DoKeyCommand(Key_T);				
			if( k == SDLK_y )	DoKeyCommand(Key_Y);
			if( k == SDLK_u )   DoKeyCommand(Key_U);				
			if( k == SDLK_i )	DoKeyCommand(Key_I);				
			if( k == SDLK_o )	DoKeyCommand(Key_O);				
			if( k == SDLK_p )	DoKeyCommand(Key_P);				
			if( k == SDLK_a )	DoKeyCommand(Key_A);
			if( k == SDLK_s )	DoKeyCommand(Key_S);				
			if( k == SDLK_d )	DoKeyCommand(Key_D);				
			if( k == SDLK_f )	DoKeyCommand(Key_F);				
			if( k == SDLK_g )	DoKeyCommand(Key_G);				
			if( k == SDLK_h )	DoKeyCommand(Key_H);				
			if( k == SDLK_j )	DoKeyCommand(Key_J);				
			if( k == SDLK_k )	DoKeyCommand(Key_K);				
			if( k == SDLK_l )	DoKeyCommand(Key_L);				
			if( k == SDLK_z )	DoKeyCommand(Key_Z);				
			if( k == SDLK_x )	DoKeyCommand(Key_X);				
			if( k == SDLK_c )	DoKeyCommand(Key_C);				
			if( k == SDLK_v )	DoKeyCommand(Key_V);				
			if( k == SDLK_b )	DoKeyCommand(Key_B);				
			if( k == SDLK_n )	DoKeyCommand(Key_N);				
			if( k == SDLK_m )	DoKeyCommand(Key_M);		
		}		

		if (sdlEvent.type == SDL_KEYUP)
		{
			auto k = sdlEvent.key.keysym.sym;
			if (k == SDLK_q)	DoKeyCommand(Key_Q, false);
			if (k == SDLK_w)	DoKeyCommand(Key_W, false);
			if (k == SDLK_e)	DoKeyCommand(Key_E, false);
			if (k == SDLK_r)	DoKeyCommand(Key_R, false);
			if (k == SDLK_t)	DoKeyCommand(Key_T, false);
			if (k == SDLK_y)	DoKeyCommand(Key_Y, false);
			if (k == SDLK_u)	DoKeyCommand(Key_U, false);
			if (k == SDLK_i)	DoKeyCommand(Key_I, false);
			if (k == SDLK_o)	DoKeyCommand(Key_O, false);
			if (k == SDLK_p)	DoKeyCommand(Key_P, false);
			if (k == SDLK_a)	DoKeyCommand(Key_A, false);
			if (k == SDLK_s)	DoKeyCommand(Key_S, false);
			if (k == SDLK_d)	DoKeyCommand(Key_D, false);
			if (k == SDLK_f)	DoKeyCommand(Key_F, false);
			if (k == SDLK_g)	DoKeyCommand(Key_G, false);
			if (k == SDLK_h)	DoKeyCommand(Key_H, false);
			if (k == SDLK_j)	DoKeyCommand(Key_J, false);
			if (k == SDLK_k)	DoKeyCommand(Key_K, false);
			if (k == SDLK_l)	DoKeyCommand(Key_L, false);
			if (k == SDLK_z)	DoKeyCommand(Key_Z, false);
			if (k == SDLK_x)	DoKeyCommand(Key_X, false);
			if (k == SDLK_c)	DoKeyCommand(Key_C, false);
			if (k == SDLK_v)	DoKeyCommand(Key_V, false);
			if (k == SDLK_b)	DoKeyCommand(Key_B, false);
			if (k == SDLK_n)	DoKeyCommand(Key_N, false);
			if (k == SDLK_m)	DoKeyCommand(Key_M, false);
		}
	}
	return true;
}
// -----------------------------------------------------------------------
void SDLWindow::VSetKeyInputCommand(KeyName key, KeyCommand command, bool onPress)
{
	m_BindingSetHandler->AddBinding(key, command, onPress);
}
// -----------------------------------------------------------------------
void SDLWindow::VSetMotionInputCommand(MotionType motionType, MotionCommand command)
{
	m_BindingSetHandler->AddBinding(motionType, command);
}
// -----------------------------------------------------------------------
void SDLWindow::VSetBindingSetHandler(StrongBindingSetHandlerPtr BindingSetHandler)
{
	m_BindingSetHandler = BindingSetHandler;
}
// -----------------------------------------------------------------------
void SDLWindow::DoKeyCommand(Key key, bool press)
{
	m_BindingSetHandler->DoCommand(key, press);
}
// -----------------------------------------------------------------------
void SDLWindow::DoMotionCommand(MotionType type, int x, int y)
{
	m_BindingSetHandler->DoCommand(type, MotionEvent(x, y));
}
// -----------------------------------------------------------------------