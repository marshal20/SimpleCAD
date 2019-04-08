#include "renderer.h"
#include <SDL/SDL_opengl.h>
#include <gl/GL.h>

void render_queue(std::queue<RenderCmd> queue)
{
	while (queue.size() > 0)
	{
		RenderCmd next = queue.front();
		queue.pop();
		if (next.type == RenderCmd::Coord)
			continue;

		switch (next.type)
		{
		case RenderCmd::Point:
			glBegin(GL_POINTS);
			RenderCmd x = queue.front(); queue.pop();
			RenderCmd y = queue.front(); queue.pop();
			glVertex2d(x.val, y.val);
			glEnd();
			break;

		case RenderCmd::Line:
			glBegin(GL_LINES);
			RenderCmd x1 = queue.front(); queue.pop();
			RenderCmd y1 = queue.front(); queue.pop();
			RenderCmd x2 = queue.front(); queue.pop();
			RenderCmd y2 = queue.front(); queue.pop();
			glVertex2d(x1.val, y1.val);
			glVertex2d(x2.val, y2.val);
			glEnd();
			break;

		case RenderCmd::Color:
			RenderCmd r = queue.front(); queue.pop();
			RenderCmd g = queue.front(); queue.pop();
			RenderCmd b = queue.front(); queue.pop();
			glColor3f(r.val, g.val, b.val);
			break;

		default: break;
		}
	}
}
