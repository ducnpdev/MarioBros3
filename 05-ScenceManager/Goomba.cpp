#include "Goomba.h"
#include "Utils.h"

CGoomba::CGoomba(int t)
{
	SetTypeGoomba(t);
	SetState(GOOMBA_STATE_WALKING);
	
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + GOOMBA_BBOX_WIDTH;

	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else 	
		bottom = y + GOOMBA_BBOX_HEIGHT;
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	
	CGameObject::Update(dt, coObjects);
	vy += 0.0005f * dt;
	if (type == 0) {
		x = 100;
		y = 350;
	}

	if (type == 1) {
		x = 200;
		y = 350;
	}
	
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		 x += dx;
		y += dy;
	//	DebugOut(L"xxx: %f \n", x);
		//DebugOut(L"yyy: %f \n", y);

	}
}

void CGoomba::Render()
{
	int ani = GOOMBA_ANI_WALKING;
	if (state == GOOMBA_STATE_DIE) {
		ani = GOOMBA_ANI_DIE;
	}

	animation_set->at(5)->Render(x,y);
	RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
			vx = 0;
			vy = 0;
			isDead = true;
			break;
		case GOOMBA_STATE_WALKING: 
			vx = -GOOMBA_WALKING_SPEED;
	}
}
