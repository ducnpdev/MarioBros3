#include <algorithm>
#include <assert.h>
#include "Utils.h"
#include "Mario.h"
#include "Game.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Portal.h"
#include "Road.h"
#include "ColorBrick.h"
#include "QuestionBrick.h"
#include "Goomba.h"
#include "ItemCoin.h"
#include "Koopas.h"
#include "FirePlantBullet.h"
#include "FirePiranhaPlant.h"



CMario::CMario(float x, float y)
{
	level = LEVEL_MARIO_SMAIL;
	untouchable = 0;
	SetState(STATE_MARIO_IDLE);

	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y; 
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);
	vy += 0.0005*dt;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (state == STATE_MARIO_RUNNING_RIGHT || state == STATE_MARIO_RUNNING_LEFT) {
	//	DebugOut(L"11111111111111111111 \n");
	}

	// turn off collision when die 
	if (state!=STATE_MARIO_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size()==0)
	{
		x += dx; 
		y += dy;
		isStandingFloor = false;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0; 
		float rdy = 0;
		
	
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx*dx + nx*0.1f;
		y += min_ty*dy + ny*0.1f;

		if (nx!=0) vx = 0;
		if (ny!=0) vy = 0;
		
	
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (untouchable == 0)
			{
				if (dynamic_cast<CGoomba*>(e->obj))
				{
					CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
					if (e->ny < 0) {
						if (goomba->GetState() != GOOMBA_STATE_IDLE) {
							if (goomba->getColorGoomba() != PARA_GOOMBA_BROWN) {
								goomba->SetState(GOOMBA_STATE_DIE);
							}
							else {
								// change paraGoomba -> goomba yellow
								goomba->setColorGoomba(GOOMBA_YELLOW_COLOR);
							}
							// jump when collision goomba
							vy = -0.2f;
							isJump = 1;
						}
				
					}
					else if(e->nx != 0 || e->ny > 0) {
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							if (level > LEVEL_MARIO_SMAIL)
							{
							//	DebugOut(L"level:",GetMarioLevel());
								SetMarioLevel(GetMarioLevel() - 1 );
								StartUntouchable();
							}
							else
								SetState(STATE_MARIO_DIE);
						}
					}
				}

				if (dynamic_cast<CKoopas*>(e->obj))
				{
					CKoopas* koopa = dynamic_cast<CKoopas*>(e->obj);
				
					// collision above koopas
					if (e->ny < 0) {
						if (koopa->GetState() != KOOPAS_STATE_TORTOISESHELL_DOWN) {
							koopa->SetState(KOOPAS_STATE_TORTOISESHELL_DOWN);
							vy = -0.2f;
						}
						else if (koopa->GetState() == KOOPAS_STATE_TORTOISESHELL_DOWN) {
							if ( (x + GetBBoxWidthMario()) < (koopa->x + round(KOOPAS_BBOX_WIDTH / 2)) ) {
								koopa->SetState(KOOPAS_STATE_SPIN_RIGHT);
							}
							else {
								koopa->SetState(KOOPAS_STATE_SPIN_LEFT);
							}
						}
						isJump = 1;
					}
					// 
					else if (e->nx != 0 || e->ny > 0) {
						if (untouchable == 0)
						{
							if (koopa->GetState() != KOOPAS_STATE_TORTOISESHELL_DOWN || koopa->GetState() != KOOPAS_STATE_TORTOISESHELL_DOWN ) {
								DebugOut(L"111 \n");
								if (level > LEVEL_MARIO_SMAIL) {
									SetMarioLevel(GetMarioLevel() - 1);  
									StartUntouchable();
								}
								else {
									SetState(STATE_MARIO_DIE);
								}
							}
							else {

								if (state == STATE_MARIO_RUNNING_RIGHT  || state == STATE_MARIO_RUNNING_LEFT) {
									DebugOut(L"222 \n");
									tortoiseshell = koopa;
									koopa->SetState(KOOPAS_STATE_TAKEN);
									if (e->nx < 0) {
										SetState(STATE_MARIO_TORTOISESHELL_RIGHT);
									}
									else SetState(STATE_MARIO_TORTOISESHELL_LEFT);
							
								}
								else {
									DebugOut(L"333 \n");
									timeStartKick = GetTickCount();
									SetState(STATE_MARIO_KICK);
									if (e->nx < 0)
									{
										koopa->SetState(KOOPAS_STATE_SPIN_RIGHT);
									}
									else
									{
										koopa->SetState(KOOPAS_STATE_SPIN_LEFT);
									}
								}
							}

						}
					}
				}

				if (dynamic_cast<CFirePlantBullet*>(e->obj))
				{
					CFirePlantBullet* fireplantbullet = dynamic_cast<CFirePlantBullet*>(e->obj);
					if (fireplantbullet->GetState() != 100 && fireplantbullet->GetState() != 400)
					{
						if (level > LEVEL_MARIO_SMAIL)
						{
							// SetLevel(GetMarioLevel() - 1);	
							SetMarioLevel(GetMarioLevel() - 1);
							fireplantbullet->SetState(100);
							StartUntouchable();
						}
						else
							SetState(STATE_MARIO_DIE);
					}
				}

				if (dynamic_cast<CFirePiranhaPlant*>(e->obj))
				{
					CFirePiranhaPlant* fireplant = dynamic_cast<CFirePiranhaPlant*>(e->obj);
					if (fireplant->GetState() != 1/* && !fight_state*/)
					{
						if (level > LEVEL_MARIO_SMAIL)
						{
							SetMarioLevel(GetMarioLevel() - 1);
							// fireplant->SetState(FIREBULLET_DESTROY_STATE);
							StartUntouchable();
						}
						else
							SetState(STATE_MARIO_DIE);
					}
					/*else if (fight_state)
					{
						fireplant->SetState(5);
					}*/
				}

			}

			if (dynamic_cast<CRoad*>(e->obj) || dynamic_cast<CColorBrick*>(e->obj)) {
				marioSpeechJump = 0.0f;
				isJump = 0;
				vy = 0;
				marioStateFall = false;
				SetMarioFallState(false);
			}
		
			if (dynamic_cast<CQuestionBrick*>(e->obj)) {
				CQuestionBrick* questionBrick = dynamic_cast<CQuestionBrick*>(e->obj);
				if (e->ny > 0) {
					vy = 0; SetMarioFallState(true);

					if (questionBrick->GetState() == QUESTION_BRICK_STATE_MOVING) {
						// 200 là BRICK_STATE_INIT_COLLISION_MARIO
						questionBrick->SetItemWhenCollision(200);
						questionBrick->SetState(QUESTION_BRICK_ANI_CRETE);
					}

				}

				if (e->ny < 0) {
				}

			}

			if (dynamic_cast<CCoin*>(e->obj)) // if e->obj is Koopa
			{
				CCoin* coin = dynamic_cast<CCoin*>(e->obj);

				if (coin->GetState() == 2)
				{
					/*coinplay->AddCoin();
					coin->SetState(COIN_STATE_HIDEN);*/

				}
			}


		}
	}

	if (GetTickCount() - timeStartKick < 300) {
		SetState(STATE_MARIO_KICK);
	}

	handlerMarioJumpFly();

	handleMarioTorToiSeShell();

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMario::handlerMarioJumpFly() {
	if (GetTickCount() - timeMarioJumpFlyLow < 300)
	{
		if (nx > 0) SetState(900);
		else SetState(910);
	}
	else isJumpFlyLow = false;
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case STATE_MARIO_WALKING_RIGHT:
		isRunning = false;
		isJump = 0;
		isTurn = false;
		isKick = false;
		marioStateTorToiSeShell = false;
		isJumpFlyLow = false;

		vx = SPEED_MARIO_WALKING;
		nx = DIRECTION_MARIO_RIGHT;
		break;
	case STATE_MARIO_WALKING_LEFT: 
		isJump = 0;
		isTurn = false;
		isKick = false;
		marioStateTorToiSeShell = false;
		vx = -SPEED_MARIO_WALKING;
		nx = DIRECTION_MARIO_LEFT;
		break;
	case STATE_MARIO_RUNNING_RIGHT:
		isRunning = true;

		isJump = 0;			
		isKick = false;

		vx = SPEED_MARIO_RUNNING;
		if (isTurn)  vx = SPEED_MARIO_RUNNING + 0.01f;
		nx = DIRECTION_MARIO_RIGHT;
		break;
	case STATE_MARIO_RUNNING_LEFT:
		isRunning = true;
		marioStateTorToiSeShell = false;
		isKick = false;

		isJump = 0;

		vx = -SPEED_MARIO_RUNNING;
		if (isTurn)  vx = -SPEED_MARIO_RUNNING - 0.01f;
		nx = DIRECTION_MARIO_LEFT;
		break;		
	case STATE_MARIO_SITDOWN:
		isJump = 0;
		if (level != LEVEL_MARIO_TAIL)
		{
			if (!isStateSitDown)
			{
				y = y + 9;
			}
		}
		isStateSitDown = true;
		marioStateTorToiSeShell = false;
		break;
	case STATE_MARIO_JUMP:
		isKick = false;
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
		isJump = 1;
		vy = -0.13f - marioSpeechJump;
		break; 
	case STATE_MARIO_IDLE:
		isKick = false;
		isTurn = false;
		// isJump = 0;
		// marioSpeechJump = 0.0f;
		vx = 0;
		break;
	case STATE_MARIO_KICK:
		isKick = true;
	//	

		if (tortoiseshell != NULL)
		{
			if (nx > 0) {
				 tortoiseshell->x = x + GetBBoxWidthMario() ;
				tortoiseshell->SetState(KOOPAS_STATE_SPIN_RIGHT);
			}
			else
			{
				tortoiseshell->x = x - GetBBoxWidthMario() ;
				tortoiseshell->SetState(KOOPAS_STATE_SPIN_LEFT);
			}

			marioStateTorToiSeShell = false;
			tortoiseshell = NULL;
		}

		break;
	case STATE_MARIO_TURN_RIGHT:
		isTurn = true;
		isRunning = false;
		isKick = false;
		marioStateTorToiSeShell = false;

		vx = SPEED_MARIO_WALKING;
		if (isRunning) vx = SPEED_MARIO_WALKING + 0.5f;
		break;
	case STATE_MARIO_TURN_LEFT:
		isTurn = true;
		isRunning = false;
		marioStateTorToiSeShell = false;

		vx = -SPEED_MARIO_WALKING;
		if (isRunning) vx = -SPEED_MARIO_WALKING - 0.5f;
		break;
	case STATE_MARIO_DIE:
		marioStateDie = true;
		marioStateTorToiSeShell = false;

		vy = -SPEED_MARIO_DIE_DEFLECT;
		break;
	case STATE_MARIO_TORTOISESHELL_RIGHT:
		marioStateTorToiSeShell = true;
		isRunning = false;
		isTurn = false;
		isRunning = false;

		isJump = 0;
	//	vx = 0.00008f;
		break;
	case STATE_MARIO_TORTOISESHELL_LEFT:
		marioStateTorToiSeShell = true;
		isRunning = false;
		isTurn = false;
		isKick = false;
	//	vx = -0.00008f;
		isJump = 0;
		break;
	case 900:
		isJumpFlyLow = true;
		marioStateTorToiSeShell = false;
		isRunning = false;
		isTurn = false;
		isKick = false;
		vy = 0.05f;

		break;
	case 910:
		isJumpFlyLow = true;
		marioStateTorToiSeShell = false;
		isRunning = false;
		isTurn = false;
		vy = 0.05f;
		break;
	}
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y; 
	switch (level)
	{
	case LEVEL_MARIO_BIG:
		right = x + BBOX_MARIO_BIG_WIDTH;
		bottom = y + BBOX_MARIO_BIG_HEIGHT;
		if(isStateSitDown) {
			right = x + BBOX_MARIO_BIG_SIT_WIDTH;
			bottom = y + BBOX_MARIO_BIG_SIT_HEIGHT;
		}
		break;
	case LEVEL_MARIO_TAIL:
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
		if (isStateSitDown) {
			bottom = y + BBOX_MARIO_BIG_SIT_HEIGHT;
		}
		if (nx > 1)
		{
			left = x + 7;
		}
		break;	
	case LEVEL_MARIO_FIRE:
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
		if (isStateSitDown) {
			bottom = y + BBOX_MARIO_BIG_SIT_HEIGHT;
		}
		break;	
	default:
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
		break;
	}

}

