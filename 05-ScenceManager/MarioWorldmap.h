#pragma once
#include "GameObject.h"
#include "Game.h"

class CMarioWorldmap : public CGameObject
{
	float destination_x;
	float destination_y;
	bool isMoving;
	int move_right;
	int move_top;
	int move_left;
	int move_bottom;
public:
	CMarioWorldmap();
	void SetState(int state);
	bool GetIsMoving();
	int GetTypeCurrentNode();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

};