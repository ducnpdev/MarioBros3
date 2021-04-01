#include "Mushroom.h"
#include "Utils.h"
#include "Pipe.h"
#include "Road.h"

CMushroom::CMushroom(int _state)
{
	SetState(MUSHROOM_STATE_HIDEN);
}


void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// DebugOut(L"state musrom %d \n", state);
	if (state == MUSHROOM_STATE_HIDEN) return;
	CGameObject::Update(dt, coObjects);

	if (state == MUSHROOM_STATE_MOVING)
	{
		if (GetTickCount64() - timeStateMovingMushroom < 1000)
		{
			if (GetTickCount64() - timeStateMovingMushroom > 200)
			{
				// hiden_state = 0;
				vy = -MUSHROOM_SPEED_Y;
				y += dy;
			}
		}
		else SetState(MUSHROOM_STATE_NORMAL);
	}

	if (state == MUSHROOM_STATE_NORMAL) {
	//	DebugOut(L"MUSHROOM_STATE_NORMAL mushroom \n");
		vy += MUSHROOM_GRAVITY * dt;
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

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

			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

			x += min_tx * dx + nx * 0.2f;
			y += min_ty * dy + ny * 0.2f;

			if (ny != 0) vy = 0;
			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];
				if (dynamic_cast<CPipe*>(e->obj) || dynamic_cast<CBorderRoad*>(e->obj))
				{
					if (e->nx != 0 ) {
						vx = -1 * vx;
					}
				}
			}
		}

		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CMushroom::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MUSHROOM_STATE_MOVING:
		if (!stateMovingMushroom)
		{
			stateMovingMushroom = true;
			timeStateMovingMushroom = GetTickCount();
		}
		break;
	case MUSHROOM_STATE_HIDEN:
		vx = 0; vy = 0;
		break;
	case MUSHROOM_STATE_NORMAL:
		stateMovingMushroom = false;
		vx = 0.05f;
		break;
	default:
		break;
	}
}

void CMushroom::Render()
{
	 if (GetState() == MUSHROOM_STATE_HIDEN) return;
	int ani = MUSHROOM_RED_ANI;
	animation_set->at(ani)->Render(x, y);
}


void CMushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + MUSHROOM_BBOX_WIDTH;
	b = y + MUSHROOM_BBOX_HEIGHT;
}

void CMushroom::FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy)
{
	//min_tx = 1.0f;
	//min_ty = 1.0f;
	//int min_ix = -1;
	//int min_iy = -1;

	//nx = 0.0f;
	//ny = 0.0f;
	//coEventsResult.clear();

	//for (UINT i = 0; i < coEvents.size(); i++)
	//{
	//	LPCOLLISIONEVENT c = coEvents[i];
	//	if (dynamic_cast<CColorBrick*>(c->obj)) {}
	//	else if (dynamic_cast<CColorBrickTop*>(c->obj)) {
	//		if (c->ny < 0) {
	//			min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
	//		}
	//	}
	//	else if (dynamic_cast<CColorBrickTop*>(c->obj)) {
	//		if (c->ny < 0) {
	//			min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
	//		}
	//	}
	//	else {
	//		if (c->t < min_tx && c->nx != 0) {
	//			min_tx = c->t; nx = c->nx; min_ix = i; rdx = c->dx;
	//		}

	//		if (c->t < min_ty && c->ny != 0) {
	//			min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
	//		}
	//	}

	//}

	//if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	//if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
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

		if (c->t < min_ty  && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
		}
	}

	if (min_ix>=0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy>=0) coEventsResult.push_back(coEvents[min_iy]);
}
