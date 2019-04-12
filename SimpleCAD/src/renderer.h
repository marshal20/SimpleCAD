#pragma once
#include <queue>

struct RenderCmd
{
	enum Type { Point = 1, Line, Color, Coord } type;
	float val;
};

void RenderQueue(std::vector<RenderCmd> queue);