void CMario::FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy)
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

		if (dynamic_cast<CColorBrick*>(c->obj))
		{
			if (c->ny < 0 && c->t < min_tx)
			{
				min_ty = c->t; ny = c->ny; rdy = c->dy;
				coEventsResult.push_back(coEvents[i]);
			}
		}
		else if (dynamic_cast<CGoomba*>(c->obj)) {
			if (c->nx != 0 && c->t < 1.0f)
			{
				min_tx = c->t; nx = c->nx; rdx = c->dx;
				coEventsResult.push_back(coEvents[i]);
			}

			if (c->ny != 0 && c->t < 1.0f)
			{
				min_ty = c->t; ny = c->ny; rdy = c->dy;
				coEventsResult.push_back(coEvents[i]);
			}
		}
		/*if (dynamic_cast<CGoomba*>(c->obj)) {
			if (c->t < min_tx && c->nx != 0) {
				min_tx = c->t; nx = c->nx; rdx = c->dx;
				coEventsResult.push_back(coEvents[i]);
			}

			if (c->t < min_ty && c->ny != 0) {
				min_ty = c->t; ny = c->ny; rdy = c->dy;
				coEventsResult.push_back(coEvents[i]);
			}
		}*/
		else {
			if (c->t < min_tx && c->nx != 0) {
				min_tx = c->t; nx = c->nx; rdx = c->dx;
				coEventsResult.push_back(coEvents[i]);
			}

			if (c->t < min_ty && c->ny != 0) {
				min_ty = c->t; ny = c->ny; rdy = c->dy;
				coEventsResult.push_back(coEvents[i]);
			}
		}

		
	}

