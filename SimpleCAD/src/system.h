#pragma once
#include <vector>
#include "renderer.h"

struct Point
{
	float x, y;
};

struct Line
{
	float x1, y1, x2, y2;
};

enum DrawMode
{
	DM_Line, DM_Polygon
};

void FillQueue(std::vector<RenderCmd>& queue);
void Update();
