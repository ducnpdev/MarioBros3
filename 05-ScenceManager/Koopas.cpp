﻿#include "Koopas.h"
#include "Road.h"
#include "Utils.h"
#include "Pipe.h"
#include "ColorBrick.h"
#include "Goomba.h"
#include "QuestionBrick.h"
#include "Leaf.h"
#include "Mario.h"
#include "PlayScence.h"
#include "ItemCoin.h"
#include "WoodBlock.h"
#include "BoomerangBros.h"
#include "Portal.h"
#include "Music.h"

CKoopas::CKoopas(int type)
{
	SetState(KOOPAS_STATE_WALKING_LEFT);
	SetTypeKoopa(type);
	timeNotEffect;
	if (typeKoopa == PARAKOOPA_COLOR_GREEN)	nx = -1;
	
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (isKoopaDie || state == KOOPAS_STATE_HIDEN) return;
	
	if (state == KOOPAS_STATE_TORTOISESHELL_DOWN || state == KOOPAS_STATE_TORTOISESHELL_UP || state == KOOPAS_STATE_REBORN) {
		right = x + KOOPAS_BBOX_WIDTH_MEDIUM;
		bottom = y + KOOPAS_BBOX_HEIGHT_REBORN;
		/*if (state == KOOPAS_STATE_SPIN_RIGHT || state == KOOPAS_STATE_SPIN_LEFT) {
			bottom = y + KOOPAS_BBOX_HEIGHT_SPIN;
		}*/
	}
	else if (state == KOOPAS_STATE_SPIN_RIGHT || state == KOOPAS_STATE_SPIN_LEFT){
		right = x + KOOPAS_BBOX_WIDTH_MIN;
		bottom = y + KOOPAS_BBOX_HEIGHT_SPIN;
		/*if (state == KOOPAS_STATE_TORTOISESHELL_DOWN ) {
			bottom = y + KOOPAS_BBOX_HEIGHT_REBORN+1;
		}*/
	}
	else {
		right = x + KOOPAS_BBOX_WIDTH_MEDIUM;
		bottom = y + KOOPAS_BBOX_HEIGHT;
	}
}

