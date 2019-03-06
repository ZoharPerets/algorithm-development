// GlView.cpp: implementation of the CGlView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GlView.h"

#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGlView::CGlView(CWnd *pclWnd)
{
	//rubikCube = new BigCube();

	m_pclWnd = pclWnd;
	m_hDC  = ::GetDC(pclWnd->m_hWnd);
	BOOL bSuccess;

	RECT rect; 
	m_pclWnd->GetClientRect(&rect);
	int iWidth = rect.right - rect.left;
	int iHeight = rect.bottom - rect.top;

	bSuccess = bSetWindowPixelFormat(m_hDC);
	ASSERT(&bSuccess);

	bSuccess = bCreateViewGLContext(m_hDC);
	ASSERT(&bSuccess);
		for (int i = 0; i < 9; i++)
		{
			cubesToSelect[i] = NULL;

		}
	rubikCube = new BigCube();
	objSolverCube= new RubikCubeAlgorithem(rubikCube);
	int j = 0;

	for (int i = 7; i < 24; i++)
	{
		randomFlagsColor[j] = i;
		j++;
	}
	vInitGl(iWidth, iHeight);
	


}

// +
// *****************************************************************************
//   Module      :  CGlView::~CGlView()
//   Access      :  Public
//   Input       :  None
//   Output      :  None
//   Return Val  :  None
//   Exceptions  :  None
//   Desc        :  This function is the one and only destructor. It provides
//									clean up and performs any needed shutdown procedures
//
//   Notes       :  None
// *****************************************************************************
// -
CGlView::~CGlView()
{
	// make the rendering context not current
	if(wglGetCurrentContext()!=NULL) 
		wglMakeCurrent(NULL, NULL);
	
	// delete the Context
  if (m_hGLContext!=NULL)
  {
		wglDeleteContext(m_hGLContext);
    m_hGLContext = NULL;
  }
}


// +
// *****************************************************************************
//   Module      :  void CGlView::DrawGLScene()	
//   Access      :  Public
//   Input       :  None
//   Output      :  None
//   Return Val  :  None
//   Exceptions  :  None
//   Desc        :  This function is called contiously to provide realtime OpenGl
//									drawing
//
//   Notes       :  Place all reatime rendering code here
// *****************************************************************************
// -
void CGlView::vDrawGLScene()									
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();								
	glTranslatef(0.0f, 0.0f, -8.0f);

	glRotatef(xAngle, 1, 0, 0);
	glRotatef(yAngle, 0, 1, 0);
	glRotatef(zAngle, 0, 0, 1);
	/*glRotatef(0, 1, 0, 0);
	glRotatef(0, 0, 1, 0);
	glRotatef(0, 0, 0, 1);*/

	//axes
	/*glBegin(GL_LINES);	
	    x  RED
	    glColor3f(1.0f,0.0f,0.0f);						
		glVertex3f( -3.0f, 0.0f, 0.0f);	
		glVertex3f( 3.0f, 0.0f, 0.0f);	
	    y  GREEN 
	    glColor3f(0.0f,1.0f,0.0f);						
		glVertex3f( 0.0f, -3.0f, 0.0f);	
		glVertex3f( 0.0f, 3.0f, 0.0f);	
	    z  BLUE
	    glColor3f(0.0f,0.0f,1.0f);						
		glVertex3f( 0.0f, 0.0f, -3.0f);	
		glVertex3f( 0.0f, 0.0f, 3.0f);	
    glEnd();*/


	//glRotatef(zAngle, 0, 0, 1);

	//glPushMatrix();


	//	draw cube
	for (int i = 0; i < 27; i++)
	{
		rubikCube->cubes[i]->drawCube();
		
	}
	glPopMatrix();


	/*glEnableClientState(GL_VERTEX_ARRAY);*/
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
	/*glVertexPointer(3, GL_FLOAT, 0, vertics);
	glDrawArrays(GL_QUADS, 0, 24);*/
 


		glDisable(GL_COLOR_MATERIAL);// enable colors on the object
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
		glDisableClientState(GL_VERTEX_ARRAY);

	// swap the buffer
	SwapBuffers(m_hDC);
	
}

// +
// *****************************************************************************
//   Module      :  BOOL CGlView::SetWindowPixelFormat(HDC hDC)
//   Access      :  Private
//   Input       :  hDC - Handle to the GL windwos device context
//   Output      :  None
//   Return Val  :  TRUE  - success
//									FALSE - failure
//   Exceptions  :  None
//   Desc        :  This function sets up the required pixelformat descriptor
//
//   Notes       :  None
// *****************************************************************************
// -
BOOL CGlView::bSetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelDesc;

    pixelDesc.nSize     = sizeof(PIXELFORMATDESCRIPTOR);
    pixelDesc.nVersion	= 1;

    pixelDesc.dwFlags   =    
	
		PFD_DRAW_TO_WINDOW |// Format Must Support Window
		PFD_SUPPORT_OPENGL |// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,	// Must Support Double Buffering
		PFD_TYPE_RGBA,		// Request An RGBA Format
		32,					// Select A 16Bit Color Depth
		0, 0, 0, 0, 0, 0,	// Color Bits Ignored (?)
		0,					// No Alpha Buffer
		0,					// Shift Bit Ignored (?)
		0,					// No Accumulation Buffer
		0, 0, 0, 0,			// Accumulation Bits Ignored (?)
		16,					// 16Bit Z-Buffer (Depth Buffer)  
		0,					// No Stencil Buffer
		0,					// No Auxiliary Buffer (?)
		PFD_MAIN_PLANE,		// Main Drawing Layer
		0,					// Reserved (?)
		0, 0, 0;			// Layer Masks Ignored (?)

    int iGLPixelIndex = ChoosePixelFormat( hDC, &pixelDesc);
		ASSERT(&iGLPixelIndex);

    if (SetPixelFormat( hDC, iGLPixelIndex, &pixelDesc))
			return false;

    return TRUE;
}

// +
// *****************************************************************************
//   Module      :  BOOL CGlView::CreateViewGLContext(HDC hDC)
//   Access      :  Private
//   Input       :  hDC - Handle to the GL windwos device context
//   Output      :  None
//   Return Val  :  TRUE  - success
//									FALSE - failure
//   Exceptions  :  None
//   Desc        :  This function sets up the required GLContext
//
//   Notes       :  None
// *****************************************************************************
// -
BOOL CGlView::bCreateViewGLContext(HDC hDC)
{
		m_hGLContext = wglCreateContext(hDC);
    if (m_hGLContext == NULL)
      return FALSE;

    if (wglMakeCurrent(hDC, m_hGLContext)==FALSE)
			return FALSE;

    return TRUE;
}

