#pragma once
#include "GameObject.h"

#define TURTLE_WALKING_SPEED			0.02f

#define TURTLE_STATE_IDLE				0
#define TURTLE_STATE_MOVING_LEFT		1
#define TURTLE_STATE_MOVING_RIGHT		2

#define TURTLE_ANI_IDLE					2
#define TURTLE_ANI_MOVING_LEFT			0
#define TURTLE_ANI_MOVING_RIGHT			1

#define DESTINATION_RIGHT				190
#define DESTINATION_LEFT				175

class CTurtle : public CGameObject
{
	bool isWalkingRight;
public:
	CTurtle();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) { return; }
	void SetState(int s);
};