float CKoopas::GetHeightKoopas() {
	float height = KOOPAS_BBOX_HEIGHT;
	if (state == KOOPAS_STATE_SPIN_RIGHT || state == KOOPAS_STATE_SPIN_LEFT) {
		height = KOOPAS_BBOX_HEIGHT_SPIN;
	}
	if (state == KOOPAS_STATE_TORTOISESHELL_DOWN || state == KOOPAS_STATE_TORTOISESHELL_UP || state == KOOPAS_STATE_REBORN) {
		height = KOOPAS_BBOX_HEIGHT_REBORN;
	}
	return height - 5.0f;;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
//	DebugOut(L"koopas state %d %f %f \n", state, x, y);
//	DebugOut(L"koopas vx vy %f %f \n",  vx, vy);
	if (hidenStateKoopas) return;
	CGameObject::Update(dt, coObjects);
	if (state != KOOPAS_STATE_TAKEN) {
		if (state == KOOPAS_STATE_SPIN_LEFT || state == KOOPAS_STATE_SPIN_RIGHT) {
			vy += KOOPA_GRAVITY_BIG * dt;
		}
		else {
			vy += KOOPA_GRAVITY * dt;
		}
	//	vy += KOOPA_GRAVITY * dt;

	}
	handlerDeflect();
	handleReborn();

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

		if (ny != 0) vy = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++) {
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (!dynamic_cast<CMario*>(e->obj) && e->ny != 0  )
			{
				prePosY = y;
			}

			if (dynamic_cast<CColorBrick*>(e->obj)) {
				if (e->nx != 0) {
				}
				else {
				}
			}

			if (dynamic_cast<CRoad*>(e->obj)) {
				if (e->nx != 0) {
				}
				else if (e->ny < 0){
				}
			}

			if (dynamic_cast<CKoopas*>(e->obj)) {
				if (e->nx != 0) {
					SetState(KOOPAS_STATE_DIE);
				}
			}

			if (dynamic_cast<CGoomba*>(e->obj)) {
			}

			if (dynamic_cast<CMusic*>(e->obj)) {
			}

			if (dynamic_cast<CBoomerangBro*>(e->obj))
			{
				CBoomerangBro* boomerangBro = dynamic_cast<CBoomerangBro*>(e->obj);
				if (boomerangBro->GetState() != STATE_BOOMERANGBRO_DIE)
				{
					CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
					mario->DisplayListScore(MARIO_SCORE_100, boomerangBro->x, boomerangBro->y, (DWORD)GetTickCount64());
					boomerangBro->SetState(STATE_BOOMERANGBRO_DIE);
					boomerangBro->SetBoomerangTimeDead((DWORD)GetTickCount64());

				}
			}
			if (dynamic_cast<CBrick*>(e->obj)) {
				CollisionWithBrick(e);
			}

			if (typeKoopa == PARAKOOPA_COLOR_GREEN)
			{
				if (e->ny < 0) { 
					if (GetTickCount64() - timeFly > KOOPAS_TIME_FLY) {
						vy = -KOOPAS_FLY_INTERVAL;
						timeFly = (DWORD)GetTickCount64();
					}
				}
			}
			
			if (typeKoopa != PARAKOOPA_COLOR_GREEN) {
				if (state == KOOPAS_STATE_WALKING_LEFT || state == KOOPAS_STATE_WALKING_RIGHT)
				{
					if (dynamic_cast<CPipe*>(e->obj) || dynamic_cast<CBorderRoad*>(e->obj)
						|| dynamic_cast<CWoodBlock*>(e->obj)
						 || dynamic_cast<CQuestionBrick*>(e->obj)
						)
					{
						if (e->nx > 0) {
							SetState(KOOPAS_STATE_WALKING_RIGHT);
						}
						else if (e->nx < 0) {
							SetState(KOOPAS_STATE_WALKING_LEFT);
						}
					}
				}

				if (state == KOOPAS_STATE_SPIN_LEFT)
				{
					if (dynamic_cast<CPipe*>(e->obj) || 
						dynamic_cast<CQuestionBrick*>(e->obj) || 
						dynamic_cast<CBorderRoad*>(e->obj) ||
						// dynamic_cast<CBrick*>(e->obj) ||
						dynamic_cast<CWoodBlock*>(e->obj) ||
						dynamic_cast<CMusic*>(e->obj))  
					{
					//	DebugOut(L"CMusic %f \n", vy);
						// colision right
						if (e->nx > 0) {
					//		DebugOut(L"CMusic  11111 \n");

							//if (dynamic_cast<CBrick*>(e->obj))
							//{
							//	CBrick* brick = dynamic_cast<CBrick*>(e->obj);
							//	// Bắt đầu va chạm
							//	if (brick->y - (y + 16) < 0)
							//	{
							//		((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->CreatePieceBrick(x, y, (DWORD)GetTickCount64());
							//		brick->SetState(BRICK_STATE_HIDEN);
							//	}

							//}
							SetState(KOOPAS_STATE_SPIN_RIGHT);
						}
					}
				}
				else if (state == KOOPAS_STATE_SPIN_RIGHT)
				{
					if (e->nx < 0) {
						if (  dynamic_cast<CPipe*>(e->obj) ||
						//	dynamic_cast<CBrick*>(e->obj) ||
							dynamic_cast<CBorderRoad*>(e->obj) ||
							dynamic_cast<CWoodBlock*>(e->obj) ||
							dynamic_cast<CMusic*>(e->obj))
						{
							////if (dynamic_cast<CBrick*>(e->obj))
							////{
							////	CBrick* brick = dynamic_cast<CBrick*>(e->obj);
							////	brick->SetState(BRICK_STATE_HIDEN);
							////	((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->CreatePieceBrick(x, y, (DWORD)GetTickCount64());
							////	 // SetState(KOOPAS_STATE_SPIN_LEFT);
							////}

							SetState(KOOPAS_STATE_SPIN_LEFT);
						}
						if (dynamic_cast<CQuestionBrick*>(e->obj)) {
							CQuestionBrick* questionBrick = dynamic_cast<CQuestionBrick*>(e->obj);
							if (questionBrick->GetState() != QUESTION_BRICK_STATE_CRETE) {
								questionBrick->SetItemWhenCollision(LEAF_STATE_FLY);
								questionBrick->SetState(QUESTION_BRICK_STATE_CRETE);
							}
						//	SetState(KOOPAS_STATE_SPIN_LEFT);
						}
						SetState(KOOPAS_STATE_SPIN_LEFT);

					}
				}
			}
		}	
	}

	if (typeKoopa == KOOPA_COLOR_RED) RedirectY();
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CKoopas::Render()
{
	if (hidenStateKoopas || state == KOOPAS_STATE_HIDEN) return;

	switch (typeKoopa)
	{
	case KOOPA_COLOR_RED:
		ani = RenderKoopaRed();
		break;
	case PARAKOOPA_COLOR_GREEN:
		ani = RenderParakoopaGreen();
		break;
	case KOOPA_GREEN_FORM:
		ani = RenderKoopaGreen();
	default:
		ani = RenderKoopaGreen();
		break;
	}
	
	animation_set->at(ani)->Render(x, y);
	// RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	if (state == KOOPAS_STATE_TORTOISESHELL_DOWN) {
		y = y - APPEND_KOOPAS_STATE_SPIN_TO_TORTOISESHELL;
	}
	CGameObject::SetState(state);
	switch (state)
	{
	
	case KOOPAS_STATE_TORTOISESHELL_DOWN:
		if (state == KOOPAS_STATE_SPIN_RIGHT || state == KOOPAS_STATE_SPIN_LEFT) {
			y = y - 20;
		}
		vx = 0.0f;
		vy = 0.0f;
		if (!stateKoopaTortoiSeShell) timeStateTorToiSeShell = (DWORD)GetTickCount64();
		stateKoopaTortoiSeShell = true;
		isDown = true;
		break;
	case KOOPAS_STATE_TORTOISESHELL_UP:
		if (!stateKoopaTortoiSeShell) {
			timeStateTorToiSeShell = (DWORD)GetTickCount64();
		}
		stateKoopaTortoiSeShell = true;
		isDown = false;
		vx = 0.0f;
		vy = 0.0f;
		break;
	case KOOPAS_STATE_DIE:
		y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		stateKoopaTortoiSeShell = false;
		isKoopaDie = true;
		break;
	case KOOPAS_STATE_WALKING_RIGHT:
		stateKoopaTortoiSeShell = false;
		vx = KOOPAS_WALKING_SPEED;
		nx = 1;
		if (GetTickCount64() - timeStateTorToiSeShell > KOOPA_TIME_REBORN_END &&
			GetTickCount64() - timeStateTorToiSeShell < KOOPA_TIME_BORN_DONE) y = y - KOOPA_SUB_Y_POSITION;
		break;
	case KOOPAS_STATE_WALKING_LEFT:
		nx = -1;
		vx = -KOOPAS_WALKING_SPEED;
		stateKoopaTortoiSeShell = false;
		break;
	case KOOPAS_STATE_SPIN_RIGHT:
		stateKoopaTortoiSeShell = false;
		vx = KOOPAS_SPINNING_SPEED;
	//	vx = 0.025f;
	///	vx = 0.075f;
		break;
	case KOOPAS_STATE_SPIN_LEFT:
		stateKoopaTortoiSeShell = false;
		 vx = -KOOPAS_SPINNING_SPEED;
		// vx = -0.025f;
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
		// stateKoopaTortoiSeShell = false;	
		break;
	
	}

}

