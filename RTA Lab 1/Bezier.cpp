#include "Bezier.h"
#include "glut.h"

Vec3 Bezier::Position(float t)
{
	return t * ((t * (t * p[0] + (1.0f- t) * p[1])) + (1.0f - t) * (t * p[1] + (1.0f - t) * p[2])) + (1.0f - t) * (t * (t * p[1] + (1.0f - t) * p[2]) + (1.0f - t) * (t * p[2] + (1.0f - t) * p[3]));
}

Bezier::Bezier(Vec3 pp1, Vec3 pp2, Vec3 pp3, Vec3 pp4)
{
	SetPoints(pp1, pp2, pp3, pp4);
}

Bezier::Bezier()
{

}

void Bezier::SetPoints(Vec3 pp1, Vec3 pp2, Vec3 pp3, Vec3 pp4)
{
	p[3] = pp1;
	p[2] = pp2;
	p[1] = pp3;
	p[0] = pp4;
}

void Bezier::Draw()
{
	float t = 0.01f;
	glBegin(GL_LINES);
	Vec3 lastPos = Position(0.0f);
	glColor4f(1.0, 0.0, 0.0, 1.0);
	while (t < 1.0f)
	{
		glVertex3fv(lastPos.Ref());
		lastPos = Position(t);
		glVertex3fv(lastPos.Ref());		
		t += 0.01f;
	}
	glEnd();
}