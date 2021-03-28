#include "Goomba.h"
#include "Utils.h"
#include "Road.h"

CGoomba::CGoomba(int typeColor)
{
	setColorGoomba(typeColor);
	SetState(GOOMBA_STATE_WALKING);
}



void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	// DebugOut(L"class goomba \n");
	if (GetState() != GOOMBA_STATE_DIE && !isVisibleGoomba) {
		vy += GOOMBA_GRAVITY * dt;
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();
		// if (state != GOOMBA_STATE_DIE && state != GOOMBA_STATE_DEFLECT)
		CalcPotentialCollisions(coObjects, coEvents);

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
			CGoomba::FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
			x += min_tx * dx + nx * 0.1f;
			y += min_ty * dy + ny * 0.1f;
			if (ny != 0) vy = 0;
			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];
				if (dynamic_cast<CRoad*>(e->obj))
				{
					// DebugOut(L"goomba collision road \n");
				}
			}
		}
	}

	if (GetState() == GOOMBA_STATE_DIE && !isVisibleGoomba) {
		timeVisibleGoomba += dt;
		if (timeVisibleGoomba >= dt * 8)
		{
			SetPosition(-50, 50);
			isVisibleGoomba = true;
		}
	}

}

void CGoomba::Render()
{
	int ani = GOOMBA_ANI_YELLOW_WALKING;
	if (typeColorGoomba == GOOMBA_YELLOW_COLOR) {
		if (state == GOOMBA_STATE_DIE) {
			ani = GOOMBA_ANI_YELLOW_DIE;
		}
		// 
		else if (state == GOOMBA_STATE_DEFLECT) {
			ani = GOOMBA_ANI_DEFLECT;
		}
		// GOOMBA_STATE_IDLE sau khi mario collision 
		else if (state == GOOMBA_STATE_IDLE) {
			ani = GOOMBA_ANI_YELLOW_IDLE;
		}
	}
	animation_set->at(ani)->Render(x, y);
	
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_WALKING:
		DebugOut(L"goomba walking hihi \n");
		vx = GOOMBA_WALKING_SPEED;
		break;
	case GOOMBA_STATE_DIE:
		break;
	case GOOMBA_STATE_DEFLECT:
		vy = -GOONBA_JUMP_DEFLECT_SPEED;
		break;	
	case GOOMBA_STATE_IDLE:
		vx = 0;
		break;	
	default: 
		break;
	}

}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	// if (state == GOOMBA_STATE_DEFLECT) return;
	left = x;
	top = y;
	right = x + GOOMBA_BBOX_WIDTH;
	bottom = y + GOOMBA_BBOX_HEIGHT;
	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
}

void CGoomba::FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy)
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

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; nx = c->nx; min_ix = i; rdx = c->dx;
		}

		if (c->t < min_ty && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}