int CKoopas::RenderKoopaRed()
{
	int ani = KOOPAS_ANI_RED_WALKING_LEFT;
	if (state == KOOPAS_STATE_TORTOISESHELL_DOWN) {
		ani = KOOPAS_ANI_RED_TORTOISESHELL_DOWN;
	}
	else if (state == KOOPAS_STATE_TORTOISESHELL_UP) {
		ani = KOOPAS_ANI_RED_TORTOISESHELL_UP;
	}
	else if (state == KOOPAS_STATE_SPIN_LEFT || state == KOOPAS_STATE_SPIN_RIGHT)
		ani = KOOPAS_ANI_RED_SPIN_DOWN;
	else if (state == KOOPAS_STATE_REBORN) {
		ani = KOOPA_ANI_RED_REBORN_DOWN;
	}
	else if (state == KOOPAS_STATE_WALKING_LEFT) {
		ani = KOOPAS_ANI_RED_WALKING_RIGHT;
	}
	else if (state == KOOPAS_STATE_TAKEN) {
		ani = 2;
	}
	return ani;
}

int CKoopas::RenderKoopaGreen()
{
	if (isKoopaDie) ani = KOOPA_ANI_GREEN_TAKEN_UP;
	else if (vx < 0 && state != KOOPAS_STATE_SPIN_LEFT && state != KOOPAS_STATE_SPIN_RIGHT)
		ani = KOOPA_ANI_GREEN_WALKING_LEFT;
	else if (state == KOOPAS_STATE_REBORN && isDown)
		ani = KOOPA_ANI_GREEN_REBORN_DOWN;
	else if (state == KOOPAS_STATE_REBORN && !isDown)
		ani = KOOPA_ANI_GREEN_REBORN_UP;
	else if (state == KOOPAS_STATE_TORTOISESHELL_DOWN)
		ani = KOOPA_ANI_GREEN_TORTOISESHELL_DOWN;
	else if (state == KOOPAS_STATE_TORTOISESHELL_UP)
		ani = KOOPA_ANI_GREEN_TORTOISESHELL_UP;
	else if (state == KOOPAS_STATE_SPIN_LEFT && isDown || state == KOOPAS_STATE_SPIN_RIGHT && isDown)
		ani = KOOPA_ANI_GREEN_SPIN_DOWN;
	else if (state == KOOPAS_STATE_SPIN_LEFT && !isDown || state == KOOPAS_STATE_SPIN_RIGHT && !isDown)
		ani = KOOPA_ANI_GREEN_SPIN_UP;
	else if (state == KOOPAS_STATE_TAKEN && isDown)
		ani = KOOPA_ANI_GREEN_TAKEN_DOWN;
	else if (state == KOOPAS_STATE_TAKEN && !isDown)
		ani = KOOPA_ANI_GREEN_TAKEN_UP;
	else  ani = KOOPA_ANI_GREEN_WALKING_RIGHT;
	return ani;

}

