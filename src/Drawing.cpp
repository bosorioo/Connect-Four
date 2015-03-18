#include "Drawing.h"
#include <cmath>
#include "glutInclude.h"

void Draw::text(std::string text, int x, int y)
{
	glRasterPos3f(x, y, 0);

	for (unsigned i = 0; i < text.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
}

void Draw::circle2d(float x, float y, float radius, unsigned points)
{
	float angle = 0.f, delta = 2 * PI / points;
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(x, y, 0.f); glVertex3f(x + radius, y, 0.f);
	while (points-- >= 2)
	{
		angle += delta;
		glVertex3f(x + radius * std::cos(angle), y - radius * std::sin(angle), 0.f);
	}
	glVertex3f(x + radius, y, 0.f);
	glEnd();
}

void Draw::wirecircle2d(float x, float y, float radius, unsigned points)
{
	float angle = 0.f, delta = 2 * PI / (points - 1);
	glBegin(GL_LINE_STRIP);
	glVertex3f(x + radius, y, 0.f);
	while (points-- >= 2)
	{
		angle += delta;
		glVertex3f(x + radius * std::cos(angle), y - radius * std::sin(angle), 0.f);
	}
	glVertex3f(x + radius, y, 0.f);
	glEnd();
}
