#pragma once
#include "Game.h"
#include "GameObject.h"
#include "Mario.h"
#include "Utils.h"

//
//#define WIDTH_MAX_MAP_1				2816
//#define NEAR_WIDTH_LIMIT_MAP_1		2656
//
//#define WIDTH_MIN_MAP_1				0
//#define WIDTH_MIN_MAP_2				0
//#define WIDTH_MAX_MAP_2				2554
//#define NEAR_WIDTH_LIMIT_MAP_2		2404
//
//#define CAM_X_MAX_2					2254
//
//#define CAM_X_MAX					2504
//#define CAM_X_MIN					0
//
//#define UNDERGROUND_Y				432.0f
//#define CAM_Y_UNDERGROUND			507.0f
//
//#define CAM_Y_MIN					267.0f
//
//#define CAM_X_SPEED_1_4				0.0315f 
//#define CAM_X_RANGE_AUTO_1_4		1743
//#define CAM_X_RANGE_NORMAL_1_4		2000
//
//#define CAM_Y_PLUS					20
//
//#define ID_SCENE_MAP_1				1
//#define ID_SCENE_MAP_4				4
//
//// camera scene 3
//#define CAM_3_X_MIN				16.0f
//#define CAM_3_X_DEFAULT			667.0f



class CCamera {
	float cameraX;
	float cameraY;
	float playerX ;
	float playerY ;
	CMario* player;
	int sceneID;
public:
	CCamera(CMario* m, int id);
	float GetPositionCameraX() { return cameraX; }
	float GetPositionCameraY() { return cameraY; }
	void SetPositionCamera(float x, float y) { cameraX = x; cameraY = y; }
	void UpdateCamera();
	void UpdateCameraScence1();
	void UpdateCameraScence2();
	void UpdateCameraScence3();
	void UpdateCameraScence3Right();
	void UpdateCameraScence4();
};