//	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	// if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);

}

/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(STATE_MARIO_IDLE);
	SetLevel(LEVEL_MARIO_BIG);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

void CMario::Render()
{
	if (state == STATE_MARIO_DIE) ani = MARIO_ANI_DIE;
	else
		switch (level)
		{
		case LEVEL_MARIO_SMAIL:
			ani = RenderLevelMarioSmall();
			break;
		case LEVEL_MARIO_BIG:
			ani = RenderLevelMarioBig();
			break;
		case LEVEL_MARIO_TAIL:
			ani = RenderLevelMarioTail();
			break;
		case LEVEL_MARIO_FIRE:
			ani = RenderLevelMarioFire();
			break;
		default:
			break;
		}

	int alpha = 255;
	if (untouchable) alpha = 128;
	animation_set->at(ani)->Render(x, y, alpha);
	RenderBoundingBox();
}

int CMario::RenderLevelMarioSmall() {
	if (vx == 0)
	{
		if (nx > 0) {
			if (isStateSitDown) ani = 0;
			else if (isJump == 1) ani = 24;
			else if (isKick) ani = 32;
			else if (marioStateTorToiSeShell) ani = 40;
			else ani = 0;
		} 
		else {
			if (isStateSitDown) ani = 1;
			else if (isJump == 1) ani = 25;
			else if (isKick) ani = 33;
			else if (marioStateTorToiSeShell) ani = 41;
			else ani = 1;
		}
	}
	else if (vx > 0){
		if (isTurn && !isRunning) ani = 64;
		else if (isJump == 1) ani = 24;
		else if (isRunning) ani = 16;
		else if (isKick == 1) ani = 32;
		else if (marioStateTorToiSeShell) ani = 42;

		else ani = 4;
	} 
	else {
		if (isTurn && !isRunning) ani = 65;
		else if (isJump == 1) ani = 25;
		else if (isRunning) ani = 17;
		else if (isKick) ani = 33;
		else if (marioStateTorToiSeShell) ani = 43;
		else ani = 5;
	}

	return ani;
}

