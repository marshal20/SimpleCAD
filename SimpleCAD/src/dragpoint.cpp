#include "dragpoint.h"
#include "inputmgr.h"

void UpdateDragPoint(DragPoint* point, Uint8 button)
{
	float x = InputMgr::GetMouseX();
	float y = InputMgr::GetMouseY();

	point->is_finished = false;

	point->x2 = x;
	point->y2 = y;

	if (InputMgr::IsBtnDown(button))
	{
		point->is_active = true;
		point->x1 = x;
		point->y1 = y;
	}
	else if (InputMgr::IsBtnUp(button))
	{
		point->is_active = false;
		point->is_finished = true;
	}
}
