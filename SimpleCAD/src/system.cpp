#include "system.h"
#include "inputmgr.h"
#include <math.h>
#include "math.h"

typedef std::vector<Point> Polygon;

struct Circle
{
	Point center;
	float r;
};

enum DrawMode
{
	DM_Line = 1, DM_Polygon, DM_Circle
};

std::vector<Line> line_list;
std::vector<Polygon> polygon_list;
std::vector<Circle> circle_list;

DrawMode mode = DM_Line;
bool close_polygon = false;
int circle_segments = 32;
std::vector<Point> temp_points; // Used as a placeholder for points while drawing.

void PushLine(std::vector<RenderCmd>& queue, const Line& line)
{
	queue.push_back({ RenderCmd::Line, 0.0f });
	queue.push_back({ RenderCmd::Coord, line.p1.x });
	queue.push_back({ RenderCmd::Coord, line.p1.y });
	queue.push_back({ RenderCmd::Coord, line.p2.x });
	queue.push_back({ RenderCmd::Coord, line.p2.y });
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
		float x1 = x + r * cos(theta1);
		float y1 = y + r * sin(theta1);
		float x2 = x + r * cos(theta2);
		float y2 = y + r * sin(theta2);

		PushLine(queue, { x1, y1, x2, y2 });
	}
}

void FillQueue(std::vector<RenderCmd>& queue)
{
	if (mode == DM_Line && temp_points.size() > 0)
	{
		PushLine(queue, { temp_points.back(), InputMgr::GetMouse() });
	}

	if (mode == DM_Polygon && temp_points.size() > 0)
	{
		for (int i = 1; i < temp_points.size(); i++)
		{
			PushLine(queue, { temp_points[i - 1], temp_points[i] });
		}
		PushLine(queue, { temp_points.back(), InputMgr::GetMouse() });
	}

	if (mode == DM_Circle && temp_points.size() > 0)
	{
		float radius = distance(InputMgr::GetMouse(), temp_points[0]);
		PushCircle(queue, { temp_points.back(), radius });
	}

	for (const Line& line : line_list)
	{
		PushLine(queue, line);
	}

	for (const Polygon& polygon : polygon_list)
	{
		for (int i = 1; i < polygon.size(); i++)
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
			line_list.push_back({ temp_points.back(), InputMgr::GetMouse() });
			temp_points.clear();
		}
		else
		{
			temp_points.push_back(InputMgr::GetMouse());
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
		temp_points.push_back(InputMgr::GetMouse());
	}
	else if (InputMgr::IsBtnDown(SDL_BUTTON_RIGHT))
	{
		if (temp_points.size() > 1)
		{
			polygon_list.push_back(Polygon());
			Polygon& cur_polygon = polygon_list.back();
			for (int i = 0; i < temp_points.size(); i++)
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
			float radius = distance(InputMgr::GetMouse(), temp_points[0]);
			circle_list.push_back({ {temp_points[0].x , temp_points[0].y }, radius });
			temp_points.clear();
		}
		else
		{
			temp_points.push_back(InputMgr::GetMouse());
		}
	}
	else if (InputMgr::IsBtnDown(SDL_BUTTON_RIGHT))
	{
		temp_points.clear();
	}
}

void Update()
{
	if (InputMgr::IsKeyDown(SDLK_l))
	{
		temp_points.clear();
		mode = DM_Line;
	}
	else if (InputMgr::IsKeyDown(SDLK_p))
	{
		temp_points.clear();
		mode = DM_Polygon;
	}
	else if (InputMgr::IsKeyDown(SDLK_c))
	{
		temp_points.clear();
		mode = DM_Circle;
	}

	switch (mode)
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
