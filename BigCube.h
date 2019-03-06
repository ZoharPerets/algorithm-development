#pragma once
#include <gl\gl.h>			
#include <gl\glu.h>	
#include "Cube.h"

class BigCube
{
public:
	Cube *cubes[27];

	BigCube();
	void DrawBigCube();
};

