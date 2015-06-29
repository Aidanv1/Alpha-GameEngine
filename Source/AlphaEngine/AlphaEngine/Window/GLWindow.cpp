#include "GLWindow.h"
// -----------------------------------------------------------------------
GLWindow::GLWindow(const char* name, int x, int y)
{
	m_xRes = x;
	m_yRes = y;
	m_programName = name;
}
// -----------------------------------------------------------------------
GLWindow::~GLWindow()
{
	//quit
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}
// -----------------------------------------------------------------------
bool GLWindow::VInit()
{
	//Initialize SDL's Video subsystem
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK) < 0)
	{
		ALPHA_ERROR("Unable to initialize SDL");
	}

	//Request opengl 4.4 context
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

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
		ALPHA_ERROR("Failed to create gl context");
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
bool GLWindow::VUpdate(float deltaMs)
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
bool GLWindow::PollEvents(float deltaMs)
{
	//TEMP TEST CODE!!!
	SDL_SetRelativeMouseMode(SDL_TRUE);
	bool mousemoved = false;
	SDL_Event mouseevent;
	int mousescroll = 0;
	float mousex = 0, mousey = 0;
	bool eventOccured = false;
	float dx = 0, dz = 0;
	while (SDL_PollEvent(&m_event))
	{
		if (m_event.type == SDL_QUIT)
		{
			return false;
		}		
		//Test camera motion
		if (m_event.type == SDL_MOUSEMOTION)
		{
			eventOccured = true;
			mousex -= m_event.motion.yrel;
			mousey -= m_event.motion.xrel;
		}

		if (m_event.type == SDL_MOUSEWHEEL)
		{
			mousescroll = m_event.wheel.y;
			eventOccured = true;
		}

		if (m_event.type == SDL_KEYDOWN)
		{
			vec3 position;
			if (m_event.key.keysym.sym == SDLK_w)
			{	
				dz--;
				eventOccured = true;
			}
			if (m_event.key.keysym.sym == SDLK_a)
			{
				dx--;
				eventOccured = true;
			}
			if (m_event.key.keysym.sym == SDLK_s)
			{
				dz++;
				eventOccured = true;
			}
			if (m_event.key.keysym.sym == SDLK_d)
			{
				dx++;
				eventOccured = true;
			}
		}
	}

	if (eventOccured)
	{
		float lookSensitivity = 0.0004f;
		float adjust = lookSensitivity;
		LookEvent* lookEvent = ALPHA_NEW LookEvent();
		lookEvent->SetLook(mousex* adjust, mousey* adjust, dz, dx, mousescroll);
		Queue_Event(lookEvent);
	}
	
	return true;

}
// -----------------------------------------------------------------------