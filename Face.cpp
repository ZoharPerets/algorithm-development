#include "stdafx.h"
#include "Face.h"


Face::Face(int index)
{
	switch (index)
	{
	case 0:
	{
		this->typeC = TypesInCube::Front;
		this->color = RGB(0, 1, 0);
		break;
	}
	case 1:
	{
		this->typeC = TypesInCube::Left;
		this->color = RGB(1, 0, 1);
		break;
	}
	case 2:
	{
		this->typeC = TypesInCube::Back;
		this->color = RGB(1, 1, 0);
		break;
	}
	case 3:
	{
		this->typeC = TypesInCube::Right;
		this->color = RGB(0, 0, 1);
		break;
	}
	case 4:
	{
		this->typeC = TypesInCube::Up;
		this->color = RGB(1, 0, 0);
		break;
	}
	case 5:
	{
		this->typeC = TypesInCube::Down;
		this->color = RGB(1, 1,1);
		break;
	}
	}


	colorFlageFront = 0;
	colorFlageRight = 0;
	colorFlageLeft = 0;
	colorFlageUp = 0;
	colorFlageDown = 0;
	colorFlageBack = 0;

}

void Face::DrawFace(float x, float y, float z)

{
	int rnd = rand() % 6;
	COLORREF colorFace = CenterColor[rnd];

	//glPushMatrix();
	if (x == 0 && y == 0)
	{
		if (z < 0)//��� ����� �����
		{
			this->colorFlageBack = 1;
			//this->color = objColor->GetCenterColor();
			this->typeF = TypesInFace::Center;
		}
		else {//��� ����� ����
			this->colorFlageBack = 1;
			//this->color = objColor->GetCenterColor();
		}
	}
	else if (x == 0 && z == 0)
	{
		if (y < 0)//��� ����� �����
		{
			//this->typeF = TypesInFace::Center;
			this->colorFlageBack = 1;
			//this->color = objColor->GetCenterColor();
			this->typeF = TypesInFace::Center;

		}
		else
		{
			//��� ����� �����
			this->colorFlageBack = 1;
			//this->color = objColor->GetCenterColor();
			this->typeF = TypesInFace::Center;

		}
	}
	else if (y == 0 && z == 0)
	{
		if (x < 0)//��� ����� �����
		{
			this->colorFlageBack = 1;
			//this->color = objColor->GetCenterColor();
			this->typeF = TypesInFace::Center;

		}
		else
		{
			//��� ����� ����
			this->colorFlageBack = 1;
			//this->color = objColor->GetCenterColor();
			this->typeF = TypesInFace::Center;

		}
	}
	else if (x == 0 && y < 0)
	{
		if (z < 0)
		{
			//��� ��� back  down
			this->typeF = TypesInFace::Middle;

		}
		else {
			//��� ��� front down
			this->typeF = TypesInFace::Middle;

		}
	}
	else if (x == 0 && y > 0)
	{
		if (z < 0)
		{
			//��� ��� up back
			this->typeF = TypesInFace::Middle;

		}
		else
		{
			//��� ��� up front
			this->typeF = TypesInFace::Middle;

		}
	}
	else if (y == 0 && x < 0)
	{
		if (z < 0)
		{
			//��� ��� left back
			this->typeF = TypesInFace::Middle;

		}
		else
		{
			//��� ��� left front
			this->typeF = TypesInFace::Middle;

		}
	}
	else if (y == 0 && x > 0)
	{
		if (z < 0)
		{
			//��� ��� back right
			this->typeF = TypesInFace::Middle;

		}
		else
		{
			//��� ��� front right
			this->typeF = TypesInFace::Middle;

		}
	}
	else if (z == 0 && x < 0)
	{
		if (y < 0)
		{
			//��� ��� left down
			this->typeF = TypesInFace::Middle;

		}
		else
		{
			//��� ��� left up
			this->typeF = TypesInFace::Middle;

		}
	}
	else if (z == 0 && x > 0)
	{
		if (y < 0)
		{
			//��� ��� right down
			this->typeF = TypesInFace::Middle;

		}
		else
		{
			//��� ��� right up
			this->typeF = TypesInFace::Middle;

		}
	}
	else if (x > 0 && y != 0 && z != 0)
	{
		if (y > 0)
		{
			if (z > 0)
			{
				//��� ����� front up right
				this->typeF = TypesInFace::Corner;


			}
			else
			{
				//��� ����� back up right
				this->typeF = TypesInFace::Corner;


			}
		}
		else
		{
			if (z > 0)
			{
				//��� ����� front right down
				this->typeF = TypesInFace::Corner;

			}
			else
			{
				//��� ����� back right down
				this->typeF = TypesInFace::Corner;


			}
		}
	}
	else if (x < 0 && y != 0 && z != 0)
	{
		if (y > 0)
		{
			if (z > 0)
			{
				//��� ����� front up left
				this->typeF = TypesInFace::Corner;


			}
			else
			{
				//��� ����� back up left
				this->typeF = TypesInFace::Corner;


			}
		}
		else
		{
			if (z > 0)
			{
				//��� ����� front down left
				this->typeF = TypesInFace::Corner;

			}
			else
			{
				//��� ����� back down left
				this->typeF = TypesInFace::Corner;


			}
		}
	}


	switch (this->typeC)
	{
	case TypesInCube::Front:
	{
		if (this->colorFlageFront == 1)
		{
			colorFace = this->color;
		}

		glLineWidth(2);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_POLYGON);
		glColor3f(0, 0, 0);
		//glColor3f(GetRValue(this->color), GetGValue(this->color), GetBValue(this->color));

		glVertex3f(x + DISTANCE_RADIUS, y + DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glVertex3f(x - DISTANCE_RADIUS, y + DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glVertex3f(x - DISTANCE_RADIUS, y - DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glVertex3f(x + DISTANCE_RADIUS, y - DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glEnd();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_POLYGON);
		//glColor3f(0, 1, 1);
		glColor3f(GetRValue(this->color), GetGValue(this->color), GetBValue(this->color));

		glVertex3f(x + DISTANCE_RADIUS, y + DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glVertex3f(x - DISTANCE_RADIUS, y + DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glVertex3f(x - DISTANCE_RADIUS, y - DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glVertex3f(x + DISTANCE_RADIUS, y - DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glEnd();

	}
	break;

	case TypesInCube::Left:
	{
		if (this->colorFlageLeft == 1)
		{
			colorFace = this->color;
		}

		glLineWidth(2);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_POLYGON);
		glColor3f(0, 0, 0);
		//glColor3f(GetRValue(this->color), GetGValue(this->color), GetBValue(this->color));

		glVertex3f(x + DISTANCE_RADIUS, y + DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glVertex3f(x - DISTANCE_RADIUS, y + DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glVertex3f(x - DISTANCE_RADIUS, y - DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glVertex3f(x + DISTANCE_RADIUS, y - DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glEnd();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_POLYGON);		//glColor3f(1, 0, 1);
		//glColor3f(GetRValue(colorFace), GetGValue(colorFace), GetBValue(colorFace));
		glColor3f(GetRValue(this->color), GetGValue(this->color), GetBValue(this->color));

		glVertex3f(x - DISTANCE_RADIUS, y + DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glVertex3f(x - DISTANCE_RADIUS, y + DISTANCE_RADIUS, z - DISTANCE_RADIUS);
		glVertex3f(x - DISTANCE_RADIUS, y - DISTANCE_RADIUS, z - DISTANCE_RADIUS);
		glVertex3f(x - DISTANCE_RADIUS, y - DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glEnd();
	}
	break;

	case TypesInCube::Back:
	{
		if (this->colorFlageBack == 1)
		{
			colorFace = this->color;
		}

		glLineWidth(2);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_POLYGON);
		glColor3f(0, 0, 0);
		//glColor3f(GetRValue(this->color), GetGValue(this->color), GetBValue(this->color));

		glVertex3f(x + DISTANCE_RADIUS, y + DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glVertex3f(x - DISTANCE_RADIUS, y + DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glVertex3f(x - DISTANCE_RADIUS, y - DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glVertex3f(x + DISTANCE_RADIUS, y - DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glEnd();


		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_POLYGON);
		//glColor3f(1, 1, 0);
		//glColor3f(GetRValue(colorFace), GetGValue(colorFace), GetBValue(colorFace));
		glColor3f(GetRValue(this->color), GetGValue(this->color), GetBValue(this->color));


		glVertex3f(x + DISTANCE_RADIUS, y + DISTANCE_RADIUS, z - DISTANCE_RADIUS);
		glVertex3f(x - DISTANCE_RADIUS, y + DISTANCE_RADIUS, z - DISTANCE_RADIUS);
		glVertex3f(x - DISTANCE_RADIUS, y - DISTANCE_RADIUS, z - DISTANCE_RADIUS);
		glVertex3f(x + DISTANCE_RADIUS, y - DISTANCE_RADIUS, z - DISTANCE_RADIUS);
		glEnd();
	}
	break;

	case TypesInCube::Right:
	{
		if (this->colorFlageRight == 1)
		{
			colorFace = this->color;
		}

		glLineWidth(2);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_POLYGON);
		glColor3f(0, 0, 0);
		//glColor3f(GetRValue(this->color), GetGValue(this->color), GetBValue(this->color));

		glVertex3f(x + DISTANCE_RADIUS, y + DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glVertex3f(x - DISTANCE_RADIUS, y + DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glVertex3f(x - DISTANCE_RADIUS, y - DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glVertex3f(x + DISTANCE_RADIUS, y - DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glEnd();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_POLYGON);
		//glColor3f(0, 0, 1);
		//glColor3f(GetRValue(colorFace), GetGValue(colorFace), GetBValue(colorFace));
		glColor3f(GetRValue(this->color), GetGValue(this->color), GetBValue(this->color));

		glVertex3f(x + DISTANCE_RADIUS, y + DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glVertex3f(x + DISTANCE_RADIUS, y + DISTANCE_RADIUS, z - DISTANCE_RADIUS);
		glVertex3f(x + DISTANCE_RADIUS, y - DISTANCE_RADIUS, z - DISTANCE_RADIUS);
		glVertex3f(x + DISTANCE_RADIUS, y - DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glEnd();

	}
	break;

	case TypesInCube::Up:
	{
		if (this->colorFlageUp == 1)
		{
			colorFace = this->color;
		}
		glLineWidth(2);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_POLYGON);
		glColor3f(0, 0, 0);
		//glColor3f(GetRValue(this->color), GetGValue(this->color), GetBValue(this->color));

		glVertex3f(x + DISTANCE_RADIUS, y + DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glVertex3f(x - DISTANCE_RADIUS, y + DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glVertex3f(x - DISTANCE_RADIUS, y - DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glVertex3f(x + DISTANCE_RADIUS, y - DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glEnd();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_POLYGON);
		//glColor3f(1, 0, 0);
		//glColor3f(GetRValue(colorFace), GetGValue(colorFace), GetBValue(colorFace));
		glColor3f(GetRValue(this->color), GetGValue(this->color), GetBValue(this->color));


		glVertex3f(x + DISTANCE_RADIUS, y + DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glVertex3f(x - DISTANCE_RADIUS, y + DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glVertex3f(x - DISTANCE_RADIUS, y + DISTANCE_RADIUS, z - DISTANCE_RADIUS);
		glVertex3f(x + DISTANCE_RADIUS, y + DISTANCE_RADIUS, z - DISTANCE_RADIUS);
		glEnd();
		break;
	}
	break;

	case TypesInCube::Down:
	{
		if (this->colorFlageDown == 1)
		{
			colorFace = this->color;
		}

		glLineWidth(2);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_POLYGON);
		glColor3f(0, 0, 0);
		//glColor3f(GetRValue(this->color), GetGValue(this->color), GetBValue(this->color));

		glVertex3f(x + DISTANCE_RADIUS, y + DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glVertex3f(x - DISTANCE_RADIUS, y + DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glVertex3f(x - DISTANCE_RADIUS, y - DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glVertex3f(x + DISTANCE_RADIUS, y - DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glEnd();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_POLYGON);
		//glColor3f(0, 1, 0);
		//glColor3f(GetRValue(colorFace), GetGValue(colorFace), GetBValue(colorFace));
		glColor3f(GetRValue(this->color), GetGValue(this->color), GetBValue(this->color));


		glVertex3f(x - DISTANCE_RADIUS, y - DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glVertex3f(x - DISTANCE_RADIUS, y - DISTANCE_RADIUS, z - DISTANCE_RADIUS);
		glVertex3f(x + DISTANCE_RADIUS, y - DISTANCE_RADIUS, z - DISTANCE_RADIUS);
		glVertex3f(x + DISTANCE_RADIUS, y - DISTANCE_RADIUS, z + DISTANCE_RADIUS);
		glEnd();
	}
	break;

	}

	//glPopMatrix();
}



