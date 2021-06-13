#include "Turtle.h"

CTurtle::CTurtle()
{
	SetState(TURTLE_STATE_MOVING_LEFT);
	isWalkingRight = false;
}

void CTurtle::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt);
	x += vx * dt;
	if (isWalkingRight && x < DESTINATION_RIGHT) {
		SetState(TURTLE_STATE_MOVING_RIGHT);
	}
	else SetState(TURTLE_STATE_MOVING_LEFT);

	if (!isWalkingRight && x > DESTINATION_LEFT) {
		SetState(TURTLE_STATE_MOVING_LEFT);
	}
	else SetState(TURTLE_STATE_MOVING_RIGHT);
}

void CTurtle::Render()
{
	int ani = 0;
	if (!isWalkingRight) ani = TURTLE_ANI_MOVING_LEFT;
	else if (isWalkingRight) ani = TURTLE_ANI_MOVING_RIGHT;
	animation_set->at(ani)->Render(x, y);
}

void CTurtle::SetState(int s)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case TURTLE_STATE_MOVING_LEFT:
		isWalkingRight = false;
		vx = -TURTLE_WALKING_SPEED;
		break;
	case TURTLE_STATE_MOVING_RIGHT:
		isWalkingRight = true;
		vx = TURTLE_WALKING_SPEED;
		break;
	}
}
