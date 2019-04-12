#include "renderer.h"
#include <SDL/SDL_opengl.h>
#include <gl/GL.h>

void RenderQueue(std::vector<RenderCmd> queue)
{
	for(int i = 0; i < queue.size(); i++)
	{
		RenderCmd next = queue[i];
		if (next.type == RenderCmd::Coord)
			continue;

		switch (next.type)
		{
		case RenderCmd::Point:
			glBegin(GL_POINTS);
			RenderCmd x = queue[++i];
			RenderCmd y = queue[++i];
			glVertex2d(x.val, y.val);
			glEnd();
			break;

		case RenderCmd::Line:
			glBegin(GL_LINES);
			RenderCmd x1 = queue[++i];
			RenderCmd y1 = queue[++i];
			RenderCmd x2 = queue[++i];
			RenderCmd y2 = queue[++i];
			glVertex2d(x1.val, y1.val);
			glVertex2d(x2.val, y2.val);
			glEnd();
			break;

		case RenderCmd::Color:
			RenderCmd r = queue[++i];
			RenderCmd g = queue[++i];
			RenderCmd b = queue[++i];
			glColor3f(r.val, g.val, b.val);
			break;

		default: break;
		}
	}
}