int CMario::RenderLevelMarioBig() {
	if (vx == 0)
	{
		if (nx > 0) {
			if (isStateSitDown) ani = 85;
			else if (isJump == 1) ani = 26;
			else if (isKick) ani = 34;
			else if (marioStateTorToiSeShell) ani = 46;
			else ani = 2;
		} 
		else {
			if (isStateSitDown) ani = 86;
			else if (isJump == 1) ani = 27;
			else if (isKick) ani = 35;
			else if (marioStateTorToiSeShell) ani = 47;
			else ani = 3;
		}
	}
	else if (vx > 0){
		
		
		if (isTurn && !isRunning) ani =  66;
		else if (isJump == 1) ani = 26;
		else if(isRunning) ani = 18;
		else if (isKick) ani = 34;
		else if (marioStateTorToiSeShell)	ani = 48;
		else ani = 6; 
	} 
	else {
		if (isTurn && !isRunning) ani = 67;
		else if (isJump == 1 ) ani = 27;
		else if(isRunning) ani = 19;
		else if (isKick) ani = 35;
		else if (marioStateTorToiSeShell) 	ani = 49;
		else ani = 7; 
	}
	
	return ani;
}

int CMario::RenderLevelMarioTail() {
	if (vx == 0)
	{
		if (nx > 0) {
			if (isStateSitDown) ani = 87;
			else if (isJump == 1) ani = 28;
			else if (isJumpFlyLow == 1) ani = 81;
			else if (isKick) ani = 36;
			else if (marioStateTorToiSeShell) 	ani = 52;
			else ani = 8;	
		} 
		else {
			if (isStateSitDown) ani = 88;
			else if (isJump == 1) ani = 29;
			else if (isJumpFlyLow == 1) ani = 82;
			else if (isKick) ani = 37;
			else if (marioStateTorToiSeShell) 	ani = 53;
			else ani = 9;
		}
	}
	else if (vx > 0){
		if (isTurn && !isRunning) ani =  68;
		else if (isJumpFlyLow == 1) ani = 81;
		else if (isJump == 1) ani = 28;
		else if(isRunning) ani = 20;
		else if (isKick) ani = 36;
		else if (marioStateTorToiSeShell) 	ani = 54;
		else ani = 10; 
	} 
	else {
		if (isTurn && !isRunning) ani = 69;
		else if (isJump == 1 ) ani = 29;
		else if (isJumpFlyLow == 1) ani = 82;
		else if(isRunning) ani = 21;
		else if (isKick) ani = 37;
		else if (marioStateTorToiSeShell) 	ani = 55;
		else ani = 11; 
	}
	return ani;
}