// +
// *****************************************************************************
//   Module      :  void CGlView::InitGl(int iWidth, int iHeigth)
//   Access      :  Private
//   Input       :  iWidth  - width of the window
//									iHeight - height of the window
//   Output      :  None
//   Return Val  :  None
//   Exceptions  :  None
//   Desc        :  This function initializes all OpenGL rendering specifics
//
//   Notes       :  None
// *****************************************************************************
// -
void CGlView::vInitGl(int iWidth, int iHeigth)
{
	

	glShadeModel(GL_SMOOTH);								
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);									
	glEnable(GL_DEPTH_TEST);							
	glDepthFunc(GL_LEQUAL);								
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	

	glViewport(0,0,iWidth,iHeigth);						

	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();								
	
	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)iWidth/(GLfloat)iHeigth,0.45f,100.0f);

	//rubikCube = new BigCube();

	glMatrixMode(GL_MODELVIEW);							
	glLoadIdentity();		

	
}
void CGlView::Update(int flag)
{
	switch (flag)
	{
	case ROT_Z_RIGHT:
	{
		COLORREF color1;
		COLORREF color2;

		for (int i = 0; i < 27; i++)
		{
			if (rubikCube->cubes[i]->y < 0 && rubikCube->cubes[i]->x < 0)
			{

				color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[5]->color;
				rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
				rubikCube->cubes[i]->cubeFaces[4]->color = color1;
				rubikCube->cubes[i]->cubeFaces[1]->color = color2;


			}
			else if (rubikCube->cubes[i]->y < 0 && rubikCube->cubes[i]->x > 0)
			{

				color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[5]->color;
				rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
				rubikCube->cubes[i]->cubeFaces[5]->color = color1;
				rubikCube->cubes[i]->cubeFaces[1]->color = color2;


			}
			else if (rubikCube->cubes[i]->y > 0 && rubikCube->cubes[i]->x < 0)
			{

				color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[4]->color;
				rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
				rubikCube->cubes[i]->cubeFaces[4]->color = color1;
				rubikCube->cubes[i]->cubeFaces[3]->color = color2;


			}
			else if (rubikCube->cubes[i]->y > 0 && rubikCube->cubes[i]->x > 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[3]->color;
				rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
				rubikCube->cubes[i]->cubeFaces[3]->color = color1;
				rubikCube->cubes[i]->cubeFaces[5]->color = color2;
			}
			else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->x < 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
				rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->x);
				rubikCube->cubes[i]->x = 0;
				rubikCube->cubes[i]->cubeFaces[4]->color = color1;

			}
			else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->x > 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
				rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->x);
				rubikCube->cubes[i]->x = 0;
				rubikCube->cubes[i]->cubeFaces[5]->color = color1;

			}
			else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->y < 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
				rubikCube->cubes[i]-> x= rubikCube->cubes[i]->y;
				rubikCube->cubes[i]->y = 0;
				rubikCube->cubes[i]->cubeFaces[1]->color = color1;

			}
			else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->y > 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
				rubikCube->cubes[i]->x = rubikCube->cubes[i]->y;
				rubikCube->cubes[i]->y = 0;
				rubikCube->cubes[i]->cubeFaces[3]->color = color1;

			}
		}
	}
	break;
	case ROT_Z_LEFT:
	{
		COLORREF color1;
		COLORREF color2;

		for (int i = 0; i < 27; i++)
		{
			if (rubikCube->cubes[i]->y < 0 && rubikCube->cubes[i]->x < 0)
			{

				color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[5]->color;
				rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
				rubikCube->cubes[i]->cubeFaces[5]->color = color1;
				rubikCube->cubes[i]->cubeFaces[3]->color = color2;


			}
			else if (rubikCube->cubes[i]->y < 0 && rubikCube->cubes[i]->x > 0)
			{

				color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[5]->color;
				rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
				rubikCube->cubes[i]->cubeFaces[4]->color = color1;
				rubikCube->cubes[i]->cubeFaces[3]->color = color2;


			}
			else if (rubikCube->cubes[i]->y > 0 && rubikCube->cubes[i]->x < 0)
			{

				color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[4]->color;
				rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
				rubikCube->cubes[i]->cubeFaces[5]->color = color1;
				rubikCube->cubes[i]->cubeFaces[1]->color = color2;


			}
			else if (rubikCube->cubes[i]->y > 0 && rubikCube->cubes[i]->x > 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[3]->color;
				rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
				rubikCube->cubes[i]->cubeFaces[1]->color = color1;
				rubikCube->cubes[i]->cubeFaces[4]->color = color2;
			}
			else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->x < 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
				rubikCube->cubes[i]->y = rubikCube->cubes[i]->x;
				rubikCube->cubes[i]->x = 0;
				rubikCube->cubes[i]->cubeFaces[5]->color = color1;

			}
			else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->x > 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
				rubikCube->cubes[i]->y = rubikCube->cubes[i]->x;
				rubikCube->cubes[i]->x = 0;
				rubikCube->cubes[i]->cubeFaces[4]->color = color1;

			}
			else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->y < 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
				rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->y);
				rubikCube->cubes[i]->y = 0;
				rubikCube->cubes[i]->cubeFaces[3]->color = color1;

			}
			else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->y > 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
				rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->y);
				rubikCube->cubes[i]->y = 0;
				rubikCube->cubes[i]->cubeFaces[1]->color = color1;

			}
		}
	}
	break;
	case ROT_X_DOWN:
	{
		COLORREF color1;
		COLORREF color2;

		for (int i = 0; i < 27; i++)
		{
			if (rubikCube->cubes[i]->y < 0 && rubikCube->cubes[i]->z < 0)
			{
			//	if (rubikCube->cubes[i]->x != 0)
				//{
					color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[5]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
					rubikCube->cubes[i]->cubeFaces[4]->color = color1;
					rubikCube->cubes[i]->cubeFaces[2]->color = color2;

				//}
				//else {

				//	color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
				//	rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
				//	rubikCube->cubes[i]->cubeFaces[4]->color = color1;

				//}
			}
			else if (rubikCube->cubes[i]->y < 0 && rubikCube->cubes[i]->z > 0)
			{
				//if (rubikCube->cubes[i]->x != 0)
				//{
					color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[5]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->cubeFaces[5]->color = color1;
					rubikCube->cubes[i]->cubeFaces[2]->color = color2;

				//}
				/*else {

					color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->cubeFaces[5]->color = color1;

				}*/
			}
			else if (rubikCube->cubes[i]->y > 0 && rubikCube->cubes[i]->z < 0)
			{
				//if (rubikCube->cubes[i]->x != 0)
				//{
					color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[2]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->cubeFaces[0]->color = color1;
					rubikCube->cubes[i]->cubeFaces[4]->color = color2;

			//	}
			/*	else {

					color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->cubeFaces[5]->color = color1;

				}*/
			}
			else if (rubikCube->cubes[i]->y > 0 && rubikCube->cubes[i]->z > 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[0]->color;
				rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
				rubikCube->cubes[i]->cubeFaces[0]->color = color1;
				rubikCube->cubes[i]->cubeFaces[5]->color = color2;
			}
			else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->z < 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
				rubikCube->cubes[i]->y = -rubikCube->cubes[i]->z;
				rubikCube->cubes[i]->z = 0;
				rubikCube->cubes[i]->cubeFaces[4]->color= color1;

			}
			else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->z > 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
				rubikCube->cubes[i]->y = -rubikCube->cubes[i]->z;
				rubikCube->cubes[i]->z = 0;
			    rubikCube->cubes[i]->cubeFaces[5]->color= color1;

			}
			else if (rubikCube->cubes[i]->z == 0 && rubikCube->cubes[i]->y < 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
				rubikCube->cubes[i]->z = rubikCube->cubes[i]->y;
				rubikCube->cubes[i]->y = 0;
				rubikCube->cubes[i]->cubeFaces[2]->color = color1;

			}
			else if (rubikCube->cubes[i]->z == 0 && rubikCube->cubes[i]->y > 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
				rubikCube->cubes[i]->z = rubikCube->cubes[i]->y;
				rubikCube->cubes[i]->y = 0;
				rubikCube->cubes[i]->cubeFaces[0]->color = color1;

			}
		}
		break;
	}
	case ROTATE_X_DOWN:
	{
		COLORREF color1;
		COLORREF color2;

		for (int i = 0; i < 27; i++)
		{
			if (rubikCube->cubes[i]->y < 0 && rubikCube->cubes[i]->z < 0)
			{
				//	if (rubikCube->cubes[i]->x != 0)
				//{
				color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[5]->color;
				rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
				rubikCube->cubes[i]->cubeFaces[4]->color = color1;
				rubikCube->cubes[i]->cubeFaces[2]->color = color2;

				//}
				//else {

				//	color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
				//	rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
				//	rubikCube->cubes[i]->cubeFaces[4]->color = color1;

				//}
			}
			else if (rubikCube->cubes[i]->y < 0 && rubikCube->cubes[i]->z > 0)
			{
				//if (rubikCube->cubes[i]->x != 0)
				//{
				color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[5]->color;
				rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
				rubikCube->cubes[i]->cubeFaces[5]->color = color1;
				rubikCube->cubes[i]->cubeFaces[2]->color = color2;

				//}
				/*else {

				color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
				rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
				rubikCube->cubes[i]->cubeFaces[5]->color = color1;

				}*/
			}
			else if (rubikCube->cubes[i]->y > 0 && rubikCube->cubes[i]->z < 0)
			{
				//if (rubikCube->cubes[i]->x != 0)
				//{
				color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[2]->color;
				rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
				rubikCube->cubes[i]->cubeFaces[0]->color = color1;
				rubikCube->cubes[i]->cubeFaces[4]->color = color2;

				//	}
				/*	else {

				color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
				rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
				rubikCube->cubes[i]->cubeFaces[5]->color = color1;

				}*/
			}
			else if (rubikCube->cubes[i]->y > 0 && rubikCube->cubes[i]->z > 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[0]->color;
				rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
				rubikCube->cubes[i]->cubeFaces[0]->color = color1;
				rubikCube->cubes[i]->cubeFaces[5]->color = color2;
			}
			else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->z < 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
				rubikCube->cubes[i]->y = -rubikCube->cubes[i]->z;
				rubikCube->cubes[i]->z = 0;
				rubikCube->cubes[i]->cubeFaces[4]->color = color1;

			}
			else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->z > 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
				rubikCube->cubes[i]->y = -rubikCube->cubes[i]->z;
				rubikCube->cubes[i]->z = 0;
				rubikCube->cubes[i]->cubeFaces[5]->color = color1;

			}
			else if (rubikCube->cubes[i]->z == 0 && rubikCube->cubes[i]->y < 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
				rubikCube->cubes[i]->z = rubikCube->cubes[i]->y;
				rubikCube->cubes[i]->y = 0;
				rubikCube->cubes[i]->cubeFaces[2]->color = color1;

			}
			else if (rubikCube->cubes[i]->z == 0 && rubikCube->cubes[i]->y > 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
				rubikCube->cubes[i]->z = rubikCube->cubes[i]->y;
				rubikCube->cubes[i]->y = 0;
				rubikCube->cubes[i]->cubeFaces[0]->color = color1;

			}
		}
		break;
	}
	case ROTATE_X_UP:
	{
		COLORREF color1;
		COLORREF color2;

		for (int i = 0; i < 27; i++)
		{
			if (rubikCube->cubes[i]->y < 0 && rubikCube->cubes[i]->z < 0)
			{

				color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[5]->color;
				rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
				rubikCube->cubes[i]->cubeFaces[5]->color = color1;
				rubikCube->cubes[i]->cubeFaces[0]->color = color2;


			}
			else if (rubikCube->cubes[i]->y < 0 && rubikCube->cubes[i]->z > 0)
			{

				color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[5]->color;
				rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
				rubikCube->cubes[i]->cubeFaces[4]->color = color1;
				rubikCube->cubes[i]->cubeFaces[0]->color = color2;


			}
			else if (rubikCube->cubes[i]->y > 0 && rubikCube->cubes[i]->z < 0)
			{

				color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[2]->color;
				rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
				rubikCube->cubes[i]->cubeFaces[2]->color = color1;
				rubikCube->cubes[i]->cubeFaces[5]->color = color2;


			}
			else if (rubikCube->cubes[i]->y > 0 && rubikCube->cubes[i]->z > 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[0]->color;
				rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
				rubikCube->cubes[i]->cubeFaces[2]->color = color1;
				rubikCube->cubes[i]->cubeFaces[4]->color = color2;
			}
			else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->z < 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
				rubikCube->cubes[i]->y = rubikCube->cubes[i]->z;
				rubikCube->cubes[i]->z = 0;
				rubikCube->cubes[i]->cubeFaces[5]->color = color1;

			}
			else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->z > 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
				rubikCube->cubes[i]->y = rubikCube->cubes[i]->z;
				rubikCube->cubes[i]->z = 0;
				rubikCube->cubes[i]->cubeFaces[4]->color = color1;

			}
			else if (rubikCube->cubes[i]->z == 0 && rubikCube->cubes[i]->y < 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
				rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->y);
				rubikCube->cubes[i]->y = 0;
				rubikCube->cubes[i]->cubeFaces[0]->color = color1;

			}
			else if (rubikCube->cubes[i]->z == 0 && rubikCube->cubes[i]->y > 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
				rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->y);
				rubikCube->cubes[i]->y = 0;
				rubikCube->cubes[i]->cubeFaces[2]->color = color1;

			}
		}
		break;
	}
	case ROT_X_UP:
	{
		COLORREF color1;
		COLORREF color2;

		for (int i = 0; i < 27; i++)
		{
			if (rubikCube->cubes[i]->y < 0 && rubikCube->cubes[i]->z < 0)
			{
				
				color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[5]->color;
				rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
				rubikCube->cubes[i]->cubeFaces[5]->color = color1;
				rubikCube->cubes[i]->cubeFaces[0]->color = color2;

			
			}
			else if (rubikCube->cubes[i]->y < 0 && rubikCube->cubes[i]->z > 0)
			{
				
				color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[5]->color;
				rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
				rubikCube->cubes[i]->cubeFaces[4]->color = color1;
				rubikCube->cubes[i]->cubeFaces[0]->color = color2;

			
			}
			else if (rubikCube->cubes[i]->y > 0 && rubikCube->cubes[i]->z < 0)
			{
				
				color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[2]->color;
				rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
				rubikCube->cubes[i]->cubeFaces[2]->color = color1;
				rubikCube->cubes[i]->cubeFaces[5]->color = color2;

				
			}
			else if (rubikCube->cubes[i]->y > 0 && rubikCube->cubes[i]->z > 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[0]->color;
				rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
				rubikCube->cubes[i]->cubeFaces[2]->color = color1;
				rubikCube->cubes[i]->cubeFaces[4]->color = color2;
			}
			else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->z < 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
				rubikCube->cubes[i]->y = rubikCube->cubes[i]->z;
				rubikCube->cubes[i]->z = 0;
				rubikCube->cubes[i]->cubeFaces[5]->color = color1;

			}
			else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->z > 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
				rubikCube->cubes[i]->y = rubikCube->cubes[i]->z;
				rubikCube->cubes[i]->z = 0;
				rubikCube->cubes[i]->cubeFaces[4]->color = color1;

			}
			else if (rubikCube->cubes[i]->z == 0 && rubikCube->cubes[i]->y < 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
				rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->y);
				rubikCube->cubes[i]->y = 0;
				rubikCube->cubes[i]->cubeFaces[0]->color = color1;

			}
			else if (rubikCube->cubes[i]->z == 0 && rubikCube->cubes[i]->y > 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
				rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->y);
				rubikCube->cubes[i]->y = 0;
				rubikCube->cubes[i]->cubeFaces[2]->color = color1;

			}
		}
		break;
	}
	case ROTATE_Y_RIGHT:
	{
		COLORREF color1;
		COLORREF color2;

		for (int i = 0; i < 27; i++)
		{

			if (rubikCube->cubes[i]->x < 0 && rubikCube->cubes[i]->z < 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[2]->color;
				rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
				rubikCube->cubes[i]->cubeFaces[0]->color = color1;
				rubikCube->cubes[i]->cubeFaces[1]->color = color2;


			}
			else if (rubikCube->cubes[i]->x < 0 && rubikCube->cubes[i]->z > 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[0]->color;
				rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
				rubikCube->cubes[i]->cubeFaces[0]->color = color1;
				rubikCube->cubes[i]->cubeFaces[3]->color = color2;

			}
			else if (rubikCube->cubes[i]->x > 0 && rubikCube->cubes[i]->z < 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[2]->color;
				rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
				rubikCube->cubes[i]->cubeFaces[2]->color = color1;
				rubikCube->cubes[i]->cubeFaces[1]->color = color2;
			}
			else if (rubikCube->cubes[i]->x > 0 && rubikCube->cubes[i]->z > 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[0]->color;
				rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
				rubikCube->cubes[i]->cubeFaces[2]->color = color1;
				rubikCube->cubes[i]->cubeFaces[3]->color = color2;
			}
			else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->z < 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
				rubikCube->cubes[i]->x = rubikCube->cubes[i]->z;
				rubikCube->cubes[i]->z = 0;
				rubikCube->cubes[i]->cubeFaces[1]->color = color1;

			}
			else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->z > 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
				rubikCube->cubes[i]->x = rubikCube->cubes[i]->z;
				rubikCube->cubes[i]->z = 0;
				rubikCube->cubes[i]->cubeFaces[3]->color = color1;


			}
			else if (rubikCube->cubes[i]->z == 0 && rubikCube->cubes[i]->x < 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
				rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->x);
				rubikCube->cubes[i]->x = 0;
				rubikCube->cubes[i]->cubeFaces[0]->color = color1;


			}
			else if (rubikCube->cubes[i]->z == 0 && rubikCube->cubes[i]->x > 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
				rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->x);
				rubikCube->cubes[i]->x = 0;
				rubikCube->cubes[i]->cubeFaces[2]->color = color1;
			}

		}
		break;

	}
	case ROT_Y_RIGHT:
	{
		COLORREF color1;
		COLORREF color2;

		for (int i = 0; i < 27; i++)
		{

			if (rubikCube->cubes[i]->x < 0 && rubikCube->cubes[i]->z < 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[2]->color;
				rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
				rubikCube->cubes[i]->cubeFaces[0]->color = color1;
				rubikCube->cubes[i]->cubeFaces[1]->color = color2;


			}
			else if (rubikCube->cubes[i]->x < 0 && rubikCube->cubes[i]->z > 0)
			{ 
				color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
			    color2 = rubikCube->cubes[i]->cubeFaces[0]->color;
			    rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
			    rubikCube->cubes[i]->cubeFaces[0]->color = color1;
			    rubikCube->cubes[i]->cubeFaces[3]->color = color2;
			
			}
			else if (rubikCube->cubes[i]->x > 0 && rubikCube->cubes[i]->z < 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[2]->color;
				rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
				rubikCube->cubes[i]->cubeFaces[2]->color = color1;
				rubikCube->cubes[i]->cubeFaces[1]->color = color2;
			}
			else if (rubikCube->cubes[i]->x > 0 && rubikCube->cubes[i]->z > 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[0]->color;
				rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
				rubikCube->cubes[i]->cubeFaces[2]->color = color1;
				rubikCube->cubes[i]->cubeFaces[3]->color = color2;
			}
			else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->z < 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
				rubikCube->cubes[i]->x = rubikCube->cubes[i]->z;
				rubikCube->cubes[i]->z = 0;
				rubikCube->cubes[i]->cubeFaces[1]->color = color1;

			}
			else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->z > 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
				rubikCube->cubes[i]->x = rubikCube->cubes[i]->z;
				rubikCube->cubes[i]->z = 0;
				rubikCube->cubes[i]->cubeFaces[3]->color = color1;

				
			}
			else if (rubikCube->cubes[i]->z == 0 && rubikCube->cubes[i]->x < 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
				rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->x);
				rubikCube->cubes[i]->x = 0;
				rubikCube->cubes[i]->cubeFaces[0]->color = color1;

				
			}
			else if (rubikCube->cubes[i]->z == 0 && rubikCube->cubes[i]->x > 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
				rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->x);
				rubikCube->cubes[i]->x = 0;
				rubikCube->cubes[i]->cubeFaces[2]->color = color1;
			}

		}
		break;

	}
	case ROTATE_Y_LEFT:
	{
		COLORREF color1;
		COLORREF color2;

		for (int i = 0; i < 27; i++)
		{

			if (rubikCube->cubes[i]->x < 0 && rubikCube->cubes[i]->z < 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[2]->color;
				rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
				rubikCube->cubes[i]->cubeFaces[2]->color = color1;
				rubikCube->cubes[i]->cubeFaces[3]->color = color2;


			}
			else if (rubikCube->cubes[i]->x < 0 && rubikCube->cubes[i]->z > 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[0]->color;
				rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
				rubikCube->cubes[i]->cubeFaces[2]->color = color1;
				rubikCube->cubes[i]->cubeFaces[1]->color = color2;

			}
			else if (rubikCube->cubes[i]->x > 0 && rubikCube->cubes[i]->z < 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[2]->color;
				rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
				rubikCube->cubes[i]->cubeFaces[0]->color = color1;
				rubikCube->cubes[i]->cubeFaces[3]->color = color2;
			}
			else if (rubikCube->cubes[i]->x > 0 && rubikCube->cubes[i]->z > 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[0]->color;
				rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
				rubikCube->cubes[i]->cubeFaces[0]->color = color1;
				rubikCube->cubes[i]->cubeFaces[1]->color = color2;
			}
			else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->z < 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
				rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->z);
				rubikCube->cubes[i]->z = 0;
				rubikCube->cubes[i]->cubeFaces[3]->color = color1;

			}
			else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->z > 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
				rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->z);
				rubikCube->cubes[i]->z = 0;
				rubikCube->cubes[i]->cubeFaces[1]->color = color1;


			}
			else if (rubikCube->cubes[i]->z == 0 && rubikCube->cubes[i]->x < 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
				rubikCube->cubes[i]->z = rubikCube->cubes[i]->x;
				rubikCube->cubes[i]->x = 0;
				rubikCube->cubes[i]->cubeFaces[2]->color = color1;


			}
			else if (rubikCube->cubes[i]->z == 0 && rubikCube->cubes[i]->x > 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
				rubikCube->cubes[i]->z = rubikCube->cubes[i]->x;
				rubikCube->cubes[i]->x = 0;
				rubikCube->cubes[i]->cubeFaces[0]->color = color1;
			}

		}
		break;

	}
	case ROT_Y_LEFT:
	{
		COLORREF color1;
		COLORREF color2;

		for (int i = 0; i < 27; i++)
		{

			if (rubikCube->cubes[i]->x < 0 && rubikCube->cubes[i]->z < 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[2]->color;
				rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
				rubikCube->cubes[i]->cubeFaces[2]->color = color1;
				rubikCube->cubes[i]->cubeFaces[3]->color = color2;


			}
			else if (rubikCube->cubes[i]->x < 0 && rubikCube->cubes[i]->z > 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[0]->color;
				rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
				rubikCube->cubes[i]->cubeFaces[2]->color = color1;
				rubikCube->cubes[i]->cubeFaces[1]->color = color2;

			}
			else if (rubikCube->cubes[i]->x > 0 && rubikCube->cubes[i]->z < 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[2]->color;
				rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
				rubikCube->cubes[i]->cubeFaces[0]->color = color1;
				rubikCube->cubes[i]->cubeFaces[3]->color = color2;
			}
			else if (rubikCube->cubes[i]->x > 0 && rubikCube->cubes[i]->z > 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
				color2 = rubikCube->cubes[i]->cubeFaces[0]->color;
				rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
				rubikCube->cubes[i]->cubeFaces[0]->color = color1;
				rubikCube->cubes[i]->cubeFaces[1]->color = color2;
			}
			else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->z < 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
				rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->z);
				rubikCube->cubes[i]->z = 0;
				rubikCube->cubes[i]->cubeFaces[3]->color = color1;

			}
			else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->z > 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
				rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->z);
				rubikCube->cubes[i]->z = 0;
				rubikCube->cubes[i]->cubeFaces[1]->color = color1;


			}
			else if (rubikCube->cubes[i]->z == 0 && rubikCube->cubes[i]->x < 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
				rubikCube->cubes[i]->z = rubikCube->cubes[i]->x;
				rubikCube->cubes[i]->x = 0;
				rubikCube->cubes[i]->cubeFaces[2]->color = color1;


			}
			else if (rubikCube->cubes[i]->z == 0 && rubikCube->cubes[i]->x > 0)
			{
				color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
				rubikCube->cubes[i]->z = rubikCube->cubes[i]->x;
				rubikCube->cubes[i]->x = 0;
				rubikCube->cubes[i]->cubeFaces[0]->color = color1;
			}

		}
		break;

	}
	case ROTF_L_X_UP:
	{
		COLORREF color1;
		COLORREF color2;

		for (int i = 0; i < 27; i++)
		{
			if (rubikCube->cubes[i]->x < 0)
			{
				if (rubikCube->cubes[i]->y < 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[5]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->cubeFaces[5]->color = color1;
					rubikCube->cubes[i]->cubeFaces[0]->color = color2;

				}
				else if (rubikCube->cubes[i]->y < 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[5]->color;
					rubikCube->cubes[i]->y = -rubikCube->cubes[i]->y;
					rubikCube->cubes[i]->cubeFaces[4]->color = color1;
					rubikCube->cubes[i]->cubeFaces[0]->color = color2;
				}
				else if (rubikCube->cubes[i]->y > 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[2]->color;
					rubikCube->cubes[i]->y = -rubikCube->cubes[i]->y;
					rubikCube->cubes[i]->cubeFaces[2]->color = color1;
					rubikCube->cubes[i]->cubeFaces[5]->color = color2;
				}
				else if (rubikCube->cubes[i]->y > 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[4]->color;
					rubikCube->cubes[i]->z = -rubikCube->cubes[i]->z;
					rubikCube->cubes[i]->cubeFaces[4]->color = color1;
					rubikCube->cubes[i]->cubeFaces[2]->color = color2;
				}
				else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
					rubikCube->cubes[i]->y = rubikCube->cubes[i]->z;
					rubikCube->cubes[i]->z = 0;
					rubikCube->cubes[i]->cubeFaces[5]->color = color1;
				}
				else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
					rubikCube->cubes[i]->y = rubikCube->cubes[i]->z;
					rubikCube->cubes[i]->z = 0;
					rubikCube->cubes[i]->cubeFaces[4]->color = color1;

				}
				else if (rubikCube->cubes[i]->z == 0 && rubikCube->cubes[i]->y < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
					rubikCube->cubes[i]->z = -rubikCube->cubes[i]->y;
					rubikCube->cubes[i]->y = 0;
					rubikCube->cubes[i]->cubeFaces[0]->color = color1;

				}
				else if (rubikCube->cubes[i]->z == 0 && rubikCube->cubes[i]->y > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
					rubikCube->cubes[i]->z = -rubikCube->cubes[i]->y;
					rubikCube->cubes[i]->y = 0;
					rubikCube->cubes[i]->cubeFaces[2]->color = color1;

				}
			}
		}
		break;
	}
	case ROTF_D_Y_LEFT:
	{
		COLORREF color1;
		COLORREF color2;

		for (int i = 0; i < 27; i++)
		{
			if (rubikCube->cubes[i]->y < 0)
			{
				if (rubikCube->cubes[i]->x < 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[2]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->cubeFaces[2]->color = color1;
					rubikCube->cubes[i]->cubeFaces[3]->color = color2;

				}
				else if (rubikCube->cubes[i]->x < 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[1]->color;
					rubikCube->cubes[i]->z = -rubikCube->cubes[i]->z;
					rubikCube->cubes[i]->cubeFaces[1]->color = color1;
					rubikCube->cubes[i]->cubeFaces[2]->color = color2;
				}
				else if (rubikCube->cubes[i]->x > 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[3]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->cubeFaces[3]->color = color1;
					rubikCube->cubes[i]->cubeFaces[0]->color = color2;
				}
				else if (rubikCube->cubes[i]->x > 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[0]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->cubeFaces[0]->color = color1;
					rubikCube->cubes[i]->cubeFaces[1]->color = color2;
				}
				else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->z = 0;
					rubikCube->cubes[i]->cubeFaces[3]->color = color1;
				}
				else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->z = 0;
					rubikCube->cubes[i]->cubeFaces[1]->color = color1;

				}
				else if (rubikCube->cubes[i]->z == 0 && rubikCube->cubes[i]->x < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
					rubikCube->cubes[i]->z = rubikCube->cubes[i]->x;
					rubikCube->cubes[i]->x = 0;
					rubikCube->cubes[i]->cubeFaces[2]->color = color1;

				}
				else if (rubikCube->cubes[i]->z == 0 && rubikCube->cubes[i]->x > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
					rubikCube->cubes[i]->z = rubikCube->cubes[i]->x;
					rubikCube->cubes[i]->x = 0;
					rubikCube->cubes[i]->cubeFaces[0]->color = color1;

				}

			}
		}
		break;
	}
	case ROTF_M_X_UP:
	{
		COLORREF color1;
		COLORREF color2;
		for (int i = 0; i < 27; i++)
		{
			if (rubikCube->cubes[i]->x == 0)
			{
				if (rubikCube->cubes[i]->y < 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[5]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->cubeFaces[5]->color = color1;
					rubikCube->cubes[i]->cubeFaces[0]->color = color2;

				}
				else if (rubikCube->cubes[i]->y < 0 && rubikCube->cubes[i]->z == 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->y);
					rubikCube->cubes[i]->y = 0;
					rubikCube->cubes[i]->cubeFaces[0]->color = color1;

				}
				else if (rubikCube->cubes[i]->y < 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[0]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
					rubikCube->cubes[i]->cubeFaces[0]->color = color1;
					rubikCube->cubes[i]->cubeFaces[4]->color = color2;

				}
				else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
					rubikCube->cubes[i]->y = rubikCube->cubes[i]->z;
					rubikCube->cubes[i]->z = 0;
					rubikCube->cubes[i]->cubeFaces[4]->color = color1;

				}
				else if (rubikCube->cubes[i]->y > 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[0]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->cubeFaces[2]->color = color1;
					rubikCube->cubes[i]->cubeFaces[4]->color = color2;

				}
				else if (rubikCube->cubes[i]->y > 0 && rubikCube->cubes[i]->z == 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->y);
					rubikCube->cubes[i]->y = 0;
					rubikCube->cubes[i]->cubeFaces[2]->color = color1;

				}
				else if (rubikCube->cubes[i]->y > 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[2]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
					rubikCube->cubes[i]->cubeFaces[2]->color = color1;
					rubikCube->cubes[i]->cubeFaces[5]->color = color2;

				}
				else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
					rubikCube->cubes[i]->y = rubikCube->cubes[i]->z;
					rubikCube->cubes[i]->z = 0;
					rubikCube->cubes[i]->cubeFaces[5]->color = color1;

				}
			}
		}
		break;
	}
	case ROTF_R_X_UP:
	{
		COLORREF color1;
		COLORREF color2;
		for (int i = 0; i < 27; i++)
		{
			if (rubikCube->cubes[i]->x > 0)
			{
				if (rubikCube->cubes[i]->y < 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[5]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->cubeFaces[5]->color = color1;
					rubikCube->cubes[i]->cubeFaces[0]->color = color2;

				}
				else if (rubikCube->cubes[i]->y < 0 && rubikCube->cubes[i]->z == 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->y);
					rubikCube->cubes[i]->y = 0;
					rubikCube->cubes[i]->cubeFaces[0]->color = color1;

				}
				else if (rubikCube->cubes[i]->y < 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[0]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
					rubikCube->cubes[i]->cubeFaces[0]->color = color1;
					rubikCube->cubes[i]->cubeFaces[4]->color = color2;

				}
				else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
					rubikCube->cubes[i]->y = rubikCube->cubes[i]->z;
					rubikCube->cubes[i]->z = 0;
					rubikCube->cubes[i]->cubeFaces[4]->color = color1;

				}
				else if (rubikCube->cubes[i]->y > 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[0]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->cubeFaces[2]->color = color1;
					rubikCube->cubes[i]->cubeFaces[4]->color = color2;

				}
				else if (rubikCube->cubes[i]->y > 0 && rubikCube->cubes[i]->z == 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->y);
					rubikCube->cubes[i]->y = 0;
					rubikCube->cubes[i]->cubeFaces[2]->color = color1;

				}
				else if (rubikCube->cubes[i]->y > 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[2]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
					rubikCube->cubes[i]->cubeFaces[2]->color = color1;
					rubikCube->cubes[i]->cubeFaces[5]->color = color2;

				}
				else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
					rubikCube->cubes[i]->y = rubikCube->cubes[i]->z;
					rubikCube->cubes[i]->z = 0;
					rubikCube->cubes[i]->cubeFaces[5]->color = color1;

				}
			}
		}
		break;
	}

	case ROTF_M_Y_LEFT:
	{
		COLORREF color1;
		COLORREF color2;

		for (int i = 0; i < 27; i++)
		{
			if (rubikCube->cubes[i]->y == 0)
			{
				if (rubikCube->cubes[i]->x < 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[2]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->cubeFaces[2]->color = color1;
					rubikCube->cubes[i]->cubeFaces[3]->color = color2;

				}
				else if (rubikCube->cubes[i]->x < 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[1]->color;
					rubikCube->cubes[i]->z = -rubikCube->cubes[i]->z;
					rubikCube->cubes[i]->cubeFaces[1]->color = color1;
					rubikCube->cubes[i]->cubeFaces[2]->color = color2;
				}
				else if (rubikCube->cubes[i]->x > 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[3]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->cubeFaces[3]->color = color1;
					rubikCube->cubes[i]->cubeFaces[0]->color = color2;
				}
				else if (rubikCube->cubes[i]->x > 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[0]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->cubeFaces[0]->color = color1;
					rubikCube->cubes[i]->cubeFaces[1]->color = color2;
				}
				else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->z = 0;
					rubikCube->cubes[i]->cubeFaces[3]->color = color1;
				}
				else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->z = 0;
					rubikCube->cubes[i]->cubeFaces[1]->color = color1;

				}
				else if (rubikCube->cubes[i]->z == 0 && rubikCube->cubes[i]->x < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
					rubikCube->cubes[i]->z = rubikCube->cubes[i]->x;
					rubikCube->cubes[i]->x = 0;
					rubikCube->cubes[i]->cubeFaces[2]->color = color1;

				}
				else if (rubikCube->cubes[i]->z == 0 && rubikCube->cubes[i]->x > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
					rubikCube->cubes[i]->z = rubikCube->cubes[i]->x;
					rubikCube->cubes[i]->x = 0;
					rubikCube->cubes[i]->cubeFaces[0]->color = color1;

				}

			}
		}
		break;
	}

	case ROTF_UP_Y_LEFT:
	{
		COLORREF color1;
		COLORREF color2;

		for (int i = 0; i < 27; i++)
		{
			if (rubikCube->cubes[i]->y > 0)
			{
				if (rubikCube->cubes[i]->x < 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[2]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->cubeFaces[2]->color = color1;
					rubikCube->cubes[i]->cubeFaces[3]->color = color2;

				}
				else if (rubikCube->cubes[i]->x < 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[1]->color;
					rubikCube->cubes[i]->z = -rubikCube->cubes[i]->z;
					rubikCube->cubes[i]->cubeFaces[1]->color = color1;
					rubikCube->cubes[i]->cubeFaces[2]->color = color2;
				}
				else if (rubikCube->cubes[i]->x > 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[3]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->cubeFaces[3]->color = color1;
					rubikCube->cubes[i]->cubeFaces[0]->color = color2;
				}
				else if (rubikCube->cubes[i]->x > 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[0]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->cubeFaces[0]->color = color1;
					rubikCube->cubes[i]->cubeFaces[1]->color = color2;
				}
				else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->z = 0;
					rubikCube->cubes[i]->cubeFaces[3]->color = color1;
				}
				else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->z = 0;
					rubikCube->cubes[i]->cubeFaces[1]->color = color1;

				}
				else if (rubikCube->cubes[i]->z == 0 && rubikCube->cubes[i]->x < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
					rubikCube->cubes[i]->z = rubikCube->cubes[i]->x;
					rubikCube->cubes[i]->x = 0;
					rubikCube->cubes[i]->cubeFaces[2]->color = color1;

				}
				else if (rubikCube->cubes[i]->z == 0 && rubikCube->cubes[i]->x > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
					rubikCube->cubes[i]->z = rubikCube->cubes[i]->x;
					rubikCube->cubes[i]->x = 0;
					rubikCube->cubes[i]->cubeFaces[0]->color = color1;

				}

			}
		}
		break;
	}
	case ROTF_M_X_DOWN:
	{
		COLORREF color1;
		COLORREF color2;
		for (int i = 0; i < 27; i++)
		{
			if (rubikCube->cubes[i]->x == 0)
			{
				if (rubikCube->cubes[i]->y < 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[5]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
					rubikCube->cubes[i]->cubeFaces[4]->color = color1;
					rubikCube->cubes[i]->cubeFaces[2]->color = color2;

				}
				else if (rubikCube->cubes[i]->y < 0 && rubikCube->cubes[i]->z == 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
					rubikCube->cubes[i]->z = rubikCube->cubes[i]->y;
					rubikCube->cubes[i]->y = 0;
					rubikCube->cubes[i]->cubeFaces[2]->color = color1;

				}
				else if (rubikCube->cubes[i]->y < 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[0]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->cubeFaces[2]->color = color1;
					rubikCube->cubes[i]->cubeFaces[5]->color = color2;

				}
				else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->z = 0;
					rubikCube->cubes[i]->cubeFaces[5]->color = color1;

				}
				else if (rubikCube->cubes[i]->y > 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[0]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
					rubikCube->cubes[i]->cubeFaces[0]->color = color1;
					rubikCube->cubes[i]->cubeFaces[5]->color = color2;

				}
				else if (rubikCube->cubes[i]->y > 0 && rubikCube->cubes[i]->z == 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
					rubikCube->cubes[i]->z = rubikCube->cubes[i]->y;
					rubikCube->cubes[i]->y = 0;
					rubikCube->cubes[i]->cubeFaces[0]->color = color1;

				}
				else if (rubikCube->cubes[i]->y > 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[2]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->cubeFaces[0]->color = color1;
					rubikCube->cubes[i]->cubeFaces[4]->color = color2;

				}
				else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->z = 0;
					rubikCube->cubes[i]->cubeFaces[4]->color = color1;

				}
			}
		}
		break;
	}

	case ROTF_L_X_DOWN:
	{
		COLORREF color1;
		COLORREF color2;
		for (int i = 0; i < 27; i++)
		{
			if (rubikCube->cubes[i]->x < 0)
			{
				if (rubikCube->cubes[i]->y < 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[5]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
					rubikCube->cubes[i]->cubeFaces[4]->color = color1;
					rubikCube->cubes[i]->cubeFaces[2]->color = color2;

				}
				else if (rubikCube->cubes[i]->y < 0 && rubikCube->cubes[i]->z == 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
					rubikCube->cubes[i]->z = rubikCube->cubes[i]->y;
					rubikCube->cubes[i]->y = 0;
					rubikCube->cubes[i]->cubeFaces[2]->color = color1;

				}
				else if (rubikCube->cubes[i]->y < 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[0]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->cubeFaces[2]->color = color1;
					rubikCube->cubes[i]->cubeFaces[5]->color = color2;

				}
				else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->z = 0;
					rubikCube->cubes[i]->cubeFaces[5]->color = color1;

				}
				else if (rubikCube->cubes[i]->y > 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[0]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
					rubikCube->cubes[i]->cubeFaces[0]->color = color1;
					rubikCube->cubes[i]->cubeFaces[5]->color = color2;

				}
				else if (rubikCube->cubes[i]->y > 0 && rubikCube->cubes[i]->z == 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
					rubikCube->cubes[i]->z = rubikCube->cubes[i]->y;
					rubikCube->cubes[i]->y = 0;
					rubikCube->cubes[i]->cubeFaces[0]->color = color1;

				}
				else if (rubikCube->cubes[i]->y > 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[2]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->cubeFaces[0]->color = color1;
					rubikCube->cubes[i]->cubeFaces[4]->color = color2;

				}
				else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->z = 0;
					rubikCube->cubes[i]->cubeFaces[4]->color = color1;

				}
			}
		}
		break;
	}
	case ROTF_R_X_DOWN:
	{
		COLORREF color1;
		COLORREF color2;
		for (int i = 0; i < 27; i++)
		{
			if (rubikCube->cubes[i]->x > 0)
			{
				if (rubikCube->cubes[i]->y < 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[5]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
					rubikCube->cubes[i]->cubeFaces[4]->color = color1;
					rubikCube->cubes[i]->cubeFaces[2]->color = color2;

				}
				else if (rubikCube->cubes[i]->y < 0 && rubikCube->cubes[i]->z == 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
					rubikCube->cubes[i]->z = rubikCube->cubes[i]->y;
					rubikCube->cubes[i]->y = 0;
					rubikCube->cubes[i]->cubeFaces[2]->color = color1;

				}
				else if (rubikCube->cubes[i]->y < 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[0]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->cubeFaces[2]->color = color1;
					rubikCube->cubes[i]->cubeFaces[5]->color = color2;

				}
				else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->z = 0;
					rubikCube->cubes[i]->cubeFaces[5]->color = color1;

				}
				else if (rubikCube->cubes[i]->y > 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[0]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
					rubikCube->cubes[i]->cubeFaces[0]->color = color1;
					rubikCube->cubes[i]->cubeFaces[5]->color = color2;

				}
				else if (rubikCube->cubes[i]->y > 0 && rubikCube->cubes[i]->z == 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
					rubikCube->cubes[i]->z = rubikCube->cubes[i]->y;
					rubikCube->cubes[i]->y = 0;
					rubikCube->cubes[i]->cubeFaces[0]->color = color1;

				}
				else if (rubikCube->cubes[i]->y > 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[2]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->cubeFaces[0]->color = color1;
					rubikCube->cubes[i]->cubeFaces[4]->color = color2;

				}
				else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->z = 0;
					rubikCube->cubes[i]->cubeFaces[4]->color = color1;

				}
			}
		}
		break;
	}

	case ROTF_D_Y_RIGHT:
	{
		COLORREF color1;
		COLORREF color2;

		for (int i = 0; i < 27; i++)
		{
			if (rubikCube->cubes[i]->y < 0)
			{
				if (rubikCube->cubes[i]->x < 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[2]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->cubeFaces[0]->color = color1;
					rubikCube->cubes[i]->cubeFaces[1]->color = color2;

				}
				else if (rubikCube->cubes[i]->x < 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[1]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->cubeFaces[3]->color = color1;
					rubikCube->cubes[i]->cubeFaces[0]->color = color2;
				}
				else if (rubikCube->cubes[i]->x > 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[3]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->cubeFaces[1]->color = color1;
					rubikCube->cubes[i]->cubeFaces[2]->color = color2;
				}
				else if (rubikCube->cubes[i]->x > 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[0]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->cubeFaces[2]->color = color1;
					rubikCube->cubes[i]->cubeFaces[3]->color = color2;
				}
				else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
					rubikCube->cubes[i]->x = rubikCube->cubes[i]->z;
					rubikCube->cubes[i]->z = 0;
					rubikCube->cubes[i]->cubeFaces[1]->color = color1;
				}
				else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
					rubikCube->cubes[i]->x = rubikCube->cubes[i]->z;
					rubikCube->cubes[i]->z = 0;
					rubikCube->cubes[i]->cubeFaces[3]->color = color1;

				}
				else if (rubikCube->cubes[i]->z == 0 && rubikCube->cubes[i]->x < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->x = 0;
					rubikCube->cubes[i]->cubeFaces[0]->color = color1;

				}
				else if (rubikCube->cubes[i]->z == 0 && rubikCube->cubes[i]->x > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->x = 0;
					rubikCube->cubes[i]->cubeFaces[2]->color = color1;

				}

			}
		}
		break;
	}
	case ROTF_M_Y_RIGHT:
	{
		COLORREF color1;
		COLORREF color2;

		for (int i = 0; i < 27; i++)
		{
			if (rubikCube->cubes[i]->y == 0)
			{
				if (rubikCube->cubes[i]->x < 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[2]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->cubeFaces[0]->color = color1;
					rubikCube->cubes[i]->cubeFaces[1]->color = color2;

				}
				else if (rubikCube->cubes[i]->x < 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[1]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->cubeFaces[3]->color = color1;
					rubikCube->cubes[i]->cubeFaces[0]->color = color2;
				}
				else if (rubikCube->cubes[i]->x > 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[3]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->cubeFaces[1]->color = color1;
					rubikCube->cubes[i]->cubeFaces[2]->color = color2;
				}
				else if (rubikCube->cubes[i]->x > 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[0]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->cubeFaces[2]->color = color1;
					rubikCube->cubes[i]->cubeFaces[3]->color = color2;
				}
				else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
					rubikCube->cubes[i]->x = rubikCube->cubes[i]->z;
					rubikCube->cubes[i]->z = 0;
					rubikCube->cubes[i]->cubeFaces[1]->color = color1;
				}
				else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
					rubikCube->cubes[i]->x = rubikCube->cubes[i]->z;
					rubikCube->cubes[i]->z = 0;
					rubikCube->cubes[i]->cubeFaces[3]->color = color1;

				}
				else if (rubikCube->cubes[i]->z == 0 && rubikCube->cubes[i]->x < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->x = 0;
					rubikCube->cubes[i]->cubeFaces[0]->color = color1;

				}
				else if (rubikCube->cubes[i]->z == 0 && rubikCube->cubes[i]->x > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->x = 0;
					rubikCube->cubes[i]->cubeFaces[2]->color = color1;

				}

			}
		}
		break;
	}
	case ROTF_UP_Y_RIGHT:
	{
		COLORREF color1;
		COLORREF color2;

		for (int i = 0; i < 27; i++)
		{
			if (rubikCube->cubes[i]->y > 0)
			{
				if (rubikCube->cubes[i]->x < 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[2]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->cubeFaces[0]->color = color1;
					rubikCube->cubes[i]->cubeFaces[1]->color = color2;

				}
				else if (rubikCube->cubes[i]->x < 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[1]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->cubeFaces[3]->color = color1;
					rubikCube->cubes[i]->cubeFaces[0]->color = color2;
				}
				else if (rubikCube->cubes[i]->x > 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[3]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->cubeFaces[1]->color = color1;
					rubikCube->cubes[i]->cubeFaces[2]->color = color2;
				}
				else if (rubikCube->cubes[i]->x > 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[0]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->z);
					rubikCube->cubes[i]->cubeFaces[2]->color = color1;
					rubikCube->cubes[i]->cubeFaces[3]->color = color2;
				}
				else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->z < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[2]->color;
					rubikCube->cubes[i]->x = rubikCube->cubes[i]->z;
					rubikCube->cubes[i]->z = 0;
					rubikCube->cubes[i]->cubeFaces[1]->color = color1;
				}
				else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->z > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[0]->color;
					rubikCube->cubes[i]->x = rubikCube->cubes[i]->z;
					rubikCube->cubes[i]->z = 0;
					rubikCube->cubes[i]->cubeFaces[3]->color = color1;

				}
				else if (rubikCube->cubes[i]->z == 0 && rubikCube->cubes[i]->x < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->x = 0;
					rubikCube->cubes[i]->cubeFaces[0]->color = color1;

				}
				else if (rubikCube->cubes[i]->z == 0 && rubikCube->cubes[i]->x > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
					rubikCube->cubes[i]->z = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->x = 0;
					rubikCube->cubes[i]->cubeFaces[2]->color = color1;

				}

			}
		}
		break;
	}
	case ROTF_FRONT_Z_RIGHT:
	{
		COLORREF color1;
		COLORREF color2;

		for (int i = 0; i < 27; i++)
		{
			if (rubikCube->cubes[i]->z > 0)
			{
				if (rubikCube->cubes[i]->x < 0 && rubikCube->cubes[i]->y < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[1]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
					rubikCube->cubes[i]->cubeFaces[1]->color = color1;
					rubikCube->cubes[i]->cubeFaces[4]->color = color2;

				}
				else if (rubikCube->cubes[i]->x < 0 && rubikCube->cubes[i]->y > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[1]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->cubeFaces[3]->color = color1;
					rubikCube->cubes[i]->cubeFaces[4]->color = color2;
				}
				else if (rubikCube->cubes[i]->x > 0 && rubikCube->cubes[i]->y < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[3]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->cubeFaces[1]->color = color1;
					rubikCube->cubes[i]->cubeFaces[5]->color = color2;
				}
				else if (rubikCube->cubes[i]->x > 0 && rubikCube->cubes[i]->y > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[4]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
					rubikCube->cubes[i]->cubeFaces[5]->color = color1;
					rubikCube->cubes[i]->cubeFaces[3]->color = color2;
				}
				else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->y < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
					rubikCube->cubes[i]->x = rubikCube->cubes[i]->y;
					rubikCube->cubes[i]->y = 0;
					rubikCube->cubes[i]->cubeFaces[1]->color = color1;
				}
				else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->y > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
					rubikCube->cubes[i]->x = rubikCube->cubes[i]->y;
					rubikCube->cubes[i]->y = 0;
					rubikCube->cubes[i]->cubeFaces[3]->color = color1;

				}
				else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->x < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->x = 0;
					rubikCube->cubes[i]->cubeFaces[4]->color = color1;

				}
				else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->x > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->x = 0;
					rubikCube->cubes[i]->cubeFaces[5]->color = color1;

				}

			}
		}
		break;
	}
	case ROTF_M_Z_RIGHT:
	{
		COLORREF color1;
		COLORREF color2;

		for (int i = 0; i < 27; i++)
		{
			if (rubikCube->cubes[i]->z == 0)
			{
				if (rubikCube->cubes[i]->x < 0 && rubikCube->cubes[i]->y < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[1]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
					rubikCube->cubes[i]->cubeFaces[1]->color = color1;
					rubikCube->cubes[i]->cubeFaces[4]->color = color2;

				}
				else if (rubikCube->cubes[i]->x < 0 && rubikCube->cubes[i]->y > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[1]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->cubeFaces[3]->color = color1;
					rubikCube->cubes[i]->cubeFaces[4]->color = color2;
				}
				else if (rubikCube->cubes[i]->x > 0 && rubikCube->cubes[i]->y < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[3]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->cubeFaces[1]->color = color1;
					rubikCube->cubes[i]->cubeFaces[5]->color = color2;
				}
				else if (rubikCube->cubes[i]->x > 0 && rubikCube->cubes[i]->y > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[4]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
					rubikCube->cubes[i]->cubeFaces[5]->color = color1;
					rubikCube->cubes[i]->cubeFaces[3]->color = color2;
				}
				else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->y < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
					rubikCube->cubes[i]->x = rubikCube->cubes[i]->y;
					rubikCube->cubes[i]->y = 0;
					rubikCube->cubes[i]->cubeFaces[1]->color = color1;
				}
				else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->y > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
					rubikCube->cubes[i]->x = rubikCube->cubes[i]->y;
					rubikCube->cubes[i]->y = 0;
					rubikCube->cubes[i]->cubeFaces[3]->color = color1;

				}
				else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->x < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->x = 0;
					rubikCube->cubes[i]->cubeFaces[4]->color = color1;

				}
				else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->x > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->x = 0;
					rubikCube->cubes[i]->cubeFaces[5]->color = color1;

				}

			}
		}
		break;
	}
	case ROTF_BACK_Z_RIGHT:
	{
		COLORREF color1;
		COLORREF color2;

		for (int i = 0; i < 27; i++)
		{
			if (rubikCube->cubes[i]->z < 0)
			{
				if (rubikCube->cubes[i]->x < 0 && rubikCube->cubes[i]->y < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[1]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
					rubikCube->cubes[i]->cubeFaces[1]->color = color1;
					rubikCube->cubes[i]->cubeFaces[4]->color = color2;

				}
				else if (rubikCube->cubes[i]->x < 0 && rubikCube->cubes[i]->y > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[1]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->cubeFaces[3]->color = color1;
					rubikCube->cubes[i]->cubeFaces[4]->color = color2;
				}
				else if (rubikCube->cubes[i]->x > 0 && rubikCube->cubes[i]->y < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[3]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->cubeFaces[1]->color = color1;
					rubikCube->cubes[i]->cubeFaces[5]->color = color2;
				}
				else if (rubikCube->cubes[i]->x > 0 && rubikCube->cubes[i]->y > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[4]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
					rubikCube->cubes[i]->cubeFaces[5]->color = color1;
					rubikCube->cubes[i]->cubeFaces[3]->color = color2;
				}
				else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->y < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
					rubikCube->cubes[i]->x = rubikCube->cubes[i]->y;
					rubikCube->cubes[i]->y = 0;
					rubikCube->cubes[i]->cubeFaces[1]->color = color1;
				}
				else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->y > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
					rubikCube->cubes[i]->x = rubikCube->cubes[i]->y;
					rubikCube->cubes[i]->y = 0;
					rubikCube->cubes[i]->cubeFaces[3]->color = color1;

				}
				else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->x < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->x = 0;
					rubikCube->cubes[i]->cubeFaces[4]->color = color1;

				}
				else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->x > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->x = 0;
					rubikCube->cubes[i]->cubeFaces[5]->color = color1;

				}

			}
		}
		break;
	}
	case ROTF_BACK_Z_LEFT:
	{
		COLORREF color1;
		COLORREF color2;

		for (int i = 0; i < 27; i++)
		{
			if (rubikCube->cubes[i]->z < 0)
			{
				if (rubikCube->cubes[i]->x < 0 && rubikCube->cubes[i]->y < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[1]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->cubeFaces[3]->color = color1;
					rubikCube->cubes[i]->cubeFaces[5]->color = color2;

				}
				else if (rubikCube->cubes[i]->x < 0 && rubikCube->cubes[i]->y > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[1]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
					rubikCube->cubes[i]->cubeFaces[1]->color = color1;
					rubikCube->cubes[i]->cubeFaces[5]->color = color2;
				}
				else if (rubikCube->cubes[i]->x > 0 && rubikCube->cubes[i]->y < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[3]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
					rubikCube->cubes[i]->cubeFaces[3]->color = color1;
					rubikCube->cubes[i]->cubeFaces[4]->color = color2;
				}
				else if (rubikCube->cubes[i]->x > 0 && rubikCube->cubes[i]->y > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[4]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->cubeFaces[4]->color = color1;
					rubikCube->cubes[i]->cubeFaces[1]->color = color2;
				}
				else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->y < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->y);
					rubikCube->cubes[i]->y = 0;
					rubikCube->cubes[i]->cubeFaces[3]->color = color1;
				}
				else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->y > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->y);
					rubikCube->cubes[i]->y = 0;
					rubikCube->cubes[i]->cubeFaces[1]->color = color1;

				}
				else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->x < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
					rubikCube->cubes[i]->y = rubikCube->cubes[i]->x;
					rubikCube->cubes[i]->x = 0;
					rubikCube->cubes[i]->cubeFaces[5]->color = color1;

				}
				else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->x > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
					rubikCube->cubes[i]->y = rubikCube->cubes[i]->x;
					rubikCube->cubes[i]->x = 0;
					rubikCube->cubes[i]->cubeFaces[4]->color = color1;

				}

			}
		}
		break;
	}
	case ROTF_M_Z_LEFT:
	{
		COLORREF color1;
		COLORREF color2;

		for (int i = 0; i < 27; i++)
		{
			if (rubikCube->cubes[i]->z == 0)
			{
				if (rubikCube->cubes[i]->x < 0 && rubikCube->cubes[i]->y < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[1]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->cubeFaces[3]->color = color1;
					rubikCube->cubes[i]->cubeFaces[5]->color = color2;

				}
				else if (rubikCube->cubes[i]->x < 0 && rubikCube->cubes[i]->y > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[1]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
					rubikCube->cubes[i]->cubeFaces[1]->color = color1;
					rubikCube->cubes[i]->cubeFaces[5]->color = color2;
				}
				else if (rubikCube->cubes[i]->x > 0 && rubikCube->cubes[i]->y < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[3]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
					rubikCube->cubes[i]->cubeFaces[3]->color = color1;
					rubikCube->cubes[i]->cubeFaces[4]->color = color2;
				}
				else if (rubikCube->cubes[i]->x > 0 && rubikCube->cubes[i]->y > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[4]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->cubeFaces[4]->color = color1;
					rubikCube->cubes[i]->cubeFaces[1]->color = color2;
				}
				else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->y < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->y);
					rubikCube->cubes[i]->y = 0;
					rubikCube->cubes[i]->cubeFaces[3]->color = color1;
				}
				else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->y > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->y);
					rubikCube->cubes[i]->y = 0;
					rubikCube->cubes[i]->cubeFaces[1]->color = color1;

				}
				else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->x < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
					rubikCube->cubes[i]->y = rubikCube->cubes[i]->x;
					rubikCube->cubes[i]->x = 0;
					rubikCube->cubes[i]->cubeFaces[5]->color = color1;

				}
				else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->x > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
					rubikCube->cubes[i]->y = rubikCube->cubes[i]->x;
					rubikCube->cubes[i]->x = 0;
					rubikCube->cubes[i]->cubeFaces[4]->color = color1;

				}

			}
		}
		break;
	}
	case ROTF_FRONT_Z_LEFT:
	{
		COLORREF color1;
		COLORREF color2;

		for (int i = 0; i < 27; i++)
		{
			if (rubikCube->cubes[i]->z > 0)
			{
				if (rubikCube->cubes[i]->x < 0 && rubikCube->cubes[i]->y < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[1]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->cubeFaces[3]->color = color1;
					rubikCube->cubes[i]->cubeFaces[5]->color = color2;

				}
				else if (rubikCube->cubes[i]->x < 0 && rubikCube->cubes[i]->y > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[1]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
					rubikCube->cubes[i]->cubeFaces[1]->color = color1;
					rubikCube->cubes[i]->cubeFaces[5]->color = color2;
				}
				else if (rubikCube->cubes[i]->x > 0 && rubikCube->cubes[i]->y < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[3]->color;
					rubikCube->cubes[i]->y = -(rubikCube->cubes[i]->y);
					rubikCube->cubes[i]->cubeFaces[3]->color = color1;
					rubikCube->cubes[i]->cubeFaces[4]->color = color2;
				}
				else if (rubikCube->cubes[i]->x > 0 && rubikCube->cubes[i]->y > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
					color2 = rubikCube->cubes[i]->cubeFaces[4]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->x);
					rubikCube->cubes[i]->cubeFaces[4]->color = color1;
					rubikCube->cubes[i]->cubeFaces[1]->color = color2;
				}
				else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->y < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[5]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->y);
					rubikCube->cubes[i]->y = 0;
					rubikCube->cubes[i]->cubeFaces[3]->color = color1;
				}
				else if (rubikCube->cubes[i]->x == 0 && rubikCube->cubes[i]->y > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[4]->color;
					rubikCube->cubes[i]->x = -(rubikCube->cubes[i]->y);
					rubikCube->cubes[i]->y = 0;
					rubikCube->cubes[i]->cubeFaces[1]->color = color1;

				}
				else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->x < 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[1]->color;
					rubikCube->cubes[i]->y = rubikCube->cubes[i]->x;
					rubikCube->cubes[i]->x = 0;
					rubikCube->cubes[i]->cubeFaces[5]->color = color1;

				}
				else if (rubikCube->cubes[i]->y == 0 && rubikCube->cubes[i]->x > 0)
				{
					color1 = rubikCube->cubes[i]->cubeFaces[3]->color;
					rubikCube->cubes[i]->y = rubikCube->cubes[i]->x;
					rubikCube->cubes[i]->x = 0;
					rubikCube->cubes[i]->cubeFaces[4]->color = color1;

				}

			}
		}
		break;
	}
	}

}


