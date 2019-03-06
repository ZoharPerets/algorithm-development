#pragma once
#include <gl\gl.h>			
#include <gl\glu.h>	 
#include "BigCube.h"
#include "Face.h"

#define ROT_X_UP 1
#define ROT_X_DOWN 2
#define ROT_Y_LEFT 3
#define ROT_Y_RIGHT 4
#define ROT_Z_LEFT 5
#define ROT_Z_RIGHT 6
#define ROTF_L_X_UP 7
#define ROTF_L_X_DOWN 8
#define ROTF_D_Y_LEFT 9
#define ROTF_D_Y_RIGHT 10
#define ROTF_M_X_UP 11
#define ROTF_M_Y_LEFT 12
#define ROTF_UP_Y_LEFT 13
#define ROTF_M_Y_RIGHT 14
#define ROTF_UP_Y_RIGHT 15
#define ROTF_R_X_UP 16
#define ROTF_R_X_DOWN 17
#define ROTF_M_X_DOWN 18
#define ROTF_FRONT_Z_LEFT 19
#define ROTF_M_Z_LEFT 20
#define ROTF_BACK_Z_LEFT 21
#define ROTF_FRONT_Z_RIGHT 22
#define ROTF_M_Z_RIGHT 23
#define ROTF_BACK_Z_RIGHT 24
#define X_ROTATE 25
#define Y_ROTATE 26
#define Z_ROTATE 27
#define ROTATE_X_DOWN 28
#define ROTATE_X_UP 29
#define ROT_Z_RIGHT 30
#define ROT_Z_LEFT 31
#define Z_ROTAT_CUBE 32
#define X_ROTAT_CUBE 33
#define FLAG_LEVEL1 44
#define FLAG_LEVEL2 45
#define FLAG_LEVEL3 46
#define FLAG_STOP_SOLVE 47
#define FLAG_FACE_LEFT 48
#define FLAG_FACE_RIGHT 49
#define FLAG_FACE_FRONT 50
#define FLAG_FACE_BACK 51
#define LEVEL2_PART1 52
#define LEVEL2_PART2 53
#define FLAG_FACE_FRONT_LEFT 54
#define FLAG_FACE_FRONT_RIGHT 55
#define FLAG_FACE_BACK_LEFT 56
#define FLAG_FACE_BACK_RIGHT 57
#define FLAG_FACE_RIGHT_FRONT 58
#define FLAG_FACE_RIGHT_BACK 59
#define FLAG_FACE_BACK_RIGHT 60
#define FLAG_FACE_LEFT_FRONT 61
#define FLAG_FACE_BACK_LEFT 62
#define FLAG_FACE_LEFT_BACK 63
#define STOP_LEVEL1 64
#define STOP_LEVEL2 65
#define LEVEL2_PART3 66
#define FLAG_LEVEL4 67
#define ALGORITHEM_TO_RIGHT 68
#define ALGORITHEM_TO_LEFT 69
#define LEVEL3_PART1 70
#define LEVEL3_PART2 71
#define CANT_SOLVE 72
#define FLAG_LEVEL5 73
#define CUBE_SOLVED 74
#define LEVEL4_PART1 75
#define LEVEL4_PART2 76
#define FLAG_LEVEL6 77
#define ROTATE_Y_LEFT 78
#define ROTATE_Y_RIGHT 79
#define Y_ROTAT_CUBE 80
#define LEVEL6_PART1 81
#define LEVEL6_PART2 82
#define LEVEL3_PART1 83
#define LEVEL3_PART2 84












class RubikCubeAlgorithem
{
public:

	RubikCubeAlgorithem(BigCube* objRubikCube);
	RubikCubeAlgorithem(int flagLevel, BigCube* objRubikCube);
	int SolveLevel1();
	int SolveLevel2();
	int SolveLevel3();
	int SolveLevel4();
	int SolveLevel5();
	int SolveLevel6();


	bool CheckIfSolved();
	int flagLevel;

private:
	void UpdateCenterColors();
	bool isSolveNow;
	bool CheckIfSolvedLevel2();
	BigCube* objRubikCube;
	COLORREF checkColor;
	int mode = 0;
	int partLevel3;
	int flagFaceIndexPositionLeft;
	int flagFaceIndexPositionRight;
	int flagFaceIndexPositionBack;
	int flagFaceIndexPositionFront;
	int flagPosition = 4;
	int timesLeftFace;
	int timesRightFace;
	int timesFrontFace;
	int timesBackFace;
	int timesManipulateLevel1;
	int timesManipulateLevel2Part1=3;
	int timesManipulateLevel2Part2 = 4;
	int timesManipulateLevel2Part3 = 3;
	int timesManipulate2Level2Part2 = 2;
	int timesManipulate2Level2Part3 = 1;
	int timesManipulate2Level3= 1;
	int timesManipulateLevel3= 11;
	int timesManipulateLevel4 = 6;
	int timesManipulateLevel5 = 8;
	int timesManipulate2Level5 = 2;
	int timesManipulateLevel6 = 12;
	int timesManipulate2Level6 = 12;
	int modeLevel5 = 0;
	//int timesManipulate2Level4 = 7;
	//int timesManipulateLevel3Part2 = 3;
	int flagStopLevel1;
	int flagStopLevel2;
	int flagStopLevel3;
	int counter ;
	bool result ;
	bool isSolveNowPart2Level2 = false;
	bool isSolveNowPart3Level2 = false;
	bool isSolveNow2Part3Level2 = true;
	bool isSolveNowLevel5 = false;
	int timesToSolvePart2Level2 = 0;
	int LastLevelSolved ;
	COLORREF centerColorUP;
	COLORREF centerColorDown;
	COLORREF centerColorLeft;
	COLORREF centerColorRight;
	COLORREF centerColorFront;
	COLORREF centerColorBack;
	bool positionLeftExist ;
	bool positionBackExist ;
	bool positionRightExist ;
	bool positionFrontExist ;
	bool ifIsTryToSolveNow = false;
	bool isSolveLevel5Part2Now = false;
	bool isSolveLevel6Now = false;
	int partLevel2;
	int partLevel4= LEVEL4_PART1;
	int partLevel6 = LEVEL6_PART1;
	int ManipulateAnimateLevel1(int flagFace);
	int ManipulateAnimateLevel2(int flagFace, int partOfLevel);
	int ManipulateAnimateLevel2Part2(int flagFace);
	int ManipulateAnimateLevel2Part3(int flagFace);
	int ManipulateAnimateLevel3(int flagFace);
	int ManipulateAnimateLevel5();
	bool CheckIfSolvedPart3Level2();
	bool CheckIfSolvedPart1Level2();
	bool CheckIfSolvedPart2Level2();
	bool CheckIfSolvedLevel3();
	bool CheckIfSolvedLevel4();
	bool CheckIfSolvedLevel5();
	bool CheckIfSolvedLevel6();
	void CheckPartLevel3();
	void CheckPartLevel4();
	bool CheckModeLevel5();
	void CheckPartLevel6();


	int CheckMode(int flagFace);
	int Manipulate2AnimateLevel3(int flagFace);


	
	~RubikCubeAlgorithem();
	


};

