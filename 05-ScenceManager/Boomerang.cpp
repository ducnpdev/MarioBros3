#include "Boomerang.h"
#include "Mario.h"
#include "BoomerangConfig.h"

CBoomerang::CBoomerang()
{
	shootStartBoomerangTime = 0;
	isBoomerangFly = false;
	state = STATE_BOOMERANG_HIDEN;
	vx = 0;
}

void CBoomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
//	DebugOut(L"111111111 \n");
	CGameObject::Update(dt);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(colliable_objects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx; 
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

	}
	// DebugOut(L"2222222 \n");

	if (!isBoomerangFly) return;
//	DebugOut(L"333333 \n");

	if (state == STATE_BOOMERANG_FLYING_LEFT) {
		if (GetTickCount64() - shootStartBoomerangTime < STATE_BOOMERANG_FLYING_ONE_TIME) {
			// fly lef and top
			vx = -BOOMERANG_FLYING_SPEECH;
			vy = -BOOMERANG_GRAVITY;
		}
		else if (GetTickCount64() - shootStartBoomerangTime < STATE_BOOMERANG_FLYING_TWO_TIME)
		{
			// fly sub left and top
			vx = -BOOMERANG_FLYING_SPEECH + BOOMERANG_SPEED_X_PLUS;
			vy = BOOMERANG_GRAVITY + BOOMERANG_SPEED_Y_PLUS;
		}
		else if (GetTickCount64() - shootStartBoomerangTime < STATE_BOOMERANG_FLYING_THREE_TIME)
		{
			vx = BOOMERANG_FLYING_SPEECH;
			vy = 0;
		}
		else
		{
			SetState(STATE_BOOMERANG_HIDEN);
		}
	}
	else {
		if (GetTickCount64() - shootStartBoomerangTime < STATE_BOOMERANG_FLYING_ONE_TIME) {
			vx = BOOMERANG_FLYING_SPEECH;
			vy = -BOOMERANG_GRAVITY;
		}
		else if (GetTickCount64() - shootStartBoomerangTime < STATE_BOOMERANG_FLYING_TWO_TIME)
		{
			vx = BOOMERANG_FLYING_SPEECH - BOOMERANG_SPEED_X_PLUS;
			vy = BOOMERANG_GRAVITY + BOOMERANG_SPEED_Y_PLUS;
		}
		else if (GetTickCount64() - shootStartBoomerangTime < STATE_BOOMERANG_FLYING_THREE_TIME)
		{
			vx = -BOOMERANG_FLYING_SPEECH;
			vy = 0;
		}
		else
		{
			SetState(STATE_BOOMERANG_HIDEN);
		}
	}
}

void CBoomerang::Render()
{
	if (state == STATE_BOOMERANG_HIDEN) return;
	int ani = ANI_BOOMERANG_FLYING;
	animation_set->at(ani)->Render(x, y);
	// RenderBoundingBox();
}

void CBoomerang::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case STATE_BOOMERANG_FLYING_LEFT:
		isBoomerangFly = true;
		break;
	case STATE_BOOMERANG_FLYING_RIGHT:
		isBoomerangFly = true;
		break;
	default:
		break;
	}
}



void CBoomerang::FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, 
	vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, 
	float& ny, float& rdx, float& rdy)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;
	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];
		if (dynamic_cast<CMario*>(c->obj)) {

			if (c->t < min_tx && c->nx != 0) {
				min_tx = c->t; nx = c->nx; min_ix = i; rdx = c->dx;
			}
			if (c->t < min_ty && c->ny != 0) {
				min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
			}
			nx = 0;
		}
	}
	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}
void CBoomerang::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == STATE_BOOMERANG_HIDEN) return;
	
	left = x;
	top = y;
	right = x + BOOMERANG_BBOX_WIDTH;
	bottom = y + BOOMERANG_BBOX_HEIGHT;
}