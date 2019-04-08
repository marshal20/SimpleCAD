#pragma once
#include <queue>

struct RenderCmd
{
	enum Type { Point = 1, Line, Color, Coord } type;
	float val;
};

void render_queue(std::queue<RenderCmd> queue);
