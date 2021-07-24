#include "GoombaMini.h"
#include "Mario.h"
#include "PlayScence.h"

CGoombaMini::CGoombaMini()
{
	SetState(STATE_GOOMBA_MINI_HIDEN);
	timeDirection = (DWORD)0.0f;
}

void CGoombaMini::Render()
{
	if (state == STATE_GOOMBA_MINI_HIDEN) return;
	int ani = 10;
	animation_set->at(ani)->Render(x, y);
}
void CGoombaMini::handleCollisionMario()
{
	if (state != STATE_GOOMBA_MINI_MOVING_IN_MARIO) return;
	float marioX, marioY;
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	mario->GetPosition(marioX, marioY);
	if (x < marioX - 5) vx = 0.1f;
	if (x > marioX + round(mario->GetBBoxWidthMario())) vx = -0.1f;

	if (mario->getMarioDirection() == 1 ){
		if (round((marioX - x)) > round(mario->GetBBoxWidthMario()) /2 ) x = marioX;
	}
	if (mario->getMarioDirection() == -1) {
		if (x - marioX > mario->GetBBoxWidthMario()) x = marioX;
	}

	y = marioY + mario->GetBBoxHeightMario()/2;
	if (mario->GetMarioLevel() == 1) {
		y -= mario->GetBBoxHeightMario() / 2;
	}
}

void CGoombaMini::handleChangeDirection()
{
	if (state == STATE_GOOMBA_MINI_MOVING_IN_MARIO) return;
	if (GetTickCount64() - timeDirection > GOOMBA_MINI_TIME_DIRECTION) {
		timeDirection = (DWORD)GetTickCount64();
		vx = -vx;
	}
}

float CGoombaMini::getMarioLevelNumber()
{
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	return 0.0f;
}

void CGoombaMini::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == STATE_GOOMBA_MINI_HIDEN) return;
	
	handleChangeDirection();
	handleCollisionMario();

    CGameObject::Update(dt);
	// vy += 0.000001f * dt;
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
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CGoombaMini::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    if (state == STATE_GOOMBA_MINI_HIDEN) return;
	left = x;
	top = y;
	right = x + GOOMBA_MINI_BBOX_WIDTH;
	bottom = y + GOOMBA_MINI_BBOX_HEIGHT;
}

void CGoombaMini::SetState(int state)
{
    CGameObject::SetState(state);
	switch (state)
	{
	case STATE_GOOMBA_MINI_HIDEN:
		break;
	case STATE_GOOMBA_MINI_MOVING_IN_MARIO:
		if (!checkCollisionMario) {
			checkCollisionMario = true;
			// vy = -0.05f;
			vy = 0.0f;
			vx = -0.05f;
		}
		break;
	case STATE_GOOMBA_MINI_FLY_LEFT:
		vy = 0.05f;
		vx = -0.1f;
		break;
	case STATE_GOOMBA_MINI_FLY_RIGHT:
		vy = 0.05f;
		vx = 0.1f;
		break;
	default:
		break;
	}
}

void CGoombaMini::FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, 
	vector<LPCOLLISIONEVENT>& coEventsResult, 
	float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy)
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
		if (dynamic_cast<CMario*>(c->obj)){
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

