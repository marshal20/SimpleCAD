#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <gl/GL.h>
#include "renderer.h"
#include <vector>

int width = 640; int height = 400;
std::queue<RenderCmd> queue;

struct Line
{
	float x1, y1, x2, y2;
};

bool is_down = false;
Line cur_line;
std::vector<Line> line_list;

void PushLine(std::queue<RenderCmd>& queue, const Line& line)
{
	queue.push({ RenderCmd::Line, 0.0f });
	queue.push({ RenderCmd::Coord, line.x1 });
	queue.push({ RenderCmd::Coord, line.y1 });
	queue.push({ RenderCmd::Coord, line.x2 });
	queue.push({ RenderCmd::Coord, line.y2 });
}

void Render()
{
	queue.push({ RenderCmd::Color, 0.0f });
	queue.push({ RenderCmd::Coord, 1.0f });
	queue.push({ RenderCmd::Coord, 1.0f });
	queue.push({ RenderCmd::Coord, 1.0f });

	for (const Line& line : line_list)
	{
		PushLine(queue, line);
	}

	if (is_down)
	{
		PushLine(queue, cur_line);
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
	float x = 2.0f * (float)Event.button.x / width - 1.0f;
	float y = -2.0f * (float)Event.button.y / height + 1.0f;

	cur_line.x2 = x;
	cur_line.y2 = y;

	if (Event.button.button == SDL_BUTTON_LEFT)
	{
		if (Event.type == SDL_MOUSEBUTTONDOWN)
		{
			is_down = true;
			cur_line.x1 = x;
			cur_line.y1 = y;
		}
		else if (Event.type == SDL_MOUSEBUTTONUP)
		{
			is_down = false;
			line_list.push_back(cur_line);
		}
	}
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

		Render();

		SDL_GL_SwapWindow(window);
	}

	return 0;
}
