#include "window.h"

/*
 * Constructors
 */
Window::Window(int _width, int _height, bool _fullscreen){
	this->width = _width;
	this->height = _height;
	this->is_fullscreen = _fullscreen;
}

/*
 * Deconstructor
 */
Window::~Window(){
    SDL_GL_DeleteContext(this->sdl_context);
    SDL_DestroyWindow(this->sdl_window);
    SDL_Quit();
}

bool Window::init(){
	debug_file << "BEGIN:\tInit Window" << std::endl;
	if (!this->init_sdl()){
		debug_file << "Failed to initialize sdl!" << std::endl;
		return false;
	}
	if (!this->init_opengl()){
		debug_file << "Failed to initialize opengl!" << std::endl;
		return false;
	}
	debug_file << "End:\tInit Window" << std::endl;
	return true;
}

bool Window::init_sdl(){
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		debug_file << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
		success = false;
	}
	else
	{
		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		//Create window
		this->sdl_window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->width, this->height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (this->sdl_window == NULL){
			debug_file << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
			success = false;
		}
		else
		{
			//Create context
			this->sdl_context = SDL_GL_CreateContext(this->sdl_window);
			if (this->sdl_context == NULL){
				debug_file << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << std::endl;
				success = false;
			}
			else
			{
				//Use Vsync
				if (SDL_GL_SetSwapInterval(1) < 0){
					debug_file << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << std::endl;
				}
			}
		}
	}

	return success;
}

bool Window::init_opengl(){
    bool success = true;
    GLenum error = GL_NO_ERROR;

    //Settings
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(100.0, (GLfloat) this->width / (GLfloat) this->height, .0001, 1000.0);
    glMatrixMode(GL_MODELVIEW);

    //Initialize clear color
    glClearColor( 0.f, 0.f, 0.f, 0.5f );
    
    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
		debug_file << "Error initializing OpenGL! " << gluErrorString(error) << std::endl;
        success = false;
    }

    return success;
}

void Window::update(){
    SDL_GL_SwapWindow(this->sdl_window);
}

void Window::set_fullscreen(){
	if (this->is_fullscreen){
		this->is_fullscreen = false;
		SDL_SetWindowFullscreen(this->sdl_window, SDL_FALSE);
	}
	else{
		this->is_fullscreen = true;
		SDL_SetWindowFullscreen(this->sdl_window, SDL_WINDOW_FULLSCREEN);
	}
}
