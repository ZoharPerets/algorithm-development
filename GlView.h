// GlView.h: interface for the CGlView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLVIEW_H__6EDA23AE_B764_11D5_8D52_0004AC3DB799__INCLUDED_)
#define AFX_GLVIEW_H__6EDA23AE_B764_11D5_8D52_0004AC3DB799__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#include "BigCube.h"
#include "Cube.h"
#include "RubikCubeAlgorithem.h"


#endif // _MSC_VER > 1000
#define DISTANCE_RADIUS 0.5
#define CUBE_ROTATE_ANGLE 90
#define TIME_GAME_FLAG 35
#define TIMER_ANIMATE_FLAG 36
#define TIMER_SOLVE_LEVEL1_FLAG 37
#define TIMER_SOLVE_LEVEL2_FLAG 38
#define TIMER_SOLVE_LEVEL3_FLAG 39
#define TIMER_SOLVE_LEVEL4_FLAG 40
#define TIMER_SOLVE_LEVEL5_FLAG 41
#define TIMER_SOLVE_LEVEL6_FLAG 42

#define TIMER_ANIMATE_ROTATE_CUBE_FLAG 43



class CGlView  
{
public:
	
	CGlView(CWnd *pclWnd);
	~CGlView();

	void vDrawGLScene();

	protected:
	private:
		void vInitGl(int iWidth, int iHeigth);
		BOOL bSetWindowPixelFormat(HDC hDC);
		BOOL bCreateViewGLContext(HDC hDC);

	// members //////////////////////////////////////////////////////////
	public:
		float xRotateAnimate = 0;
		float yRotateAnimate = 0;
		float zRotateAnimate = 0;
		float xBeforeRotateAnimate = 0;
		float yBeforeRotateAnimate = 0;
		float zBeforeRotateAnimate = 0;
		float xAngle=20;
		float xAngleBefore = 0;
		float yAngle= 15;
		float zAngle=0;
		float zRotCube = 0;
		float xRotCube = 0;
		float yRotCube = 0;
		int flag = 0;
		int flagStopLevel1 = 0;
		int flagStopLevel2 = 0;
		int flagStopLevel3 = 0;
		int flagStopLevel4 = 0;
		int flagStopLevel5 = 0;
		int flagTimer = 0;
		Cube *cubesToSelect[9];
		int cubesToMoveLengh=0;
		BigCube* rubikCube;
		RubikCubeAlgorithem* objSolverCube;
		int randomFlagsColor[17];
		bool flag_Y_Angale = false;
		void Update(int flag);
		void showRotateAnimation(int flag);
		void RandomColorsCube();
		void RotateAnimateCube(int flag);


		//void rotateX()
	protected:
	private:			
		HGLRC	m_hGLContext;	// required GLContext
		CWnd *m_pclWnd;			// window inwhich opengl will be drawn
		HDC		m_hDC;			// handle to m_pclWnds device context

};

#endif

