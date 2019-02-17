#pragma once
#include "Core.h";
class Triangle
{
public:
	Triangle();
	Triangle(int na, int nb, int nc);
	int GetA();
	int GetB();
	int GetC();
	~Triangle();
private:
	int a;
	int b;
	int c;
};

