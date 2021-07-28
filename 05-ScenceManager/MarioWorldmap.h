#pragma once
#include "GameObject.h"
#include "Game.h"
#include "Node.h"

#define MARIO_INTRO_WALKING_SPEED						0.05f

#define MARIO_INTRO_SMALL_BBOX_WIDTH					16
#define MARIO_INTRO_SMALL_BBOX_HEIGHT					16

#define MARIO_STATE_IDLE								0
#define MARIO_STATE_MOVING_RIGHT						1
#define MARIO_STATE_MOVING_UP							2
#define MARIO_STATE_MOVING_LEFT							3
#define MARIO_STATE_MOVING_DOWN							4

#define MARIO_INTRO_LEVEL_SMALL							0

class CMarioWorldmap : public CGameObject
{
	float destination_x;
	float destination_y;
	bool isMoving;
	int move_right;
	int move_top;
	int move_left;
	int move_bottom;

	vector<LPNODE> nodes;

public:
	CMarioWorldmap();
	~CMarioWorldmap() {
		DebugOut(L"Unload CMarioOverworld \n");
	};
	void SetState(int state);
	bool GetIsMoving();
	int GetTypeCurrentNode();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void PushNode(CNode* node);
};