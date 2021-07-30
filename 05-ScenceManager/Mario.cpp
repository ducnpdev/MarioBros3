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
#include "PiranhaPlant.h"
#include "Leaf.h"
#include "Brick.h"
#include "Mushroom.h"
#include "Switch.h"
#include "GoldCard.h"
#include "Pipe.h"
#include "GoombaConfig.h"
#include "WoodBlock.h"	
#include "BoomerangBros.h"	
#include "Boomerang.h"	
#include "BlueBrick.h"	
#include "FireBullet.h"	
#include "Music.h"	
#include "GoombaMini.h"	
#include "Gift.h"	

CMario::CMario(float x, float y)
{
	level = LEVEL_MARIO_TAIL;
	untouchable = 0;
	SetState(STATE_MARIO_IDLE);
	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y; 
	marioStateDie = false;
	timeMarioDead = 0;
	timeMarioShootFire = 0;
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
//	DebugOut(L"x,y %f %f \n", x, y);
	CGameObject::Update(dt);
	if (!marioStateDie) {
		vy += 0.00038f * dt;
	} 
	if (marioPreEndScene) {
		vx = 0.1f;
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	handlerMarioUpLevelOtherSmall();
	handlerMarioUpLevelSmoke();
	coEvents.clear();

	if (state == STATE_MARIO_RUNNING_RIGHT || state == STATE_MARIO_RUNNING_LEFT) {
	}

	// turn off collision when die 
	if (state!=STATE_MARIO_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
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
	
		CMario::FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx*dx + nx*0.1f;
		y += min_ty*dy + ny*0.1f;

		if (nx!=0) vx = 0;
		if (ny!=0) vy = 0;
		
	
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			// colission effect
			if (untouchable == 0)
			{
			
				if (dynamic_cast<CGoomba*>(e->obj)) {
					CollisionWithGoomba(e);
				}

				if (dynamic_cast<CKoopas*>(e->obj)) {
					CollisionWithKoopa(e);
				}

				if (dynamic_cast<CMusic*>(e->obj)) {
					CollisionWithMusic(e);
				}

				if (dynamic_cast<CWoodBlock*>(e->obj)) {
					isJump = 0;
					CollisionWithWoodBlock(e);
					marioSpeechJump = 0.0f;
				}
				if (dynamic_cast<CBlueBrick*>(e->obj)) {
					isJump = 0;
					isAcceptFlyCamera = false;
					marioSpeechJump = 0.0f;
				}
				if (dynamic_cast<CBoomerang*>(e->obj)) {
					// CollisionWithBoomerang(e);
				}

				if (dynamic_cast<CBoomerangBro*>(e->obj)) {
					CollisionWithBoomerangBros(e);
				}

				if (dynamic_cast<CFirePlantBullet*>(e->obj))
				{
					//DebugOut(L"Mario Collision Plant bullet \n");
					CFirePlantBullet* fireplantbullet = dynamic_cast<CFirePlantBullet*>(e->obj);
					if (fireplantbullet->GetState() != 100 && fireplantbullet->GetState() != 400)
					{
						if (e->ny > 0) {
							y = y - MARIO_DEFECT_Y_COLLISION;
						}
						if (level > LEVEL_MARIO_SMAIL)
						{
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
					if (fireplant->GetState() != 1 && !marioStateFight)
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
					else if (marioStateFight)
					{
						fireplant->SetState(FIREPIRANHAPLANT_STATE_DESTROY);
						DisplayListScore(MARIO_SCORE_100, fireplant->x, fireplant->y, (DWORD)GetTickCount64());
					}
				}

				if (dynamic_cast<CPiranhaPlant*>(e->obj))
				{
					CPiranhaPlant* plant = dynamic_cast<CPiranhaPlant*>(e->obj);
					if (plant->GetState() != PIRANHAPLANT_STATE_HIDE && !marioStateFight)
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
					else if (marioStateFight)
					{
						plant->SetState(FIREPIRANHAPLANT_STATE_DESTROY);
						DisplayListScore(MARIO_SCORE_100, plant->x, plant->y, (DWORD)GetTickCount64());
					}
				}
			}
			if (dynamic_cast<CRoad*>(e->obj)) {
				marioSpeechJump = 0.0f;
				isJump = 0;
				vy = 0;
				marioStateFall = false;
				SetMarioFallState(false);
				isAcceptFlyCamera = false;
				y = y - 0.01f;
				isMarioNotJump = false;
				if (untouchable == 1) {
				}
			}
			if ( dynamic_cast<CColorBrick*>(e->obj)) {
				marioSpeechJump = 0.0f;
				isJump = 0;
				vy = 0;
				marioStateFall = false;
				SetMarioFallState(false);
			}
		
			if (dynamic_cast<CQuestionBrick*>(e->obj)) {
				marioSpeechJump = 0.0f;
				isJump = 0;
				CQuestionBrick* questionBrick = dynamic_cast<CQuestionBrick*>(e->obj);

				if (e->ny > 0) {
					vy = 0; 
					SetMarioFallState(true);
					if (questionBrick->GetState() == QUESTIONBLOCK_ITEM_STATE) {
						if (dynamic_cast<CCoin*>(questionBrick->GetItemInBrick())) {
							coinplay->AddCoinHub();
						}
						questionBrick->SetItemWhenCollision(COIN_EFFECT_ANI); // 200 là BRICK_STATE_INIT_COLLISION_MARIO
						questionBrick->SetState(QUESTIONBLOCK_DEFLECT_STATE);
					}
				}
				if (e->nx != 0) {
					if (marioStateFight) {
						// TODO 
						if (questionBrick->GetState() == QUESTIONBLOCK_ITEM_STATE) {
							if (dynamic_cast<CCoin*>(questionBrick->GetItemInBrick()))
								coinplay->AddCoinHub();
							questionBrick->SetItemWhenCollision(COIN_EFFECT_ANI);
							questionBrick->SetState(QUESTIONBLOCK_DEFLECT_STATE);
						}
					}
				}
			}

			if (dynamic_cast<CCoin*>(e->obj)) // if e->obj is Koopa
			{
				/*CCoin* coin = dynamic_cast<CCoin*>(e->obj);
				if (coin->GetState() == COIN_STATE_NORMAL)
				{
					coinplay->AddCoinHub();
					coin->SetState(COIN_STATE_HIDEN);
				}*/
			}

			if (dynamic_cast<CGoalCard*>(e->obj)) {
				CGoalCard* goalCard = dynamic_cast<CGoalCard*>(e->obj);
				CGame* game = CGame::GetInstance();
				if (goalCard->GetCheckPrene()) return;
				if (goalCard->GetState() == GOALCARD_STATE_HIDEN) return;
				SetCardState(goalCard->GetState());
				game->SetItemGoalCard(goalCard->GetState());
				goalCard->SetSwitchScene((DWORD)GetTickCount64());
				goalCard->GetCardText()->SetState(1);
				goalCard->GetCardText()->GetCard()->SetState(goalCard->GetState());
				goalCard->SetState(COIN_STATE_HIDEN);

				//goalCard->SetTimePre_end((DWORD)GetTickCount64());
				//goalCard->SetCheckPrene(true);

			}

			if (dynamic_cast<CLeaf*>(e->obj))
			{
				CLeaf* leaf = dynamic_cast<CLeaf*>(e->obj);
				if (leaf->GetState() != LEAF_STATE_HIDEN)
				{
					leaf->SetState(LEAF_STATE_HIDEN);

					if (level == LEVEL_MARIO_SMAIL) {
						SetPosition(x, y - UP_DOWN_POSITOIN_Y);
						SetState(STATE_MARIO_UP_LEVEL);
						timeMarioUpLevel = (DWORD)GetTickCount64();
					}
					if (level == LEVEL_MARIO_BIG) {
						SetState(STATE_MARIO_SMOKE);
						SetPosition(x, y - 2);
						timeMarioSmoke = (DWORD)GetTickCount64();
					}

					//SetState(STATE_MARIO_SMOKE);
					//SetPosition(x, y - 2);
					//timeMarioSmoke = (DWORD)GetTickCount64();
					DisplayListScore(MARIO_SCORE_1000, leaf->x, leaf->y, (DWORD)GetTickCount64());
				}
			}


			if (dynamic_cast<CSwitch*>(e->obj))
			{
				CSwitch* switchs = dynamic_cast<CSwitch*>(e->obj);
				if (e->ny < 0)
				{ 
					if (switchs->GetState() == EFFECT_STATE)
					{
						switchs->SetState(SWITCH_STATE_ACTIVE);
						switchs->SetPosition(switchs->x, switchs->y + 9);
						switchs->SetSwitch();
						switchs->SetSwitchTime((DWORD)GetTickCount64());
					}
				}
			}
			if (dynamic_cast<CBrick*>(e->obj)) {
				isJump = 0;
				CollisionWithBrick(e);
			}
			if (dynamic_cast<CPipe*>(e->obj)) {
				marioSpeechJump = 0.0f;
				CollisionWithPipe(e);
				isJump = 0;
			}
			if (dynamic_cast<CMushroom*>(e->obj))
			{
				CollisionWithMushroom(e);
			}
		}
	}

	if (GetTickCount64() - timeStartKick < TIME_MARIO_KICK) {
		SetState(STATE_MARIO_KICK);
	}
	handlerMarioJumpFly();
	handleMarioTorToiSeShell();
	handlerMarioFight();
	MarioHanlerProcessArrow();
	checkMarioMaxPower();
	handlerMarioDownPipe();
	handlerMarioUpPipe();
	handleMarioFlyHigh();
	handleMarioDead();
	handleMarioDeadFly();
	handlerMarioShootFire();
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	if (y < GAME_POSITION_Y_LIMIT) y = GAME_POSITION_Y_LIMIT;
	UpdateSub(coObjects);

}

void CMario::UpdateSub(vector<LPGAMEOBJECT>* coObjects)
{
	vector<LPGAMEOBJECT> colidingObjects;
	isCollidingObject(coObjects, colidingObjects);
	for (UINT i = 0; i < colidingObjects.size(); i++)
	{
		LPGAMEOBJECT c = colidingObjects[i];
		if (untouchable == 0)
		{
			if (dynamic_cast<CGift*>(c)) {
				CGift* gift = dynamic_cast<CGift*>(c);
				gift->SetState(GIFT_OPEN);
				gift->SetTimeStartOpen((DWORD)GetTickCount64());
			}

			if (dynamic_cast<CBoomerang*>(c)) {
				if (untouchable == 0) {
					 CBoomerang* boomerang = dynamic_cast<CBoomerang*>(c);
					 if (boomerang->GetState() != STATE_BOOMERANG_HIDEN)
					 {
						/* if (e->ny > 0) {
							 y = y - MARIO_DEFECT_Y_COLLISION;
						 }*/
						 if (level > LEVEL_MARIO_SMAIL)
						 {
							 SetMarioLevel(GetMarioLevel() - 1);
							 StartUntouchable();
						 }
						 else SetState(STATE_MARIO_DIE);
					 }
				}
			}
			if (dynamic_cast<CGoombaMini*>(c)) {
				// CollisionWithGoombaMini(c);
				CGoombaMini* goombaMini = dynamic_cast<CGoombaMini*>(c);
				if (!marioHaveGoompaMini && marioStateFight) {
					goombaMini->SetState(STATE_GOOMBA_MINI_HIDEN);
					return;
				}
				marioHaveGoompaMini = true;
				if (!goombaMini->GetCheckCollisionMario()) {
					timeMarioHaveGoompaMini = (DWORD)GetTickCount64();
					goombaMini->SetState(STATE_GOOMBA_MINI_MOVING_IN_MARIO);
				}
			}
			if (dynamic_cast<CPortal*>(c))
			{
				isMarioInPortal = true;
			}
			else if (dynamic_cast<CCoin*>(c)) {
				isMarioInPortal = false;
				CCoin* coin = dynamic_cast<CCoin*>(c);
				if (coin->GetState() == COIN_STATE_NORMAL)
				{
					coinplay->AddCoinHub();
					coin->SetState(COIN_STATE_HIDEN);
				}
			}

			if (dynamic_cast<CMusic*>(c))
			{
				marioSpeechJump = 0.0f;
				/*CMusic* music = dynamic_cast<CMusic*>(c);
				if (isRenderMusic) {
				}

				if (!isRenderMusic) {
				}*/

				//CMusic* music = dynamic_cast<CMusic*>(c);
				//// music->GetTypeMusic();
				//DebugOut(L"music %d \n", music->GetTypeMusic());
			}

		}
	}
}

void CMario::MarioCollisionPiranhaPlant()
{
}

void CMario::handlerMarioJumpFly() {
	if (GetTickCount64() - timeMarioJumpFlyLow < MARIO_TIME_FLY_LOW)
	{
		if (nx > 0) SetState(STATE_MARIO_FLYING_LOW_RIGHT);
		else SetState(STATE_MARIO_FLYING_LOW_LEFT);
	}
	else isJumpFlyLow = false;
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case STATE_MARIO_PRE_SWITCH_SCENE:
		vx = SPEED_MARIO_WALKING;
		nx = DIRECTION_MARIO_RIGHT;
		break;
	case STATE_MARIO_SCENE_3_TOP_LEFT:
		autoChangeAni = true;
		break;
	case STATE_MARIO_FLYING_HIGH_RIGHT:
		nx = 1;
		isRunning = false;
		isJump = 0;
		isTurn = false;
		isKick = false;
		marioStateFlyHigh = true;
		vy = -MARIO_FLYING_SPEED_Y;
		break;
	case STATE_MARIO_FLYING_HIGH_LEFT:
		isStateSitDown = false;

		nx = -1;
		isRunning = false;
		isJump = 0;
		isTurn = false;
		isKick = false;
		marioStateFlyHigh = true;
		vy = -MARIO_FLYING_SPEED_Y;
		break;
	case STATE_MARIO_WALKING_RIGHT:
		isStateSitDown = false;
		autoChangeAni = false;
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
		isStateSitDown = false;
		isJump = 0;
		isTurn = false;
		isKick = false;
		marioStateTorToiSeShell = false;
		vx = -SPEED_MARIO_WALKING;
		nx = DIRECTION_MARIO_LEFT;
		break;
	case STATE_MARIO_RUNNING_FAST_RIGHT:
		isStateSitDown = false;

		isRunning = true;
		isJump = 0;
		isKick = false;
		vx = SPEED_MARIO_RUNNING_FAST;
		if (isTurn)  vx = SPEED_MARIO_RUNNING + 0.01f;
		nx = DIRECTION_MARIO_RIGHT;
		break;
	case STATE_MARIO_RUNNING_RIGHT:
		isRunning = true;
		isJump = 0;			
		isKick = false;
		vx = SPEED_MARIO_RUNNING;
		if (isTurn)  vx = SPEED_MARIO_RUNNING + 0.01f;
		nx = DIRECTION_MARIO_RIGHT;
		isStateSitDown = false;

		break;
	case STATE_MARIO_RUNNING_LEFT:
		isRunning = true;
		marioStateTorToiSeShell = false;
		isKick = false;
		isJump = 0;
		isStateSitDown = false;

		vx = -SPEED_MARIO_RUNNING;
		if (isTurn)  vx = -SPEED_MARIO_RUNNING - 0.01f;
		nx = DIRECTION_MARIO_LEFT;
		break;		
	case STATE_MARIO_SITDOWN:
		isJump = 0;
		/*if (level != LEVEL_MARIO_TAIL)
		{
			if (!isStateSitDown)
			{
				DebugOut(L"111111111 \n");
				y = y + 9;
			}
		}*/
		isStateSitDown = true;
		marioStateTorToiSeShell = false;
		break;
	case STATE_MARIO_JUMP:
		isKick = false;
		isJump = 1;
		if (marioStateMaxPower) {
			vy = -0.17f - marioSpeechJump;
		}
		else {
			if (marioHaveGoompaMini) {
				vy = -0.05f;
			}
			else {
				vy = -0.1f - marioSpeechJump;
			}
		}
		break; 
	case STATE_MARIO_IDLE:
		isKick = false;
		autoChangeAni = false;
		marioStateFlyHigh = false;
		isTurn = false;
		// isStateSitDown = false;
		// isJump = 0;
		// marioSpeechJump = 0.0f;
		vx = 0;
		break;
	case STATE_MARIO_KICK:
		isKick = true;
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
		timeMarioDead = (DWORD)GetTickCount64();
		marioStateTorToiSeShell = false;
		tortoiseshell = NULL;
	//	vy = -SPEED_MARIO_DIE_DEFLECT;
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
		isJump = 0;
		break;
	case STATE_MARIO_FLYING_LOW_RIGHT:
		isJumpFlyLow = true;
		marioStateTorToiSeShell = false;
		isRunning = false;
		isTurn = false;
		isKick = false;
		vy = MARIO_SPEED_Y_FYL_SLOW;
		break;
	case STATE_MARIO_FLYING_LOW_LEFT:
		isJumpFlyLow = true;
		marioStateTorToiSeShell = false;
		isRunning = false;
		isTurn = false;
		vy = MARIO_SPEED_Y_FYL_SLOW;
		break;
	case STATE_MARIO_UP_LEVEL:
		marioStateUpLevel = true;
		vx = 0;
		vy = 0;
		break;
	case STATE_MARIO_SMOKE:
		marioStateSmoke = true;
		vy = 0;
		vx = 0;
		break;
	case STATE_MARIO_FIGHT:
		marioStateFight = true;
		break;
	case STATE_MARIO_PIPE_DOWN:
		marioStatePipeDown = true;
		break;
	case STATE_MARIO_PIPE_UP:
		marioStatePipeUp = true;
		break;
	case STATE_MARIO_SHOOT_FIRE_BULLET_RIGHT: 
		nx = 1;
		marioStateShootFire = true;
		break;
	case STATE_MARIO_SHOOT_FIRE_BULLET_LEFT: 
		nx = -1;
		marioStateShootFire = true;
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
		if (nx == 1)
		{
			right = x + MARIO_BIG_BBOX_WIDTH + MARIO_BIG_BBOX_WIDTH_RIGHT_PLUS;
			left = x + MARIO_BIG_BBOX_WIDTH_LEFT_PLUS;
		}
		break;	
	case LEVEL_MARIO_FIRE:
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
		if (isStateSitDown) {
			bottom = y + BBOX_MARIO_BIG_SIT_HEIGHT;
		}
		break;	
	/*case STATE_MARIO_PIPE_DOWN:
		marioStatePipeDown = true;
		break;*/
	default:
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
		if (marioStateUpLevel) {
			bottom = y + MARIO_BIG_BBOX_HEIGHT;
		}
		break;
	}

}

void CMario::FilterCollision(
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

		if (dynamic_cast<CPortal*>(c->obj)) continue;
		if (dynamic_cast<CCoin*>(c->obj)) continue;
		if (dynamic_cast<CGoombaMini*>(c->obj)) continue;
		if (dynamic_cast<CBoomerang*>(c->obj)) continue;
		if (dynamic_cast<CGift*>(c->obj)) continue;
	//	if (dynamic_cast<CMusic*>(c->obj)) continue;
		else if (dynamic_cast<CColorBrick*>(c->obj))
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
		/*else if (dynamic_cast<CQuestionBrick*>(c->obj)) {
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

void CMario::isCollidingObject(vector<LPGAMEOBJECT>* coObjects, vector<LPGAMEOBJECT>& colidingObjects)
{
	float otherL;
	float otherT;
	float otherB;
	float otherR;

	float objectL;
	float objectT;
	float objectB;
	float objectR;
	GetBoundingBox(objectL, objectT, objectR, objectB);
	for (size_t i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CPortal*>(coObjects->at(i))
			|| dynamic_cast<CCoin*>(coObjects->at(i))
			|| dynamic_cast<CMusic*>(coObjects->at(i))
			|| dynamic_cast<CGoombaMini*>(coObjects->at(i))
			|| dynamic_cast<CBoomerang*>(coObjects->at(i))
			|| dynamic_cast<CGift*>(coObjects->at(i))
		)
		{
			coObjects->at(i)->GetBoundingBox(otherL, otherT, otherR, otherB);
			if (otherL <= objectR &&
				otherR >= objectL &&
				otherT <= objectB &&
				otherB >= objectT)
			{
				colidingObjects.push_back(coObjects->at(i));
			}
		}
	}
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
			if (autoChangeAni) ani = ANI_MARIO_SMALL_WALKING_RIGHT;
			else if (marioStateUpLevel) ani = ANI_MARIO_UP_LEVEL_SMALL_TO_BIG_RIGHT;
			else if (isStateSitDown) ani = ANI_MARIO_SMALL_SITDOWN_RIGHT;
			else if (isJump == 1) ani = ANI_MARIO_SMALL_JUMP_RIGHT;
			else if (isKick) ani = ANI_MARIO_SMALL_KICK_RIGHT;
			// else if (marioStateMaxPower) ani = 116;
			else if (marioStateTorToiSeShell) ani = ANI_MARIO_SMALL_IDLE_TORTOISESHELL_RIGHT;
			else ani = ANI_MARIO_SMALL_IDLE_RIGHT;
		} 
		else {
			if (autoChangeAni) ani = ANI_MARIO_SMALL_TURN_RIGHT;
			else if (marioStateUpLevel) ani = ANI_MARIO_UP_LEVEL_SMALL_TO_BIG_LEFT;
			else if (isStateSitDown) ani = ANI_MARIO_SMALL_SITDOWN_LEFT;
			else if (isJump == 1) ani = ANI_MARIO_SMALL_JUMP_LEFT;
			// else if (marioStateMaxPower) ani = 117;
			else if (isKick) ani = ANI_MARIO_SMALL_KICK_LEFT;
			else if (marioStateTorToiSeShell) ani = ANI_MARIO_SMALL_IDLE_TORTOISESHELL_LEFT;
			else ani = ANI_MARIO_SMALL_IDLE_LEFT;
		}
	}
	else if (vx > 0){
		if (marioStateUpLevel) ani = ANI_MARIO_UP_LEVEL_SMALL_TO_BIG_RIGHT;
		else if (marioStateMaxPower) ani = ANI_MARIO_SMALL_MAX_RUNNING_RIGHT;
		else if (isTurn && !isRunning) ani = ANI_MARIO_SMALL_TURN_RIGHT;
		else if (isJump == 1) ani = ANI_MARIO_SMALL_JUMP_RIGHT;
		else if (isRunning) ani = ANI_MARIO_SMALL_RUNNING_RIGHT;
		else if (isKick == 1) ani = ANI_MARIO_SMALL_KICK_RIGHT;
		else if (marioStateTorToiSeShell) ani = ANI_MARIO_SMALL_WALKING_TORTOISESHELL_RIGHT;
		else ani = ANI_MARIO_SMALL_WALKING_RIGHT;
	} 
	else {
		if (marioStateUpLevel) ani = ANI_MARIO_UP_LEVEL_SMALL_TO_BIG_LEFT;
		else if (marioStateMaxPower) ani = ANI_MARIO_SMALL_MAX_RUNNING_LEFT;
		else if (isTurn && !isRunning) ani = ANI_MARIO_SMALL_TURN_LEFT;
		else if (isJump == 1) ani = ANI_MARIO_SMALL_JUMP_LEFT;
		else if (isRunning) ani = ANI_MARIO_SMALL_RUNNING_LEFT;
		else if (isKick) ani = ANI_MARIO_SMALL_KICK_LEFT;
		else if (marioStateTorToiSeShell) ani = ANI_MARIO_SMALL_WALKING_TORTOISESHELL_LEFT;
		else ani = ANI_MARIO_SMALL_WALKING_LEFT;
	}
	return ani;
}

int CMario::RenderLevelMarioBig() {
	if (vx == 0)
	{
		if (nx > 0) {
			if (autoChangeAni) ani = ANI_MARIO_BIG_WALKING_RIGHT;
			else if (marioStateSmoke) ani = ANI_MARIO_UP_LEVEL_BIG_TO_TAIL_SMOKE;
			else if (isStateSitDown) ani = ANI_MARIO_BIG_SITDOWN_RIGHT;
			else if (isJump == 1) ani = ANI_MARIO_BIG_JUMP_RIGHT;
			else if (isKick) ani = ANI_MARIO_BIG_KICK_RIGHT;
			else if (marioStateTorToiSeShell) ani = ANI_MARIO_BIG_IDLE_TORTOISESHELL_RIGHT;
			else ani = ANI_MARIO_BIG_IDLE_RIGHT;
		} 
		else {
			if (autoChangeAni) ani = ANI_MARIO_BIG_TURN_RIGHT;
			else if (marioStateSmoke) ani = ANI_MARIO_UP_LEVEL_BIG_TO_TAIL_SMOKE;
			else if (isStateSitDown) ani = ANI_MARIO_BIG_SITDOWN_LEFT;
			else if (isJump == 1) ani = ANI_MARIO_BIG_JUMP_LEFT;
			else if (isKick) ani = ANI_MARIO_BIG_KICK_LEFT;
			else if (marioStateTorToiSeShell) ani = ANI_MARIO_BIG_IDLE_TORTOISESHELL_LEFT;
			else ani = ANI_MARIO_BIG_IDLE_LEFT;
		}
	}
	else if (vx > 0){
		if (marioStateSmoke) ani = ANI_MARIO_UP_LEVEL_BIG_TO_TAIL_SMOKE;
		else if (marioStateMaxPower) ani = ANI_MARIO_BIG_MAX_RUNNING_RIGHT;
		else if (isTurn && !isRunning) ani =  ANI_MARIO_BIG_TURN_RIGHT;
		else if (isJump == 1) ani = ANI_MARIO_BIG_JUMP_RIGHT;
		else if(isRunning) ani = ANI_MARIO_BIG_RUNNING_RIGHT;
		else if (isKick) ani = ANI_MARIO_BIG_KICK_RIGHT;
		else if (marioStateTorToiSeShell) ani = ANI_MARIO_BIG_WALKING_TORTOISESHELL_RIGHT;
		else ani = ANI_MARIO_BIG_WALKING_RIGHT; 
	} 
	else {
		if (marioStateSmoke) ani = ANI_MARIO_UP_LEVEL_BIG_TO_TAIL_SMOKE;
		else if (marioStateMaxPower) ani = ANI_MARIO_BIG_MAX_RUNNING_LEFT;
		else if (isTurn && !isRunning) ani = ANI_MARIO_BIG_TURN_LEFT;
		else if (isJump == 1 ) ani = ANI_MARIO_BIG_JUMP_LEFT;
		else if(isRunning) ani = ANI_MARIO_BIG_RUNNING_LEFT;
		else if (isKick) ani = ANI_MARIO_BIG_KICK_LEFT;
		else if (marioStateTorToiSeShell) ani = ANI_MARIO_BIG_WALKING_TORTOISESHELL_LEFT;
		else ani = ANI_MARIO_BIG_WALKING_LEFT; 
	}
	
	return ani;
}

int CMario::RenderLevelMarioTail() {
	if (vx == 0)
	{
		// right
		if (nx > 0) {
			if (autoChangeAni) ani = ANI_MARIO_TAIL_WALKING_RIGHT;
			else if (marioStateFlyHigh) ani = ANI_MARIO_TAIL_FLY_HIGH_RIGHT;
			else if (isStateSitDown) ani = ANI_MARIO_TAIL_SITDOWN_RIGHT;
			else if (isJump == 1) ani = ANI_MARIO_TAIL_JUMP_RIGHT;
			else if (isJumpFlyLow == 1) ani = ANI_MARIO_TAIL_FLY_LOW_RIGHT;
			else if (marioStateFight) ani = ANI_MARIO_TAIL_FIGHT;
			else if (isKick) ani = ANI_MARIO_TAIL_KICK_RIGHT;
			else if (marioStateTorToiSeShell) ani = ANI_MARIO_TAIL_IDLE_TORTOISESHELL_RIGHT;
			else ani = ANI_MARIO_TAIL_IDLE_RIGHT;	
		} 
		else {
			if (autoChangeAni) ani = ANI_MARIO_TAIL_TURN_RIGHT;
			else if (marioStateFlyHigh) ani = ANI_MARIO_TAIL_FLY_HIGH_LEFT;
			else if (isStateSitDown) ani = ANI_MARIO_TAIL_SITDOWN_LEFT;
			else if (isJump == 1) ani = ANI_MARIO_TAIL_JUMP_LEFT;
			else if (isJumpFlyLow == 1) ani = ANI_MARIO_TAIL_FLY_LOW_LEFT;
			else if (marioStateFight) ani = ANI_MARIO_TAIL_FIGHT;
			else if (isKick) ani = ANI_MARIO_TAIL_KICK_LEFT;
			else if (marioStateTorToiSeShell) ani = ANI_MARIO_TAIL_IDLE_TORTOISESHELL_LEFT;
			else ani = ANI_MARIO_TAIL_IDLE_LEFT;
		}
	}
	else if (vx > 0){
		 if (marioStateFlyHigh) ani = ANI_MARIO_TAIL_FLY_HIGH_RIGHT;
		else if (marioStateFight) ani = ANI_MARIO_TAIL_FIGHT; 
		else if (isJumpFlyLow == 1) ani = ANI_MARIO_TAIL_FLY_LOW_RIGHT;
		else if (marioStateMaxPower) ani = ANI_MARIO_TAIL_MAX_RUNNING_RIGHT;
		else if (isJump == 1 && marioStateMaxPower) ani = ANI_MARIO_TAIL_FLY_HEIGHT_RIGHT;
		else if (isJump == 1) ani = ANI_MARIO_TAIL_JUMP_RIGHT;
		else if(isRunning) ani = ANI_MARIO_TAIL_RUNNING_RIGHT;
		else if (isKick) ani = ANI_MARIO_TAIL_KICK_RIGHT;
		else if (marioStateTorToiSeShell) ani = ANI_MARIO_TAIL_WALKING_TORTOISESHELL_RIGHT;
		else if (isTurn && !isRunning) ani = ANI_MARIO_TAIL_TURN_RIGHT;
		else ani = ANI_MARIO_TAIL_WALKING_RIGHT; 
	} 
	else {
		if (marioStateFlyHigh) ani = ANI_MARIO_TAIL_FLY_HIGH_LEFT;
		else if (marioStateFight) ani = ANI_MARIO_TAIL_FIGHT;
		else if (isJumpFlyLow == 1) ani = ANI_MARIO_TAIL_FLY_LOW_LEFT;
		else if (marioStateMaxPower) ani = ANI_MARIO_TAIL_MAX_RUNNING_LEFT;
		else if (isJump == 1 && marioStateMaxPower) ani = ANI_MARIO_TAIL_FLY_HEIGHT_LEFT;
		else if (isJump == 1 ) ani = ANI_MARIO_TAIL_JUMP_LEFT;
		else if(isRunning) ani = ANI_MARIO_TAIL_RUNNING_LEFT;
		else if (isKick) ani = ANI_MARIO_TAIL_KICK_LEFT;
		else if (marioStateTorToiSeShell) ani = ANI_MARIO_TAIL_WALKING_TORTOISESHELL_LEFT;
		else if (isTurn && !isRunning) ani = ANI_MARIO_TAIL_TURN_LEFT;
		else ani = ANI_MARIO_TAIL_WALKING_LEFT; 
	}
	return ani;
}

int CMario::RenderLevelMarioFire() {
	if (vx == 0)
	{
		if (nx > 0) {
			if (autoChangeAni) ani = 14;
			else if (marioStateShootFire) ani = 91;
			else if (isStateSitDown) ani = 89;
			else if (isJump == 1) ani = 30;
			else if (isKick) ani = 38;
			else if (marioStateTorToiSeShell) ani = 58;
			else ani = 12; 
		} 
		else {
			if (autoChangeAni) ani = 70;
			else if (marioStateShootFire) ani = 92;
			else if (isStateSitDown) ani = 90;
			else if (isJump == 1) ani = 31;
			else if (isKick) ani = 39;
			else if (marioStateTorToiSeShell) ani = 59;
			else ani = 13;
		}
	}
	else if (vx > 0){
		if (marioStateShootFire) ani = 91;
		else if (isTurn && !isRunning) ani =  70;
		else if (isJump == 1) ani = 30;
		else if (isKick) ani = 38;
		else if (marioStateMaxPower) ani = 79;
		else if (isRunning) ani = 22;
		else if (marioStateTorToiSeShell) ani = 60;
		else ani = 14; 
	} 
	else {
		if (marioStateShootFire) ani = 92;
		else if (isTurn && !isRunning) ani = 71;
		else if (isJump == 1 ) ani = 31;
		else if (isKick) ani = 39;
		else if (marioStateMaxPower) ani = 80;
		else if(isRunning) ani = 23;
		else if (marioStateTorToiSeShell) ani = 61;
		else ani = 15; 
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
	return (width);
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
	return (height);
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
		plustortoiseshellInMario(MARIO_TORTOISESHELL_PLUS_10, MARIO_TORTOISESHELL_PLUS_10);
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

void CMario::CreateListScore(CListScore* s)
{
	for (int i = 0; i < 3; i++)
	{
		if (listScore[i] == NULL) {
			listScore[i] = s;
			break;
		}
		else if (listScore[i]->GetState() != SCORE_STATE_HIDEN)
		{
			listScore[i]->SetState(SCORE_STATE_HIDEN);
		}
	}
}

void CMario::DisplayListScore(int typeNumber, float posX, float posY, DWORD time)
{
	for (int i = 0; i < 3; i++)
	{
		if (listScore[i]->GetState() == SCORE_STATE_HIDEN)
		{
			listScore[i]->DisplayScores(typeNumber, posX, posY, time);
			break;
		}
	}
}

void CMario::CreateTail(CTail* t)
{
	if (tail != NULL) return;

	tail = t;
}

void CMario::ShipCollision()
{
	if (untouchable == 0) return;
	float marioX, marioY;
	GetPosition(marioX, marioY);
	SetPosition(marioX, marioY );

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

void CMario::handlerMarioUpLevelOtherSmall()
{
	if (!marioStateUpLevel ) {
		return;
	}
	if (GetTickCount64() - timeMarioUpLevel < MARIO_UP_LEVEL_TIME)
	{
		return;
	}
	SetState(STATE_MARIO_IDLE);
	marioStateUpLevel = false;

	SetLevel(LEVEL_MARIO_BIG);
}

void CMario::handlerMarioUpLevelSmoke()
{
	if (!marioStateSmoke) return;

	if (GetTickCount64() - timeMarioSmoke < MARIO_TIME_UP_LEVEL_SMOLE) return;
	DebugOut(L"okkk \n");

	marioStateSmoke = false;
	if (level == LEVEL_MARIO_TAIL) {
		//DebugOut(L"111 \n");
		SetLevel(LEVEL_MARIO_BIG);
	}
	else if (level == LEVEL_MARIO_BIG) {
	//	DebugOut(L"333 \n");
		SetLevel(LEVEL_MARIO_TAIL);
	}
	else if (level == LEVEL_MARIO_TAIL) {
	//	DebugOut(L"222 \n");
		SetLevel(LEVEL_MARIO_FIRE);
	}
	SetState(STATE_MARIO_IDLE);
}

void CMario::handlerMarioFight()
{
	if (GetTickCount64() - timeMarioFight < MARIO_FIGHT_TIME) {
		// SetState(STATE_MARIO_FIGHT);
		marioStateFight = true;
		return;
	}
	marioStateFight = false;
}

void CMario::handlerMarioDownPipe()
{
	if (!marioStatePipeDown) return;
	
	if (GetTickCount64() - timeMarioPipeDown < MARIO_PIPE_DOWN_UP_TIME)
	{
		SetState(STATE_MARIO_PIPE_DOWN);
		y += MARIO_PIPE_VY_DOWN;
	}
	else
	{
		marioStatePipeDown = false;
		SetState(STATE_MARIO_IDLE);
		int sceneID =	CGame::GetInstance()->GetScene();
		if (sceneID == SCENE_1) {
			SetPosition(MARIO_PIPE_DOWN_POS_X_4, MARIO_PIPE_DOWN_POS_Y_4);
		}
		if (sceneID == SCENE_3) {
			isMarioScene3Top = false;
			SetPosition(MARIO_PIPE_DOWN_POS_X_SCENE_3, MARIO_PIPE_DOWN_POS_Y_SCENE_3);
		}
	}
}

void CMario::handlerMarioUpPipe()
{
	if (!marioStatePipeUp) return;
		
	if (GetTickCount64() - timeMarioPipeUp < MARIO_PIPE_DOWN_UP_TIME)
	{
		SetState(STATE_MARIO_PIPE_UP);
		y -= MARIO_PIPE_VY_UP;
	}
	else if (GetTickCount64() - timeMarioPipeUp < MARIO_PIPE_DOWN_1_TIME) {
		SetPosition(MARIO_PIPE_POS_X_4, MARIO_PIPE_POS_Y_4);
		checkUpdateUpdatePipe = true;
		DebugOut(L"set  position mario up pipe NO set \n");
	}
	else if (GetTickCount64() - timeMarioPipeUp < MARIO_PIPE_DOWN_2_TIME)
	{
		SetState(STATE_MARIO_PIPE_UP);
		y -= MARIO_PIPE_VY_UP;
	}
	else
	{
		if (!checkUpdateUpdatePipe) {
			DebugOut(L"set  position mario up pipe \n");
			SetPosition(MARIO_PIPE_POS_X_4, MARIO_PIPE_POS_Y_4+20);
		}
		marioStatePipeUp = false;
		SetState(STATE_MARIO_IDLE);
	}
}


void CMario::handleMarioDead()
{
	int sceneID = CGame::GetInstance()->GetScene();
	if (sceneID == SCENE_1) {
		if (y > POSITION_Y_SCENE_1_MARIO_DEAD && state != STATE_MARIO_PIPE_UP && state != STATE_MARIO_PIPE_DOWN)
		{
			if (y > POSITION_Y_SCENE_1_OVER) {
				marioStateDie = true;
			}
			if (x > POSITION_X_SCENE_1_MARIO_DEAD && y < 496) {
					marioStateDie = true;
			}
			else if (x <= POSITION_X_SCENE_1_MARIO_DEAD) {
					marioStateDie = true;
			}
		}
	}
	else {
		// scene 3
		if (y > POSITION_Y_SCENE_3_MARIO_DEAD) {
			marioStateDie = true;
		}
	}
}

void CMario::handlerMarioShootFire()
{
	if (GetTickCount64() - timeMarioShootFire < MARIO_TIME_SHOOT_FIRE)
	{
		if (nx > 0) SetState(STATE_MARIO_SHOOT_FIRE_BULLET_RIGHT);
		else SetState(STATE_MARIO_SHOOT_FIRE_BULLET_LEFT);
	}
	else
	{
		marioStateShootFire = false;
		CreateFireBullet(NULL);
	}
}

void CMario::CollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopas* koopa = dynamic_cast<CKoopas*>(e->obj);
	if (koopa->GetState() == KOOPAS_STATE_TAKEN) return;
	if (e->ny > 0) {
		y = y - MARIO_DEFECT_Y_COLLISION;
	}
	if (marioStateFight &&
		(koopa->GetState() != KOOPAS_STATE_TORTOISESHELL_DOWN ||
		koopa->GetState() != KOOPAS_STATE_TORTOISESHELL_UP)
		)
	{
		koopa->SetState(KOOPAS_STATE_TORTOISESHELL_UP);
		koopa->SetDefectStart((DWORD)GetTickCount64());
		koopa->SetStateDefect(true);
	}
	// collision tren koopas
	else if (e->ny < 0) {
		if (koopa->GetState() != KOOPAS_STATE_TORTOISESHELL_DOWN &&
			koopa->GetState() != KOOPAS_STATE_TORTOISESHELL_UP)
		{
			if (koopa->GetState() == KOOPAS_STATE_SPIN_RIGHT || koopa->GetState() == KOOPAS_STATE_SPIN_LEFT) {
				//float koopaX, koopaY;
				//koopa->GetPosition(koopaX, koopaY);
				//koopaY = koopaY - 100.0f;
				//	// APPEND_KOOPAS_STATE_SPIN_TO_TORTOISESHELL;
				//koopa->GetPosition(koopaX, koopaY);
				koopa->SetState(KOOPAS_STATE_TORTOISESHELL_DOWN);
			}
			else {
				DisplayListScore(MARIO_SCORE_100, koopa->x, koopa->y, (DWORD)GetTickCount64());
				if (koopa->GetTypeKoopa() == PARAKOOPA_COLOR_GREEN) {
					koopa->SetTypeKoopa(KOOPA_GREEN_FORM);
				}

				else if (koopa->getIsDown()) {
					koopa->SetState(KOOPAS_STATE_TORTOISESHELL_DOWN);
				}
				else {
					koopa->SetState(KOOPAS_STATE_TORTOISESHELL_UP);
				}
			}

			 vy = -0.2f;
			// vy = -MARIO_JUMP_SPEED_Y;
		}
		else if (koopa->GetState() == KOOPAS_STATE_TORTOISESHELL_DOWN || koopa->GetState() == KOOPAS_STATE_TORTOISESHELL_UP) {
			if ((x + round(GetBBoxWidthMario() + 1)) <= (koopa->x + round(KOOPAS_BBOX_WIDTH / 2))) {
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
			if (koopa->GetState() != KOOPAS_STATE_TORTOISESHELL_DOWN &&
				koopa->GetState() != KOOPAS_STATE_TORTOISESHELL_UP && !marioStateFight)
			{
				// ShipCollision();
				if (level > LEVEL_MARIO_SMAIL) {
					SetMarioLevel(GetMarioLevel() - 1);
					StartUntouchable();
				}
				else {
					SetState(STATE_MARIO_DIE);
				}
			}
			else {
				if (state != STATE_MARIO_RUNNING_RIGHT && state != STATE_MARIO_RUNNING_LEFT) {
					DisplayListScore(MARIO_SCORE_200, koopa->x, koopa->y, (DWORD)GetTickCount64());
					timeStartKick = (DWORD)GetTickCount64();
					y = y - 2;
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
				else if (!marioStateFight) {
					tortoiseshell = koopa;
					koopa->SetState(KOOPAS_STATE_TAKEN);
					if (e->nx < 0) {
						SetState(STATE_MARIO_TORTOISESHELL_RIGHT);
					}
					else SetState(STATE_MARIO_TORTOISESHELL_LEFT);
				}
			}
		}
	}
}

void CMario::CollisionWithPipe(LPCOLLISIONEVENT e)
{
	CPipe* pipe = dynamic_cast<CPipe*>(e->obj);
	if (e->ny != 0 && isStateSitDown && pipe->GetType() == PIPE_STATE_UP_DOWN) {
		if (!marioStatePipeDown) {
			SetState(STATE_MARIO_PIPE_DOWN);
			timeMarioPipeDown = (DWORD)GetTickCount64();
		}
	}
	if (e->ny != 0 && pipe->GetType() == PIPE_STATE_DOWN_UP)
	{
		if (!marioStatePipeUp)
		{
			SetState(STATE_MARIO_PIPE_UP);
			timeMarioPipeUp = (DWORD)GetTickCount64();
		}
	}
}

void CMario::CollisionWithBoomerang(LPCOLLISIONEVENT e)
{
	CBoomerang* boomerang = dynamic_cast<CBoomerang*>(e->obj);
	if (boomerang->GetState() != STATE_BOOMERANG_HIDEN)
	{
		if (e->ny > 0) {
			y = y - MARIO_DEFECT_Y_COLLISION;
		}
		if (level > LEVEL_MARIO_SMAIL)
		{
			SetMarioLevel(GetMarioLevel() - 1);
			StartUntouchable();
		}
		else SetState(STATE_MARIO_DIE);
	}
}

void CMario::CollisionWithBoomerangBros(LPCOLLISIONEVENT e)
{
	CBoomerangBro* boomerangbro = dynamic_cast<CBoomerangBro*>(e->obj);
	// top of boomerangbro
	if (e->ny < 0)
	{
		if (boomerangbro->GetState() != STATE_BOOMERANGBRO_DIE)
		{
			DisplayListScore(MARIO_SCORE_100, boomerangbro->x, boomerangbro->y, (DWORD)GetTickCount64());
			boomerangbro->SetState(STATE_BOOMERANGBRO_DIE);
			boomerangbro->SetBoomerangTimeDead((DWORD)GetTickCount64());
			vy = -SPEED_MARIO_JUMP_DEFLECT;
		}
	}

	else if (e->nx != 0)
	{
		if (untouchable == 0)
		{
			if (boomerangbro->GetState() != STATE_BOOMERANGBRO_DIE)
			{
				if (level > LEVEL_MARIO_SMAIL && !marioStateFight)
				{
					SetMarioLevel(GetMarioLevel() - 1);
					StartUntouchable();
				}
				else if (marioStateFight)
				{
					boomerangbro->SetState(STATE_BOOMERANGBRO_DIE);
					boomerangbro->SetBoomerangTimeDead((DWORD)GetTickCount64());
				}
				else
					SetState(STATE_MARIO_DIE);
			}
		}
	}
}

void CMario::CollisionWithWoodBlock(LPCOLLISIONEVENT e)
{
	CWoodBlock* woodBlock = dynamic_cast<CWoodBlock*>(e->obj);
	if (e->nx != 0 && woodBlock->GetWoodBlockType() == 1) {
		woodBlock->SetWoodBlockActive(true);
		float woodBlockX, woodBlockY;
		woodBlock->GetPosition(woodBlockX, woodBlockY);
		if (woodBlockX > x) {
			woodBlock->SetState(WOOD_BLOCK_MOVING_RIGHT);
			x = x - MARIO_ADD_POSITION_X;
		}
		else {
			x = x + MARIO_ADD_POSITION_X;
			woodBlock->SetState(WOOD_BLOCK_MOVING_LEFT);
		}

	}
}

void CMario::CollisionWithPortal(LPCOLLISIONEVENT e)
{

	CPortal* portal = dynamic_cast<CPortal*>(e->obj);
	/*if (e->ny < 0) {
	DebugOut(L"marrio collisoin Portal e->ny \n");
		y += dy;
	}
	if (e->nx != 0) {
		x += dx;
		DebugOut(L"marrio collisoin Portal e->nx != 0 \n");

	}*/
}

void CMario::CollisionWithBrick(LPCOLLISIONEVENT e)
{
	marioSpeechJump = 0.0f;
	CBrick* brick = dynamic_cast<CBrick*>(e->obj);
	if (brick->GetState() == BRICK_STATE_NEED_CRETE) return;

	if (nx != 0)
	{
		if (brick->GetState() == BRICK_STATE_NORMAL && marioStateFight)
		{
			//	brick->SetState(BRICK_STATE_HIDEN);
		}
	}
	if (e->ny < 0) {
		isMarioNotJump = false;
	}
	if (e->ny > 0) {
		if (brick->GetState() == BRICK_STATE_FLY_DOWN) {
			float marioX, marioY;
			GetPosition(marioX, marioY);
			SetPosition(marioX, marioY-1);
		}
	//	if (brick->GetBrickType() == BRICK_TYPE_HAVE_ITEM) {
			brick->SetState(BRICK_STATE_310);
		//}
		//else {
		//	if (level == LEVEL_MARIO_SMAIL) {
		//		brick->SetState(BRICK_STATE_FLY_UP);
		//		brick->SetTimeBack((DWORD)GetTickCount64());

		//		isMarioNotJump = true;
		//	}
		//	else {
		//		float mariovx, mariovy;
		//		GetSpeed(mariovx, mariovy);
		//		SetSpeed(mariovx, 0.01f);
		//		brick->SetState(BRICK_STATE_HIDEN);
		//		float mariovssx, zzz;
		//		//	float brickX, brickY;
		//		// brick->GetPosition(brickX, brickY)

		//		((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->CreatePieceBrick(x, y, (DWORD)GetTickCount64());
		//	}
		//}
	}
}

void CMario::CollisionWithMusic(LPCOLLISIONEVENT e)
{
	CMusic* music = dynamic_cast<CMusic*>(e->obj);
	if (music->GetTypeMusic() == MUSIC_ANI_WHITE) {
		if (e->ny < 0) {
			vy = -SPEED_MARIO_MUSIC_Y;
			music->SetState(MUSIC_DOWN_STATE);
			if (nx > 0) vx = SPEED_MARIO_MUSIC_X;
			if (nx < 0) vx = -SPEED_MARIO_MUSIC_X;
		}
	}
	if (music->GetTypeMusic() == MUSIC_ANI_RED) {
		if (music->GetState() == MUSIC_HIDEN_STATE){
			x += dx;
			y += dy;
		}
		if (e->ny > 0) {
			music->SetState(MUSIC_APPEAR_STATE);
		}
		if (music->GetState() != MUSIC_HIDEN_STATE) {
			if (e->ny < 0) {
				vy = -SPEED_MARIO_MUSIC_Y;
				music->SetState(MUSIC_DOWN_STATE);
				if (nx > 0) vx = SPEED_MARIO_MUSIC_X;
				if (nx < 0) vx = -SPEED_MARIO_MUSIC_X;
				timeMarioAcceptScene3_1 = (DWORD)GetTickCount64();
			}
		}
	}
}

void CMario::CollisionWithMushroom(LPCOLLISIONEVENT e)
{
	CMushroom* mushroom = dynamic_cast<CMushroom*>(e->obj);
	if (mushroom->GetState() == MUSHROOM_STATE_HIDEN) return;
	if (mushroom->GetMushroomType() == MUSHROOM_TYPE_RED) {
		mushroom->SetState(MUSHROOM_STATE_HIDEN);
		if (level == LEVEL_MARIO_SMAIL) {
			SetPosition(x, y - UP_DOWN_POSITOIN_Y);
			SetState(STATE_MARIO_UP_LEVEL);
			timeMarioUpLevel = (DWORD)GetTickCount64();
		}
		if (level == LEVEL_MARIO_BIG) {
			SetState(STATE_MARIO_SMOKE);
			SetPosition(x, y - 2);
			timeMarioSmoke = (DWORD)GetTickCount64();
		}
		DisplayListScore(MARIO_SCORE_1000, mushroom->x, mushroom->y, (DWORD)GetTickCount64());
	}
	if (mushroom->GetMushroomType() == MUSHROOM_TYPE_GREEN) {
		mushroom->SetState(MUSHROOM_STATE_HIDEN);
		DisplayListScore(MARIO_SCORE_1000, mushroom->x, mushroom->y, (DWORD)GetTickCount64());
		if (mushroom->GetUpVip() != NULL) {
			mushroom->GetUpVip()->SetState(STATE_UP_VIP_ITEM_FLY);
			mushroom->GetUpVip()->SetTimePreHide((DWORD)GetTickCount64());
		}
		else {
		}
	}
}

void CMario::CollisionWithGoombaMini(LPCOLLISIONEVENT e)
{
	CGoombaMini* goombaMini = dynamic_cast<CGoombaMini*>(e->obj);
	if (marioStateFight) {
		goombaMini->SetState(STATE_GOOMBA_MINI_HIDEN);
		return;
	}
	if (!goombaMini->GetCheckCollisionMario()) {
		goombaMini->SetState(STATE_GOOMBA_MINI_MOVING_IN_MARIO);
	}
}

void CMario::CollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	if (e->ny > 0) {
		y = y - MARIO_DEFECT_Y_COLLISION;
	}
	if (e->ny < 0) {
		if (goomba->GetState() != GOOMBA_STATE_DIE) {
			DisplayListScore(MARIO_SCORE_100, goomba->x, goomba->y, (DWORD)GetTickCount64());

			if (goomba->getColorGoomba() == GOOMBA_YELLOW_COLOR) {
				goomba->SetState(GOOMBA_STATE_DIE);
			}

			if (goomba->getColorGoomba() == GOOMBA_YELLOW_COLOR_FLY) goomba->setColorGoomba(GOOMBA_YELLOW_COLOR);

			if (goomba->getColorGoomba() == PARA_GOOMBA_BROWN) {

				// if(marioStateFight) goomba->SetState(GOOMBA_STATE_DIE);

				if (goomba->GetState() == GOOMBA_STATE_BROWN_WALKING) {
					goomba->SetState(GOOMBA_STATE_DIE);
				}
				else {
					goomba->SetState(GOOMBA_STATE_BROWN_WALKING);
				}
			}
			vy = -MARIO_JUMP_SPEED_Y;
			isJump = 1;
		}

	}
	else if (e->nx != 0 || e->ny > 0) {
	
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			if (level > LEVEL_MARIO_SMAIL && !marioStateFight)
			{
				ShipCollision();
				SetMarioLevel(GetMarioLevel() - 1);
				StartUntouchable();
			}
			/*else if (marioStateFight) {
				goomba->SetState(GOOMBA_STATE_DEFLECT);
			}*/
			else if (level == LEVEL_MARIO_SMAIL){
				SetState(STATE_MARIO_DIE);
			}
		}
	}
	 if (e->ny > 0) 	ShipCollision();
}