int CKoopas::RenderParakoopaGreen()
{
	if (isKoopaDie) ani = KOOPA_ANI_GREEN_TAKEN_UP;
	else if (nx < 0) ani = PARAKOOPA_ANI_GREEN_JUMPING_LEFT;
	else ani = PARAKOOPA_ANI_GREEN_JUMPING_RIGHT;
	return ani;
}

void CKoopas::handlerDeflect()
{
	if (!stateDeflect) return;
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float ax, ay;
	mario->GetPosition(ax, ay);
	bool check = true;
	if (ax > x) {
		check = false;
	}
	if (GetTickCount64() - timeStartDeflect > PARAKOOPA_TIME_DEFECT)
	{
		/*if (vx > 0) {
			vx = vx - 0.015f;
		}
		if (vx == 0){
			stateDeflect = false;
			vx = 0;
		}*/
		vx = 0;
		stateDeflect = false;
	}
	else
	{
		SetState(KOOPAS_STATE_TORTOISESHELL_UP);
		vy = -0.1f;
		if (check) {
			vx = 0.1f ;
		}
		else {
			vx = -0.1f;
		}
	}
}

void CKoopas::handleReborn()
{
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (stateKoopaTortoiSeShell) {
	}

	if (!stateKoopaTortoiSeShell) {
	}
	// koopa chuẩn bị hồi sinh
	if (stateKoopaTortoiSeShell && GetTickCount64() - timeStateTorToiSeShell > KOOPA_TIME_REBORN_START
		&& GetTickCount64() - timeStateTorToiSeShell < KOOPA_TIME_REBORN_END) {
		SetState(KOOPAS_STATE_REBORN);
	}

	// hồi sinh qua trạng thái ban đầu
	else if (stateKoopaTortoiSeShell && GetTickCount64() - timeStateTorToiSeShell > KOOPA_TIME_REBORN_END) {
		timeNotEffect = (DWORD)GetTickCount64();
		mario->SetState(STATE_MARIO_IDLE);
		mario->SetMarioIsTortoiseshell(false);
		mario->MarioSetTortoiseshell(NULL);

		float marioX, marioY;
		mario->GetPosition(marioX, marioY);
		if (abs(marioX - x) < KOOPAS_REBORN_EFFECTY_MARIO && abs(marioY - y) < KOOPAS_REBORN_EFFECTY_MARIO) {
			if (mario->GetMarioLevel() == LEVEL_MARIO_SMAIL) {
				mario->SetState(STATE_MARIO_DIE);
			}
			else {
				mario->SetMarioLevel(mario->GetMarioLevel() - 1);
				mario->StartUntouchable();
			}
		}
		SetState(KOOPAS_STATE_WALKING_RIGHT);
	
	}
}

