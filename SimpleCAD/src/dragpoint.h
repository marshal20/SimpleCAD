#pragma once
#include <SDL/SDL.h>

struct DragPoint
{
	float x1, y1;
	float x2, y2;
	bool is_active;
	bool is_finished;
};

void UpdateDragPoint(DragPoint* point, Uint8 button);

