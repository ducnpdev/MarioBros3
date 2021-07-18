#include "Music.h"
#include <algorithm>
#include "Game.h"
#include "ColorBrick.h"
#include "Road.h"
#include "FirePiranhaPlant.h"
#include "PiranhaPlant.h"
#include "Koopas.h"
#include "Goomba.h"

CMusic::CMusic(int _type,int _status, float _originY)
{
	SetState(_status);
	type = _type;
	originY = _originY;
}

void CMusic::Render()
{
	if (state == MUSIC_HIDEN_STATE) return;
	int ani = MUSIC_ANI_WHITE;
	if (type == 1) ani = MUSIC_ANI_RED;
	animation_set->at(ani)->Render(x, y);
}

void CMusic::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
//	y += dy;
	if (type == MUSIC_TYPE_WHITE) {
		if (state == MUSIC_DOWN_STATE) {
			y += dy;
			if (y - originY > MUSIC_RANGE_POSITION_Y) {
				SetState(MUSIC_UP_STATE);
			
			}
		}
	}
	if (type == MUSIC_TYPE_RED) {

	}
	if (state == MUSIC_UP_STATE) {
		y += dy;
		if (y < originY) y = originY;
		if (y == originY) {
			SetState(MUSIC_APPEAR_STATE);
		}
	}
}

void CMusic::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + MUSIC_BBOX_WIDTH;
	b = y + MUSIC_BBOX_HEIGHT;
	if (state == MUSIC_HIDEN_STATE)
	{
		r = 0;
		b = 0;
	}
}

void CMusic::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MUSIC_DESTROY_STATE:
		vx = 0;
		vy = 0;
		break;
	case MUSIC_HIDEN_STATE:
		vx = 0;
		vy = 0;
		break;
	case MUSIC_APPEAR_STATE:
		vx = 0;
		vy = 0;
		break;
	case MUSIC_DOWN_STATE:
		vx = 0;
		vy = 0.1f;
		break;
	case MUSIC_UP_STATE:
		vx = 0;
		vy = -0.125f;
		break;
	default:
		break;
	}
}

void CMusic::FilterCollision(
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