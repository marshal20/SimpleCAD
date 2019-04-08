#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <gl/GL.h>
#include "renderer.h"

void render()
{
	std::queue<RenderCmd> queue;
	// Change color
	queue.push({ RenderCmd::Color, 0.0f });
	queue.push({ RenderCmd::Coord, 1.0f });
	queue.push({ RenderCmd::Coord, 1.0f });
	queue.push({ RenderCmd::Coord, 1.0f });
	// Draw line
	queue.push({ RenderCmd::Line, 0.0f });
	queue.push({ RenderCmd::Coord, -0.5f });
	queue.push({ RenderCmd::Coord, 0.5f });
	queue.push({ RenderCmd::Coord, 0.5f });
	queue.push({ RenderCmd::Coord, -0.5f });
	// Draw point
	queue.push({ RenderCmd::Point, 0.0f });
	queue.push({ RenderCmd::Coord, 0.5f });
	queue.push({ RenderCmd::Coord, 0.5f });

	render_queue(queue);
}

int main(int argc, char* argv[]) 
{
	SDL_Window* window;
	int width = 640; int height = 400;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) 
	{
		std::cout << "Unable to initialise SDL\n" << SDL_GetError();
		return -1;
	}

	window = SDL_CreateWindow( "My SDL Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	SDL_GLContext Context = SDL_GL_CreateContext(window);


	if (window == NULL) 
	{
		std::cout << "Unable to create a window\n" << SDL_GetError();
		return -1;
	}

	while (true)
	{
		SDL_Event Event;
		while (SDL_PollEvent(&Event));

		glViewport(0, 0, width, height);
		glClearColor(0.6f, 0.0f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		render();

		SDL_GL_SwapWindow(window);
	}

	return 0;
}
