#include "FireBullet.h"
#include <algorithm>
#include "Game.h"
#include "ColorBrick.h"
#include "Road.h"
#include "FirePiranhaPlant.h"
#include "PiranhaPlant.h"

CFireBullet::CFireBullet()
{
	destroyStart = 0;
	shootStart = 0;
	state = FIRE_BULLET_TRANSPARENT_STATE;
	vx = 0;
}

void CFireBullet::Render()
{
	int ani = FIRE_BULLET_ANI_NORMAL;
	if (state == FIRE_BULLET_TRANSPARENT_STATE) {
		return;
	}
	else if (state == FIRE_BULLET_SHOOTED_RIGHT_STATE || 
		state == FIRE_BULLET_SHOOTED_LEFT_STATE) ani = FIRE_BULLET_ANI_NORMAL;
	
	else if (state == FIRE_BULLET_DESTROY_STATE)
	{
		ani = FIRE_BULLET_ANI_DESTROY;
	}
	animation_set->at(ani)->Render(x, y);
}



void CFireBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (state == FIRE_BULLET_SHOOTED_RIGHT_STATE || state == FIRE_BULLET_SHOOTED_LEFT_STATE)
	{
		vy += FIRE_BULLET_GRAVITY * dt;
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
			x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.4f;

			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];
				if (e->ny != 0) vy = -FIRE_BULLET_DEFLECT_SPEED;
				else if (e->nx != 0 && (-y - FIREBULLET_BBOX_HEIGHT + e->obj->y < 0))
				{
					destroyStart = (DWORD)GetTickCount64();
					SetState(FIRE_BULLET_DESTROY_STATE);
				}
				if (dynamic_cast<CFirePiranhaPlant*>(e->obj))
				{
					if (nx != 0 || ny != 0)
					{
						CFirePiranhaPlant* plant = dynamic_cast<CFirePiranhaPlant*>(e->obj);
						destroyStart = (DWORD)GetTickCount64();
						plant->SetState(FIREPIRANHAPLANT_STATE_DESTROY);
						SetState(FIRE_BULLET_DESTROY_STATE);
					}
				}
				if (dynamic_cast<CPiranhaPlant*>(e->obj))
				{
					if (nx != 0 || ny != 0)
					{
						CPiranhaPlant* plant = dynamic_cast<CPiranhaPlant*>(e->obj);
						destroyStart = (DWORD)GetTickCount64();
						plant->SetState(PIRANHAPLANT_STATE_DESTROY);
						SetState(FIRE_BULLET_DESTROY_STATE);
					}
				}
			}

		}
		if (GetTickCount64() - shootStart > FIRE_BULLET_SHOOT_TIME)
		{
			SetState(FIRE_BULLET_TRANSPARENT_STATE);
		}
	}
	else
	{
		shootStart = (DWORD)GetTickCount64();
	}

	if (state == FIRE_BULLET_DESTROY_STATE)
	{
		if (GetTickCount64() - destroyStart < FIRE_BULLET_DESTROY_TIME)
			SetState(FIRE_BULLET_DESTROY_STATE);
		else SetState(FIRE_BULLET_TRANSPARENT_STATE);
	}

}

void CFireBullet::GetBoundingBox(float& l, float& t, float& r, float& b)
{
		l = x;
		t = y;
	if (state == FIRE_BULLET_DESTROY_STATE)
	{
		r = 0;
		b = 0;
	}
	if (state == FIRE_BULLET_SHOOTED_RIGHT_STATE || state == FIRE_BULLET_SHOOTED_LEFT_STATE)
	{
		r = x + FIREBULLET_BBOX_WIDTH;
		b = y + FIREBULLET_BBOX_HEIGHT;
	}

}

void CFireBullet::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case FIRE_BULLET_DESTROY_STATE:
		vx = 0;
		vy = 0;
		break;
	case FIRE_BULLET_SHOOTED_RIGHT_STATE:
		vx = FIRE_BULLET_FLYING_SPEECH;
		break;
	case FIRE_BULLET_SHOOTED_LEFT_STATE:
		vx = -FIRE_BULLET_FLYING_SPEECH;
		break;
	case FIRE_BULLET_TRANSPARENT_STATE:
		vx = 0;
		vy = 0;
		break;
	default:
		break;
	}
}

void CFireBullet::FilterCollision(
	vector<LPCOLLISIONEVENT>& coEvents,
	vector<LPCOLLISIONEVENT>& coEventsResult,
	float& min_tx, float& min_ty,
	float& nx, float& ny, float& rdx, float& rdy)
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
		if (dynamic_cast<CColorBrick*>(c->obj)) {}
		/*else if (dynamic_cast<CTransObject*>(c->obj)) {}
		else if (dynamic_cast<CColorBrickTop*>(c->obj)) {
			if (c->ny < 0) {
				min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
			}
		}
		else if (dynamic_cast<CColorBrickTop*>(c->obj)) {
			if (c->ny < 0) {
				min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
			}
		}*/
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