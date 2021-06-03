#include "PieceBrick.h"

void CPieceBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	y += vy * dt;
	x += vx * dt;
	if (state == STATE_PIECE_BRICK_DISPLAY && GetTickCount64() - timeDisplay > STATE_PIECE_BRICK_HIDEN_TIME)
	{
		SetState(STATE_PIECE_BRICK_HIDEN);
	}
}

void CPieceBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	return;
}

CPieceBrick::CPieceBrick()
{
	timeDisplay = 0;
}

void CPieceBrick::SetDisplay(float x, float y, float vx, float vy, DWORD t)
{
	SetPosition(x, y);
	this->vx = vx;
	this->vy = vy;
	timeDisplay = t;
}

void CPieceBrick::Render()
{
	if (state == STATE_PIECE_BRICK_HIDEN) return;
	animation_set->at(0)->Render(x, y);
}