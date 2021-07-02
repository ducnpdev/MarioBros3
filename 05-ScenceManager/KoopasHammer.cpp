#include "KoopasHammer.h"
#include "Utils.h"
#include "ColorBrick.h"

CKoopasHammer::CKoopasHammer(int type)
{
	SetState(KOOPAS_HAMMER_STATE_WALKING_LEFT);
}

void CKoopasHammer::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (hiden) return;
	CGameObject::Update(dt, coObjects);
	vy += KOOPA_HAMMER_GRAVITY * dt;
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
		x += min_tx * dx + nx * 0.35f; // 
		y += min_ty * dy + ny * 0.35f;
		for (UINT i = 0; i < coEventsResult.size(); i++) {
			LPCOLLISIONEVENT e = coEventsResult[i];
			DebugOut(L"update koopas hammer \n");
		}
	}
}

void CKoopasHammer::Render()
{
	if (hiden) return;
	int ani = KOOPAS_HAMMER_ANI_WALKING_RIGHT;
	if (state == KOOPAS_HAMMER_STATE_WALKING_LEFT ) {
		ani = KOOPAS_HAMMER_ANI_WALKING_LEFT;
	}
	animation_set->at(1)->Render(x, y); 	 
	RenderBoundingBox();
}

void CKoopasHammer::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_HAMMER_STATE_HIDEN:
		vx = 0;
		vy = 0;
		hiden = true;
		break;
	case KOOPAS_HAMMER_STATE_WALKING_RIGHT:
		vx = KOOPAS_HAMMER_WALKING_SPEED;
		nx = 1;
		break;
	case KOOPAS_HAMMER_STATE_WALKING_LEFT:
		nx = -1;
		vx = -KOOPAS_HAMMER_WALKING_SPEED;
		break;
	}
}

void CKoopasHammer::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_HAMMER_BBOX_WIDTH;
	bottom = y + KOOPAS_HAMMER_BBOX_HEIGHT;
}


void CKoopasHammer::FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, 
	vector<LPCOLLISIONEVENT>& coEventsResult, 
	float& min_tx, float& min_ty, float& nx, 
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
		if (dynamic_cast<CColorBrick*>(c->obj)) {
			if (c->ny < 0) {
				min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
			}
			continue;
		}
		else {
			if (c->t < min_tx && c->nx != 0) {
				min_tx = c->t; nx = c->nx; min_ix = i; rdx = c->dx;
			}
			if (c->t < min_ty && c->ny != 0) {
				min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
			}
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}
