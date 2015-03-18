#ifndef _DRAWING_H
#define _DRAWING_H

#include <string>

#define PI 3.1415f

namespace Draw
{
	void text(std::string text, int x, int y);
	void circle2d(float x, float y, float radius, unsigned points);
	void wirecircle2d(float x, float y, float radius, unsigned points);
};

#endif