void CGlView::showRotateAnimation(int flag)
{
	

	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -8.0f);

	glRotatef(xAngle, 1, 0, 0);
	glRotatef(yAngle, 0, 1, 0);
	glRotatef(zAngle, 0, 0, 1);
	
	//axes
	//glBegin(GL_LINES);
	////x  RED
	//glColor3f(1.0f, 0.0f, 0.0f);
	//glVertex3f(-3.0f, 0.0f, 0.0f);
	//glVertex3f(3.0f, 0.0f, 0.0f);
	////y  GREEN 
	//glColor3f(0.0f, 1.0f, 0.0f);
	//glVertex3f(0.0f, -3.0f, 0.0f);
	//glVertex3f(0.0f, 3.0f, 0.0f);
	////z  BLUE
	//glColor3f(0.0f, 0.0f, 1.0f);
	//glVertex3f(0.0f, 0.0f, -3.0f);
	//glVertex3f(0.0f, 0.0f, 3.0f);
	//glEnd();

	glPushMatrix();

	//	draw cube
	for (int i = 0; i < 27; i++)
	{
		glPushMatrix();

		switch (flag)
		{

			case ROTF_L_X_UP:
			if (rubikCube->cubes[i]->x < 0)
			{
				//glRotatef(rotLeftF, 1, 0, 0)
				glRotatef(xRotateAnimate, 1, 0, 0);

			}
			break;
			case ROTF_M_X_UP:
				if (rubikCube->cubes[i]->x == 0)
				{
					//glRotatef(rotLeftF, 1, 0, 0)
					glRotatef(xRotateAnimate, 1, 0, 0);

				}
				break;
			case ROTF_R_X_UP:
				if (rubikCube->cubes[i]->x > 0)
				{
					glRotatef(xRotateAnimate, 1, 0, 0);
				}
				break;
			case ROTF_R_X_DOWN:
				if (rubikCube->cubes[i]->x > 0)
				{
					glRotatef(xRotateAnimate, 1, 0, 0);
				}
				break;
			case ROTF_M_X_DOWN:
				if (rubikCube->cubes[i]->x == 0)
				{
					glRotatef(xRotateAnimate, 1, 0, 0);
				}
				break;
			case ROTF_L_X_DOWN:
				if (rubikCube->cubes[i]->x < 0)
				{
					glRotatef(xRotateAnimate, 1, 0, 0);
				}
				break;
			case ROTF_D_Y_LEFT:
				if (rubikCube->cubes[i]->y < 0)
				{
					glRotatef(yRotateAnimate, 0, 1, 0);
				}
				break;
			case ROTF_D_Y_RIGHT:
				if (rubikCube->cubes[i]->y < 0)
				{
					glRotatef(yRotateAnimate, 0, 1, 0);
				}
				break;
			case ROTF_M_Y_RIGHT:
				if (rubikCube->cubes[i]->y == 0)
				{
					glRotatef(yRotateAnimate, 0, 1, 0);
				}
				break;
			case ROTF_M_Y_LEFT:
				if (rubikCube->cubes[i]->y == 0)
				{
					glRotatef(yRotateAnimate, 0, 1, 0);
				}
				break;
			case ROTF_UP_Y_LEFT:
				if (rubikCube->cubes[i]->y > 0)
				{
					glRotatef(yRotateAnimate, 0, 1, 0);
				}
				break;
			case ROTF_UP_Y_RIGHT:
				if (rubikCube->cubes[i]->y > 0)
				{
					glRotatef(yRotateAnimate, 0, 1, 0);
				}
				break;
			case ROTF_FRONT_Z_RIGHT:
				if (rubikCube->cubes[i]->z > 0)
				{
					glRotatef(zRotateAnimate, 0, 0, 1);
				}
				break;
			case ROTF_M_Z_RIGHT:
				if (rubikCube->cubes[i]->z == 0)
				{
					glRotatef(zRotateAnimate, 0, 0, 1);
				}
				break;
			case ROTF_BACK_Z_RIGHT:
				if (rubikCube->cubes[i]->z < 0)
				{
					glRotatef(zRotateAnimate, 0, 0, 1);
				}
				break;
			case ROTF_FRONT_Z_LEFT:
				if (rubikCube->cubes[i]->z > 0)
				{
					glRotatef(zRotateAnimate, 0, 0, 1);
				}
				break;
			case ROTF_M_Z_LEFT:
				if (rubikCube->cubes[i]->z == 0)
				{
					glRotatef(zRotateAnimate, 0, 0, 1);
				}
				break;
			case ROTF_BACK_Z_LEFT:
				if (rubikCube->cubes[i]->z < 0)
				{
					glRotatef(zRotateAnimate, 0, 0, 1);
				}
				break;
			case ROT_Z_LEFT:
				/*if (rubikCube->cubes[i]->z < 0)
				{*/
					glRotatef(zRotCube, 0, 0, 1);
				/*}*/
				break;
			case ROT_Z_RIGHT:
				/*if (rubikCube->cubes[i]->z < 0)
				{*/
					glRotatef(zRotCube, 0, 0, 1);
				/*}*/
				break;
			case ROTATE_X_DOWN:
				/*if (rubikCube->cubes[i]->z < 0)
				{*/
				glRotatef(xRotCube, 1, 0, 0);
				/*}*/
				break;
			case  ROTATE_X_UP:
				glRotatef(xRotCube, 1, 0, 0);
				break;
			case  ROTATE_Y_RIGHT:
				glRotatef(yRotCube, 0, 1, 0);
				break;
			case  ROTATE_Y_LEFT:
				glRotatef(yRotCube, 0, 1, 0);
				break;
		}
		rubikCube->cubes[i]->drawCube();
		glPopMatrix();
		//PopMat();
	}

	glPopMatrix();
	//PopMat();

	/*glEnableClientState(GL_VERTEX_ARRAY);*/
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	/*glVertexPointer(3, GL_FLOAT, 0, vertics);
	glDrawArrays(GL_QUADS, 0, 24);*/



	glDisable(GL_COLOR_MATERIAL);// enable colors on the object
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	glDisableClientState(GL_VERTEX_ARRAY);

	// swap the buffer
	SwapBuffers(m_hDC);
}
void  CGlView::RandomColorsCube()
{
	for (int i = 1; i < 9; i++)
	{
		int randIndex = rand() % 17;
		Update(randomFlagsColor[randIndex]);
	}
	
	vDrawGLScene();
}
void  CGlView::RotateAnimateCube(int flag)
{
	switch (flag)
	{
	case ROT_X_UP:
	{
		glPushMatrix();
		xAngle -= 30;
		
		vDrawGLScene();
		glPopMatrix();
		
		while (xAngle >= -(CUBE_ROTATE_ANGLE*2))
		{
		}
		/*if (xAngle <= -(CUBE_ROTATE_ANGLE * 2))
		{
			Update(flag);
			xAngle = 0;
			vDrawGLScene();
			return;
		}*/

		
	}
		break;
	case ROT_X_DOWN:
	{

	}
	break;
	case ROT_Y_LEFT:
	{

	}
	break;
	case ROT_Y_RIGHT:
	{

	}
	break;
	}
}


