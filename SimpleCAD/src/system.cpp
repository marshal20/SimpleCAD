#include "system.h"
#include "inputmgr.h"

typedef std::vector<Point> Polygon;

std::vector<Line> line_list;
std::vector<Polygon> polygon_list;

DrawMode mode = DM_Line;
bool close_polygon = false;
std::vector<Point> new_line;
std::vector<Point> new_polygon;

void PushLine(std::vector<RenderCmd>& queue, const Line& line)
{
	queue.push_back({ RenderCmd::Line, 0.0f });
	queue.push_back({ RenderCmd::Coord, line.x1 });
	queue.push_back({ RenderCmd::Coord, line.y1 });
	queue.push_back({ RenderCmd::Coord, line.x2 });
	queue.push_back({ RenderCmd::Coord, line.y2 });
}

void ChangeMode(DrawMode new_mode)
{
	if (mode != new_mode)
	{
		switch (mode)
		{
		case DM_Line:
			new_line.clear();
			break;

		case DM_Polygon:
			if (new_polygon.size() > 1)
			{
				for (int i = 1; i < new_polygon.size(); i++)
				{
					line_list.push_back({ new_polygon[i - 1].x, new_polygon[i - 1].y, new_polygon[i].x, new_polygon[i].y });
				}
				line_list.push_back({ new_polygon.back().x, new_polygon.back().y, new_polygon[0].x, new_polygon[0].y });
			}
			new_polygon.clear();
			break;

		default:
			break;
		}
	}

	mode = new_mode;
}

void FillQueue(std::vector<RenderCmd>& queue)
{
	if (mode == DM_Line && new_line.size() > 0)
	{
		PushLine(queue, { new_line.back().x, new_line.back().y, InputMgr::GetMouseX(), InputMgr::GetMouseY() });
	}

	if (mode == DM_Polygon && new_polygon.size() > 0)
	{
		for (int i = 1; i < new_polygon.size(); i++)
		{
			PushLine(queue, { new_polygon[i - 1].x, new_polygon[i - 1].y, new_polygon[i].x, new_polygon[i].y });
		}
		PushLine(queue, { new_polygon.back().x, new_polygon.back().y, InputMgr::GetMouseX(), InputMgr::GetMouseY() });
	}

	for (const Line& line : line_list)
	{
		PushLine(queue, line);
	}

	for (const Polygon& polygon : polygon_list)
	{
		for (int i = 1; i < polygon.size(); i++)
		{
			PushLine(queue, { polygon[i - 1].x, polygon[i - 1].y, polygon[i].x, polygon[i].y });
		}
	}
}

void HandleLine()
{
	if (InputMgr::IsBtnDown(SDL_BUTTON_LEFT))
	{
		if (new_line.size() > 0)
		{
			line_list.push_back({ new_line.back().x, new_line.back().y, InputMgr::GetMouseX(), InputMgr::GetMouseY() });
			new_line.clear();
		}
		else
		{
			new_line.push_back({ InputMgr::GetMouseX(), InputMgr::GetMouseY() });
		}
	}
	else if (InputMgr::IsBtnDown(SDL_BUTTON_RIGHT))
	{
		new_line.clear();
	}
}

void HandlePolygon()
{
	if (InputMgr::IsBtnDown(SDL_BUTTON_LEFT))
	{
		new_polygon.push_back({ InputMgr::GetMouseX(), InputMgr::GetMouseY() });
	}
	else if (InputMgr::IsBtnDown(SDL_BUTTON_RIGHT))
	{
		if (new_polygon.size() > 1)
		{
			polygon_list.push_back(Polygon());
			Polygon& cur_polygon = polygon_list.back();
			for (int i = 0; i < new_polygon.size(); i++)
			{
				cur_polygon.push_back({ new_polygon[i].x, new_polygon[i].y });
			}
			if (close_polygon)
			{
				cur_polygon.push_back({ new_polygon[0].x, new_polygon[0].y });
			}
		}
		new_polygon.clear();
	}
}

void Update()
{
	if (InputMgr::IsKeyDown(SDLK_l))
	{
		ChangeMode(DM_Line);
	}
	else if (InputMgr::IsKeyDown(SDLK_p))
	{
		ChangeMode(DM_Polygon);
	}

	switch (mode)
	{
	case DM_Line:
		HandleLine();
		break;
	case DM_Polygon:
		HandlePolygon();
		break;
	default:
		break;
	}

}
