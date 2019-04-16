#include "system.h"
#include "inputmgr.h"
#include <math.h>
#include "math.h"
#include "math/math.h"

typedef std::vector<vec2f> Polygon;

struct Line
{
	vec2f p1, p2;
};

struct Circle
{
	vec2f center;
	float r;
};

enum MouseMode
{
	MM_Draw, MM_Move
};

enum DrawMode
{
	DM_Line = 1, DM_Polygon, DM_Circle
};

std::vector<Line> line_list;
std::vector<Polygon> polygon_list;
std::vector<Circle> circle_list;

MouseMode mouse_mode = MM_Draw;
DrawMode draw_mode = DM_Line;
bool close_polygon = false;
int circle_segments = 32;
std::vector<vec2f> temp_points; // Used as a placeholder for points while drawing.

Transform grid_to_normalised = { {0.0f, 0.0f}, 0.0f, {1.0f, 1.0f} };

vec2f GetGridCursor()
{
	vec2f mouse = InputMgr::GetMouse();
	return TransformInverse(grid_to_normalised, mouse);
}

void PushLine(std::vector<RenderCmd>& queue, const Line& line)
{
	vec2f p1 = TransformForward(grid_to_normalised, line.p1);
	vec2f p2 = TransformForward(grid_to_normalised, line.p2);
	queue.push_back({ RenderCmd::Line, 0.0f });
	queue.push_back({ RenderCmd::Coord, p1.x });
	queue.push_back({ RenderCmd::Coord, p1.y });
	queue.push_back({ RenderCmd::Coord, p2.x });
	queue.push_back({ RenderCmd::Coord, p2.y });
}

void PushCircle(std::vector<RenderCmd>& queue, const Circle& circle)
{
	float x = circle.center.x;
	float y = circle.center.y;
	float r = circle.r;

	for (int i = 0; i < circle_segments; i++)
	{
		float degree1 = float(i) / circle_segments * 360;
		float degree2 = float(i+1) / circle_segments * 360;
		float theta1 = PI * degree1 / 180;
		float theta2 = PI * degree2 / 180;
		float x1 = x + r * (float)cos(theta1);
		float y1 = y + r * (float)sin(theta1);
		float x2 = x + r * (float)cos(theta2);
		float y2 = y + r * (float)sin(theta2);

		PushLine(queue, { {x1, y1}, {x2, y2} });
	}
}

void FillQueue(std::vector<RenderCmd>& queue)
{
	if (mouse_mode == MM_Draw)
	{
		if (draw_mode == DM_Line && temp_points.size() > 0)
		{
			PushLine(queue, { temp_points.back(), GetGridCursor() });
		}

		if (draw_mode == DM_Polygon && temp_points.size() > 0)
		{
			for (unsigned int i = 1; i < temp_points.size(); i++)
			{
				PushLine(queue, { temp_points[i - 1], temp_points[i] });
			}
			PushLine(queue, { temp_points.back(), GetGridCursor() });
		}

		if (draw_mode == DM_Circle && temp_points.size() > 0)
		{
			float radius = distance<float>(GetGridCursor(), temp_points[0]);
			PushCircle(queue, { temp_points.back(), radius });
		}
	}

	for (const Line& line : line_list)
	{
		PushLine(queue, line);
	}

	for (const Polygon& polygon : polygon_list)
	{
		for (unsigned int i = 1; i < polygon.size(); i++)
		{
			PushLine(queue, { polygon[i - 1], polygon[i] });
		}
	}

	for (const Circle& polygon : circle_list)
	{
		PushCircle(queue, polygon);
	}
}

void HandleLine()
{
	if (InputMgr::IsBtnDown(SDL_BUTTON_LEFT))
	{
		if (temp_points.size() > 0)
		{
			line_list.push_back({ temp_points.back(), GetGridCursor() });
			temp_points.clear();
		}
		else
		{
			temp_points.push_back(GetGridCursor());
		}
	}
	else if (InputMgr::IsBtnDown(SDL_BUTTON_RIGHT))
	{
		temp_points.clear();
	}
}

void HandlePolygon()
{
	if (InputMgr::IsBtnDown(SDL_BUTTON_LEFT))
	{
		temp_points.push_back(GetGridCursor());
	}
	else if (InputMgr::IsBtnDown(SDL_BUTTON_RIGHT))
	{
		if (temp_points.size() > 1)
		{
			polygon_list.push_back(Polygon());
			Polygon& cur_polygon = polygon_list.back();
			for (unsigned int i = 0; i < temp_points.size(); i++)
			{
				cur_polygon.push_back(temp_points[i]);
			}
			if (close_polygon)
			{
				cur_polygon.push_back(temp_points[0]);
			}
		}
		temp_points.clear();
	}
}

void HandleCircle()
{
	if (InputMgr::IsBtnDown(SDL_BUTTON_LEFT))
	{
		if (temp_points.size() > 0)
		{
			float radius = distance(GetGridCursor(), temp_points[0]);
			circle_list.push_back({ {temp_points[0].x , temp_points[0].y }, radius });
			temp_points.clear();
		}
		else
		{
			temp_points.push_back(GetGridCursor());
		}
	}
	else if (InputMgr::IsBtnDown(SDL_BUTTON_RIGHT))
	{
		temp_points.clear();
	}
}

void UpdateDraw()
{
	if (InputMgr::IsKeyDown(SDLK_0))
	{
		temp_points.clear();
		draw_mode = DM_Line;
	}
	else if (InputMgr::IsKeyDown(SDLK_1))
	{
		temp_points.clear();
		draw_mode = DM_Polygon;
	}
	else if (InputMgr::IsKeyDown(SDLK_2))
	{
		temp_points.clear();
		draw_mode = DM_Circle;
	}

	switch (draw_mode)
	{
	case DM_Line:
		HandleLine();
		break;
	case DM_Polygon:
		HandlePolygon();
		break;
	case DM_Circle:
		HandleCircle();
		break;
	default:
		break;
	}
}

void UpdateMove()
{
	static vec2f translate_initial;
	static vec2f mouse_initial;
	static bool is_moving = false;

	vec2f delta_mouse = InputMgr::GetMouse() / grid_to_normalised.scale - mouse_initial;

	if (is_moving)
	{
		grid_to_normalised.translation = translate_initial + delta_mouse;
	}

	if (InputMgr::IsBtnDown(SDL_BUTTON_LEFT))
	{
		mouse_initial = InputMgr::GetMouse() / grid_to_normalised.scale;
		translate_initial = grid_to_normalised.translation;
		is_moving = true;
	}
	if (InputMgr::IsBtnUp(SDL_BUTTON_LEFT) && is_moving)
	{
		grid_to_normalised.translation = translate_initial + delta_mouse;
		is_moving = false;
	}
	if (InputMgr::IsBtnDown(SDL_BUTTON_RIGHT))
	{
		grid_to_normalised.translation = translate_initial;
		is_moving = false;
	}
}

void Update()
{
	float aspect = float(InputMgr::GetSizeX()) / float(InputMgr::GetSizeY());
	grid_to_normalised.scale.x = 1.0f / aspect;

	if (InputMgr::IsKeyDown(SDLK_d))
	{
		mouse_mode = MM_Draw;
	}
	else if (InputMgr::IsKeyDown(SDLK_m))
	{
		mouse_mode = MM_Move;
	}

	switch (mouse_mode)
	{
	case MM_Draw:
		UpdateDraw();
		break;
	case MM_Move:
		UpdateMove();
		break;
	default:
		break;
	}

}