int CMario::RenderLevelMarioFire() {
	if (vx == 0)
	{
		if (nx > 0) {
			if (isStateSitDown) ani = ANI_MARIO_FIRE_SIT_RIGHT;
			else if (isJump == 1 || isJump == -1) ani = ANI_MARIO_FIRE_JUMP_RIGHT;
			else ani = ANI_MARIO_FIRE_IDLE_RIGHT;	
		} 
		else {
			if (isStateSitDown) ani = ANI_MARIO_FIRE_SIT_LEFT;
			else if (isJump == 1 || isJump == -1) ani = ANI_MARIO_FIRE_JUMP_LEFT;
			else ani = ANI_MARIO_FIRE_IDLE_LEFT;
		}
	}
	else if (vx > 0){
		if (isTurn && !isRunning) ani =  ANI_MARIO_FIRE_TURN_RIGHT;
		else if (isJump == 1) ani = ANI_MARIO_FIRE_JUMP_RIGHT;
		else if (isRunning) ani = ANI_MARIO_FIRE_RUN_RIGHT;
		else ani = ANI_MARIO_FIRE_WALK_RIGHT; 
	} 
	else {
		if (isTurn && !isRunning) ani = ANI_MARIO_FIRE_TURN_LEFT;
		else if (isJump == 1 ) ani = ANI_MARIO_FIRE_JUMP_LEFT;
		else if(isRunning) ani = ANI_MARIO_FIRE_RUN_LEFT;
		else ani = ANI_MARIO_FIRE_WALK_LEFT; 
	}
	
	return ani;
}

