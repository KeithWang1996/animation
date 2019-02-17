#include "Triangle.h"


Triangle::Triangle(int na, int nb, int nc)
{
	a = na;
	b = nb;
	c = nc;
}

int Triangle::GetA() {
	return a;
}

int Triangle::GetB() {
	return b;
}

int Triangle::GetC() {
	return c;
}

Triangle::~Triangle()
{
}
