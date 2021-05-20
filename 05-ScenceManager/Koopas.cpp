#include "Koopas.h"
#include "Road.h"
#include "Utils.h"
#include "Pipe.h"
#include "ColorBrick.h"
#include "Goomba.h"
#include "QuestionBrick.h"
#include "Leaf.h"
#include "Mario.h"



CKoopas::CKoopas()
{
	SetState(KOOPAS_STATE_WALKING_RIGHT);
	// SetState(KOOPAS_STATE_TORTOISESHELL_DOWN);

}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;
	bottom = y + KOOPAS_BBOX_HEIGHT;
	if (state == KOOPAS_STATE_TORTOISESHELL_DOWN || state == KOOPAS_STATE_REBORN) {
		bottom = y + 16;
	}
	else if (state == KOOPAS_STATE_SPIN_RIGHT || state == KOOPAS_STATE_SPIN_LEFT){
		bottom = y + 16;
	}
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	DebugOut(L"ssssssssssss \n");
	if (hidenStateKoopas) return;
	CGameObject::Update(dt, coObjects);
	if (state != KOOPAS_STATE_TAKEN)  vy += KOOPA_GRAVITY * dt;
	//vx = 0;
	// vy = 0;

	// koopa chuẩn bị hồi sinh
	if (stateKoopaTortoiSeShell && GetTickCount() - timeStateTorToiSeShell > 5000 && GetTickCount() - timeStateTorToiSeShell < 7000)  {
		SetState(KOOPAS_STATE_REBORN);
	}
	// hồi sinh qua trạng thái ban đầu
	else if(stateKoopaTortoiSeShell && GetTickCount() - timeStateTorToiSeShell > 7000) {
		SetPosition(x, y - 10);
		SetState(KOOPAS_STATE_WALKING_RIGHT);
	}

	/*if (vx < 0 && x < 500) {
		x = 500; vx = -1 * vx;
	}
	if (vx > 0 && x > 590) {
		x = 590; vx = -1 * vx;
	}*/


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
		

		for (UINT i = 0; i < coEventsResult.size(); i++) {
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (!dynamic_cast<CMario*>(e->obj) && e->ny != 0)
			{
				prePosY = y;
			}

			if (state == KOOPAS_STATE_WALKING_LEFT || state == KOOPAS_STATE_WALKING_RIGHT)
			{
				if (dynamic_cast<CPipe*>(e->obj) || dynamic_cast<CBorderRoad*>(e->obj))
				{
					if (e->nx > 0) {

						SetState(KOOPAS_STATE_WALKING_RIGHT);
					}
					else if (e->nx < 0) SetState(KOOPAS_STATE_WALKING_LEFT);
				}
			}
			if (state == KOOPAS_STATE_SPIN_LEFT)
			{
				if (dynamic_cast<CPipe*>(e->obj) || dynamic_cast<CQuestionBrick*>(e->obj) || dynamic_cast<CBorderRoad*>(e->obj))
				{
					SetState(KOOPAS_STATE_SPIN_RIGHT);
				}
				
			}
			else if (state == KOOPAS_STATE_SPIN_RIGHT)
			{
				if (dynamic_cast<CPipe*>(e->obj) || dynamic_cast<CBorderRoad*>(e->obj))
				{
					SetState(KOOPAS_STATE_SPIN_LEFT);
				}
				if (dynamic_cast<CQuestionBrick*>(e->obj)) {
					CQuestionBrick* questionBrick = dynamic_cast<CQuestionBrick*>(e->obj);
					if (questionBrick->GetState() != QUESTION_BRICK_ANI_CRETE) {
						questionBrick->SetItemWhenCollision(LEAF_STATE_FLY);
						questionBrick->SetState(QUESTION_BRICK_ANI_CRETE);
					}
					SetState(KOOPAS_STATE_SPIN_LEFT);
				}
				
				
			}
		
		}
	}
	RedirectY();
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	
}



void CKoopas::Render()
{
	if (hidenStateKoopas) return;
	// koopas color red
	int ani = KOOPAS_ANI_WALKING_LEFT;
	if (state == KOOPAS_STATE_TORTOISESHELL_DOWN) {
		ani = KOOPAS_ANI_RED_TORTOISESHELL_DOWN;
	}
	else if (state == KOOPAS_STATE_SPIN_LEFT || state == KOOPAS_STATE_SPIN_RIGHT)
		ani = KOOPAS_ANI_RED_SPIN_DOWN;
	else if (state == KOOPAS_STATE_REBORN) {
		ani = 16;
	}
	else if (state == KOOPAS_STATE_WALKING_LEFT) {
		ani = 1;
	}
	else if (state == KOOPAS_STATE_TAKEN) {
		ani = 2;
	}

	// koopas color green


	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}


void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	
	case KOOPAS_STATE_TORTOISESHELL_DOWN:
		vx = 0;
		vy = 0;
		timeStateTorToiSeShell = GetTickCount();
		stateKoopaTortoiSeShell = true;
		break;
	case KOOPAS_STATE_DIE:
		y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		stateKoopaTortoiSeShell = false;

		break;
	case KOOPAS_STATE_WALKING_RIGHT:
		stateKoopaTortoiSeShell = false;
		vx = KOOPAS_WALKING_SPEED;
		nx = 1;
		break;
	case KOOPAS_STATE_WALKING_LEFT:
		nx = -1;
		vx = -KOOPAS_WALKING_SPEED;
		stateKoopaTortoiSeShell = false;
		break;
	case KOOPAS_STATE_SPIN_RIGHT:
		stateKoopaTortoiSeShell = false;
		vx = KOOPAS_SPINNING_SPEED;
	///	vx = 0.075f;
		break;
	case KOOPAS_STATE_SPIN_LEFT:
		stateKoopaTortoiSeShell = false;
		vx = -KOOPAS_SPINNING_SPEED;
		break;
	case KOOPAS_STATE_HIDEN:
		stateKoopaTortoiSeShell = false;
		hidenStateKoopas = true;
		break;
	case KOOPAS_STATE_REBORN:
		vx = 0;
		vy = 0;
		break;
	case KOOPAS_STATE_TAKEN:
		vx = 0;
		vy = 0;
		stateKoopaTortoiSeShell = false;
		break;
	}

}



void CKoopas::FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy)
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
		if (dynamic_cast<CGoomba*>(c->obj)) {
			continue;
		}
		else if (dynamic_cast<CColorBrick*>(c->obj)) {
			if (c->ny < 0) {
				min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
			}
			// continue;
		}
		else if (dynamic_cast<CRoad*>(c->obj)) {	
			if (c->ny < 0) {
				min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
			}
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

void CKoopas::RedirectY()
{
	if (state == KOOPAS_STATE_WALKING_LEFT || state == KOOPAS_STATE_WALKING_RIGHT)
	{
		if (abs(prePosY - y) > 1 && prePosY != 0) {
			if (vx > 0) {
				SetPosition(x - 5, prePosY);
				SetState(KOOPAS_STATE_WALKING_LEFT);
				prePosY = y;
			}
			else {
				SetPosition(x + 5, prePosY);
				SetState(KOOPAS_STATE_WALKING_RIGHT);
				prePosY = y;
			}
		}

	}

}