int CMario::GetBBoxWidthMario()
{
	int width = 0;
	switch (level)
	{
	case LEVEL_MARIO_SMAIL:
		width =  MARIO_SMALL_BBOX_WIDTH;
	case LEVEL_MARIO_BIG:
		width = BBOX_MARIO_BIG_WIDTH;
	case LEVEL_MARIO_TAIL:
		width = BBOX_MARIO_TAIL_WIDTH;
	case LEVEL_MARIO_FIRE:
		width = BBOX_MARIO_FIRE_WIDTH;
	}
	return round(width);
}

int CMario::GetBBoxHeightMario()
{
	int height = 0;
	switch (level)
	{
	case LEVEL_MARIO_SMAIL:
		height = MARIO_SMALL_BBOX_WIDTH;
	case LEVEL_MARIO_BIG:
		height = BBOX_MARIO_BIG_HEIGHT;
	case LEVEL_MARIO_TAIL:
		height = MARIO_BIG_BBOX_HEIGHT;
	case LEVEL_MARIO_FIRE:
		height = MARIO_BIG_BBOX_HEIGHT;
	}
	return round(height);
}


void CMario::handleMarioTorToiSeShell()
{
	if (tortoiseshell == NULL) return;
	switch (level)
	{
	case LEVEL_MARIO_SMAIL:
		if (nx > 0)
		{
			SetState(STATE_MARIO_TORTOISESHELL_RIGHT);
			tortoiseshell->x = x + 10;
			tortoiseshell->y = y - 2;
		}
		else
		{
			SetState(STATE_MARIO_TORTOISESHELL_LEFT);
			tortoiseshell->x = x - 10;
			tortoiseshell->y = y - 2;
		}
		break;
	case LEVEL_MARIO_BIG:
		plustortoiseshellInMario(MARIO_TORTOISESHELL_PLUS_10, MARIO_TORTOISESHELL_PLUS_10);
		break;
	case LEVEL_MARIO_TAIL:
		plustortoiseshellInMario(MARIO_TORTOISESHELL_PLUS_10, MARIO_TORTOISESHELL_PLUS_15);
		break;
	case LEVEL_MARIO_FIRE:
		// SetLevel(LEVEL_MARIO_FIRE);
		break;
	}
}

void CMario::marioSetUpDownLevel(int _level)
{
	if (level == LEVEL_MARIO_SMAIL && _level != LEVEL_MARIO_SMAIL) SetPosition(x, y - 15);

	switch (_level)
	{
	case LEVEL_MARIO_SMAIL:
		SetLevel(LEVEL_MARIO_SMAIL);
		break;
	case LEVEL_MARIO_BIG:
		SetLevel(LEVEL_MARIO_BIG);
		break;
	case LEVEL_MARIO_TAIL:
		SetLevel(LEVEL_MARIO_TAIL);
		break;
	case LEVEL_MARIO_FIRE:
		SetLevel(LEVEL_MARIO_FIRE);
		break;
	}
}

  
void CMario::plustortoiseshellInMario(int numberPlusLeft, int numberPlusRight) {
	if (nx > 0)
	{
		SetState(STATE_MARIO_TORTOISESHELL_RIGHT);
		tortoiseshell->x = x + numberPlusRight;
		tortoiseshell->y = y + numberPlusRight;
	}
	else
	{
		SetState(STATE_MARIO_TORTOISESHELL_LEFT);
		tortoiseshell->x = x - numberPlusLeft;
		tortoiseshell->y = y + numberPlusLeft;
	}
}
