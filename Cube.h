#pragma once
#include "Face.h"
#include <gl\gl.h>			
#include <gl\glu.h>	
#include <algorithm>
using namespace std;


class Cube
{
public:
	Cube(float x, float y, float z);
	float x;
	float y;
	float z;
	Face *cubeFaces[6];

	void drawCube();
	
};