void CKoopas::CollisionWithBrick(LPCOLLISIONEVENT e)
{
	CBrick* brick = dynamic_cast<CBrick*>(e->obj);

	if (typeKoopa == KOOPA_COLOR_RED) {
		if (state == KOOPAS_STATE_SPIN_LEFT) {
			if (e->nx > 0) {
				/*DebugOut(L"Y of Koopas %f \n", y);
				DebugOut(L"Y of Brick %f \n", brick->y);
				DebugOut(L"height %f \n", y + GetHeightKoopas());*/
				if ((y + GetHeightKoopas()) > brick->y) {

					if (brick->y - (y + 16) < 0)
					{
						((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->CreatePieceBrick(x, y, (DWORD)GetTickCount64());
						brick->SetState(BRICK_STATE_HIDEN);
						SetState(KOOPAS_STATE_SPIN_RIGHT);
					}
				}
			}
		}
		
		if (state == KOOPAS_STATE_SPIN_RIGHT) {
			if (e->nx < 0) {
			/*	DebugOut(L"spin right \n");
				DebugOut(L"Y of Koopas %f \n", y);
				DebugOut(L"Y of Brick %f \n", brick->y);
				DebugOut(L"height %f \n", y + GetHeightKoopas());*/
				if ((y + GetHeightKoopas()) > brick->y) {
				//	DebugOut(L"spin right exute\n");
					brick->SetState(BRICK_STATE_HIDEN);
					((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->CreatePieceBrick(x, y, (DWORD)GetTickCount64());
					SetState(KOOPAS_STATE_SPIN_LEFT);
				}
			}
		}


		if (state == KOOPAS_STATE_WALKING_LEFT || state == KOOPAS_STATE_WALKING_RIGHT)
		{
			float tempX, tempY;
			brick->GetPosition(tempX,tempY);
			if (e->ny == 0) {
		
				if ((y + GetHeightKoopas()) > tempY) {
					if (e->nx > 0) {
						SetState(KOOPAS_STATE_WALKING_RIGHT);
					}
					else if (e->nx < 0) {
						SetState(KOOPAS_STATE_WALKING_LEFT);
					}
				}
			}
		}
		// TODO
	}
	if (typeKoopa == KOOPA_GREEN_FORM) {
		if (state == KOOPAS_STATE_WALKING_LEFT || state == KOOPAS_STATE_WALKING_RIGHT)
		{
			if (e->nx > 0) {
				SetState(KOOPAS_STATE_WALKING_RIGHT);
			}
			else if (e->nx < 0) {
				SetState(KOOPAS_STATE_WALKING_LEFT);
			}
		}
		if (state == KOOPAS_STATE_SPIN_LEFT) {
			if (e->nx > 0) {
				if (brick->y - (y + 16) < 0)
				{
					((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->CreatePieceBrick(x, y, (DWORD)GetTickCount64());
					brick->SetState(BRICK_STATE_HIDEN);
					SetState(KOOPAS_STATE_SPIN_RIGHT);
				}
			}
		}

		if (state == KOOPAS_STATE_SPIN_RIGHT) {
			if (e->nx < 0) {
				DebugOut(L"spin right exute\n");
				brick->SetState(BRICK_STATE_HIDEN);
				((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->CreatePieceBrick(x, y, (DWORD)GetTickCount64());
				SetState(KOOPAS_STATE_SPIN_LEFT);
			}
		}
	}
}

void CKoopas::CollisionWithMusic(LPCOLLISIONEVENT e)
{
	CMusic* music = dynamic_cast<CMusic*>(e->obj);
	if (typeKoopa == KOOPA_COLOR_RED) {
		
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
		if (dynamic_cast<CCoin*>(c->obj) || dynamic_cast<CPortal*>(c->obj)) {
			continue;
		}
		// else
		if (dynamic_cast<CColorBrick*>(c->obj)) {
			if (c->ny < 0) {
				min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
			}
			continue;
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
		else {
		}
	}
}



