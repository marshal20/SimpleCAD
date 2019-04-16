#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <gl/GL.h>
#include "renderer.h"
#include "inputmgr.h"
#include "system.h"
#include <vector>

int width = 640; int height = 400;
std::vector<RenderCmd> queue;

void Render()
{
	Update();
	FillQueue(queue);

	RenderQueue(queue);
	queue.clear();
}

int main(int argc, char* argv[]) 
{
	SDL_Window* window;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) 
	{
		std::cout << "Unable to initialise SDL\n" << SDL_GetError();
		return -1;
	}

	window = SDL_CreateWindow( "My SDL Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	SDL_GLContext Context = SDL_GL_CreateContext(window);


	if (window == NULL) 
	{
		std::cout << "Unable to create a window\n" << SDL_GetError();
		return -1;
	}

	bool running = true;
	while (running)
	{
		InputMgr::Update(window);

		glViewport(0, 0, InputMgr::GetSizeX(), InputMgr::GetSizeY());
		glClearColor(0.6f, 0.0f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Render();

		SDL_GL_SwapWindow(window);
	}

	return 0;
}
