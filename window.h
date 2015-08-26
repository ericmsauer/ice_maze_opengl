#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>

extern std::ofstream debug_file;

class Window
{
	public:
		Window(int _width, int _height, bool _fullscreen);
		~Window();

		bool init();
		bool init_opengl();
		bool init_sdl();
		void update();
		void quit();

		//Get Variables
		int get_window_width(){ return width; }
		int get_window_height(){ return height; }

		void set_fullscreen();
	private:
		//Window
		int width;
		int height;
		bool is_fullscreen;

		SDL_Window *sdl_window;
		SDL_GLContext sdl_context;
		SDL_DisplayMode sdl_mode;
};
#endif
