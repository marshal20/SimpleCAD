#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <gl/GL.h>
#include "renderer.h"
#include "slidepoint.h"

int width = 640; int height = 400;
SlidePoint slidig_point;
std::queue<RenderCmd> queue;

void render()
{
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
	
	// Render the sliding point
	{
		if (slidig_point.issliding())
		{
			queue.push({ RenderCmd::Color, 0.0f });
			queue.push({ RenderCmd::Coord, 0.0f });
			queue.push({ RenderCmd::Coord, 0.0f });
			queue.push({ RenderCmd::Coord, 0.0f });
		}
		else
		{
			queue.push({ RenderCmd::Color, 1.0f });
			queue.push({ RenderCmd::Coord, 1.0f });
			queue.push({ RenderCmd::Coord, 1.0f });
			queue.push({ RenderCmd::Coord, 1.0f });
		}
		queue.push({ RenderCmd::Point, 0.0f });
		queue.push({ RenderCmd::Coord, slidig_point.getx()-0.1f });
		queue.push({ RenderCmd::Coord, slidig_point.gety()+0.1f });
	}

	render_queue(queue);

	// Clear render queue
	while (queue.size() > 0)
	{
		queue.pop();
	}
}

void handle_event(SDL_Event Event)
{
	if (Event.type == SDL_MOUSEBUTTONDOWN)
	{
		if (Event.button.button == SDL_BUTTON_LEFT)
		{
			float x = 2.0f * (float)Event.button.x / width - 1.0f;
			float y = -2.0f * (float)Event.button.y / height + 1.0f;
			slidig_point.start(x, y);
		}
	}

	if (Event.type == SDL_MOUSEBUTTONUP)
	{
		if (Event.button.button == SDL_BUTTON_LEFT)
		{
			slidig_point.end();
		}
	}

	float x = 2.0f * (float)Event.button.x / width - 1.0f;
	float y = -2.0f * (float)Event.button.y / height + 1.0f;
	slidig_point.update(x, y);
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
		width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	SDL_GLContext Context = SDL_GL_CreateContext(window);


	if (window == NULL) 
	{
		std::cout << "Unable to create a window\n" << SDL_GetError();
		return -1;
	}

	bool running = true;
	while (running)
	{
		SDL_Event Event;
		while (SDL_PollEvent(&Event))
		{
			if (Event.type == SDL_QUIT)
			{
				running = false;
			}
			handle_event(Event);
		}

		glViewport(0, 0, width, height);
		glClearColor(0.6f, 0.0f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		render();

		SDL_GL_SwapWindow(window);
	}

	return 0;
}
