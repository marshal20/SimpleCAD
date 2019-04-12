#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <gl/GL.h>
#include "renderer.h"
#include "inputmgr.h"
#include "dragpoint.h"
#include <vector>

int width = 640; int height = 400;
std::queue<RenderCmd> queue;

struct Point
{
	float x, y;
};

struct Line
{
	float x1, y1, x2, y2;
};

DragPoint left_drag_point;
std::vector<Point> new_polygon;
std::vector<Line> line_list;

Line DragPointToLine(const DragPoint& point)
{
	return { point.x1, point.y1, point.x2, point.y2 };
}

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
	/*
	UpdateDragPoint(&left_drag_point, SDL_BUTTON_LEFT);

	if (left_drag_point.is_active)
	{
		PushLine(queue, DragPointToLine(left_drag_point));
	}
	else if (left_drag_point.is_finished)
	{
		line_list.push_back(DragPointToLine(left_drag_point));
	}
	*/

	if (InputMgr::IsBtnDown(SDL_BUTTON_LEFT))
	{
		new_polygon.push_back({ InputMgr::GetMouseX(), InputMgr::GetMouseY() });
	}
	else if (InputMgr::IsBtnDown(SDL_BUTTON_RIGHT))
	{
		if (new_polygon.size() > 1)
		{
			for (int i = 1; i < new_polygon.size(); i++)
			{
				line_list.push_back({ new_polygon[i-1].x, new_polygon[i-1].y, new_polygon[i].x, new_polygon[i].y });
			}
			line_list.push_back({ new_polygon.back().x, new_polygon.back().y, new_polygon[0].x, new_polygon[0].y });
		}
		new_polygon.clear();
	}
	if (new_polygon.size() > 0)
	{
		for (int i = 1; i < new_polygon.size(); i++)
		{
			PushLine(queue, { new_polygon[i - 1].x, new_polygon[i - 1].y, new_polygon[i].x, new_polygon[i].y });
		}
		PushLine(queue, { new_polygon.back().x, new_polygon.back().y, InputMgr::GetMouseX(), InputMgr::GetMouseY() });
	}

	queue.push({ RenderCmd::Color, 0.0f });
	queue.push({ RenderCmd::Coord, 1.0f });
	queue.push({ RenderCmd::Coord, 1.0f });
	queue.push({ RenderCmd::Coord, 1.0f });

	for (const Line& line : line_list)
	{
		PushLine(queue, line);
	}

	render_queue(queue);

	// Clear render queue
	while (queue.size() > 0)
	{
		queue.pop();
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
		InputMgr::Update(window);

		glViewport(0, 0, width, height);
		glClearColor(0.6f, 0.0f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Render();

		SDL_GL_SwapWindow(window);
	}

	return 0;
}
