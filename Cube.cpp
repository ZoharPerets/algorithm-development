#include "stdafx.h"
#include "Cube.h"


Cube::Cube(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
	for (int i = 0; i < 6; i++)
	{
		this->cubeFaces[i] = new Face(i);
	}
}


void Cube::drawCube()
{
	for (int i = 0; i < 6; i++)
	{
		cubeFaces[i]->DrawFace(this->x, this->y, this->z);
	}

}
