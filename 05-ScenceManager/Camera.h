#pragma once
#include "Game.h"
#include "GameObject.h"
#include "Mario.h"
#include "Utils.h"

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
	void UpdateCameraScence4();
};
