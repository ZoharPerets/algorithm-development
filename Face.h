#pragma once
#include <gl\gl.h>			
#include <gl\glu.h>	
#include <algorithm>
#include <iostream>
#include "Colors.h"

using namespace std;

#define DISTANCE_RADIUS 0.5

class Face
{
public:
	enum TypesInCube { Up = 0, Down=1, Left=2, Right=3, Front=4, Back=5 };
	enum TypesInFace { Corner, Center, Middle };
	TypesInCube typeC;
	TypesInFace typeF;
	COLORREF color;
private:
	Colors * objColor = new Colors();

	
	int colorFlageFront;
	int colorFlageRight;
	int colorFlageLeft;
	int colorFlageUp;
	int colorFlageDown;
	int colorFlageBack;
	COLORREF *CenterColor = new COLORREF[6]{
		RGB(0xff, 0 ,0), // red
		RGB(0, 0xff ,0), // green
		RGB(0, 0 ,0xff), // blue
		RGB(0xff, 0xff ,0xff), // white
		RGB(0x8b, 0, 0x8b), // pink
		RGB(0xff, 0xff ,0) // yellow
	};


public:

	Face(int index);
	

	void DrawFace(float x, float y, float z);


};