void CMario::MarioHanlerProcessArrow()
{
	 if (state == STATE_MARIO_RUNNING_RIGHT ||
		state == STATE_MARIO_RUNNING_LEFT || 
		state == STATE_MARIO_RUNNING_FAST_LEFT || 
		state == STATE_MARIO_RUNNING_FAST_RIGHT)
	{
		if (GetTickCount64() - timeRunningStart > MARIO_TIME_RUNNING) {
			listArrow->SetWhiteListArrow();
			timeRunningStart = (DWORD)GetTickCount64();
		}
	}
	else {
		if (GetTickCount64() - timeRunningStart > MARIO_TIME_RUNNING) {
			listArrow->SetBlackListArrow();
			timeRunningStart = (DWORD)GetTickCount64();
		}
	}
}

bool CMario::checkMarioMaxPower()
{
	if (listArrow->GetPState()) {
		marioStateMaxPower = false;
		return false;
	}
	marioStateMaxPower = true;
	return true;
}


void CMario::handleMarioFlyHigh()
{
	if (GetTickCount64() - timeMarioFlyHigh < MARIO_FLY_HIGH_TIME)
	{
		if (nx > 0) SetState(STATE_MARIO_FLYING_HIGH_RIGHT);
		else SetState(STATE_MARIO_FLYING_HIGH_LEFT);
		isAcceptFlyCamera = true;
	}
	else marioStateFlyHigh = 0;
}
void CMario::handleMarioDeadFly()
{
	if (!marioStateDie) return;
	vx = 0;
	if (GetTickCount64() - timeMarioDead < MARIO_TIME_DEAD_TOP) {
		vy = -MARIO_SPEED_DEAD;
	}
	else if (GetTickCount64() - timeMarioDead < MARIO_TIME_DEAD_BOTTOM) {
		vy = MARIO_SPEED_DEAD;
	}
	else {
		 isMarioAcceptSwitchScene = true;
	}
}
void CMario::upLevelMario()
{
	if (level == LEVEL_MARIO_SMAIL) {
		SetLevel(LEVEL_MARIO_BIG);
	}
	else if (level == LEVEL_MARIO_BIG) {
		SetLevel(LEVEL_MARIO_TAIL);
	}
	else if (level == LEVEL_MARIO_BIG) {
		SetLevel(LEVEL_MARIO_TAIL);
	}
}


void CMario::ShootFireBullet()
{
	for (int i = 0; i < FIRE_BULLET_AMOUNT; i++)
	{
		if (fireBullet[i]->GetState() != FIRE_BULLET_SHOOTED_RIGHT_STATE &&
			fireBullet[i]->GetState() != FIRE_BULLET_SHOOTED_LEFT_STATE)
		{
			if (nx > 0)
			{
				fireBullet[i]->SetPosition(x + 14, y);
				fireBullet[i]->SetState(FIRE_BULLET_SHOOTED_RIGHT_STATE);
			}
			else
			{
				fireBullet[i]->SetPosition(x, y);
				fireBullet[i]->SetState(FIRE_BULLET_SHOOTED_LEFT_STATE);
			}
			break;
		}
	}
}

void CMario::CreateFireBullet(CGameObject* fire)
{
	for (int i = 0; i < FIRE_BULLET_AMOUNT; i++)
	{
		if (fireBullet[i] == NULL)
		{
			fireBullet[i] = fire;
			break;
		}
		else if (fireBullet[i]->GetState() == FIRE_BULLET_DESTROY_STATE)
		{
			fireBullet[i]->SetState(FIRE_BULLET_TRANSPARENT_STATE);
		}
	}
}