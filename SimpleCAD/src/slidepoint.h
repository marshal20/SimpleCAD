#pragma once

class SlidePoint
{
public:
	SlidePoint();
	~SlidePoint();

	void start(float x, float y);
	void update(float x, float y);
	void end();

	bool issliding();
	float getx();
	float gety();
	float getdeltax();
	float getdeltay();

private:
	bool is_sliding = false;
	float x1 = 0.0f;
	float y1 = 0.0f;
	float x2 = 0.0f;
	float y2 = 0.0f;

};
