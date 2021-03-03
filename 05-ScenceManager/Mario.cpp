#include <algorithm>
#include <assert.h>
#include "Utils.h"
#include "Mario.h"
#include "Game.h"
#include "Goomba.h"
#include "Portal.h"

CMario::CMario(float x, float y) : CGameObject()
{
	level = LEVEL_MARIO_BIG;
	untouchable = 0;
	SetState(STATE_MARIO_IDLE);

	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y; 
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += MARIO_GRAVITY*dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

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

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 
		
		// block every object first!
		x += min_tx*dx + nx*0.4f;
		y += min_ty*dy + ny*0.4f;

		if (nx!=0) vx = 0;
		if (ny!=0) vy = 0;


		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState()!= GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -SPEED_MARIO_JUMP_DEFLECT;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable==0)
					{
						if (goomba->GetState()!=GOOMBA_STATE_DIE)
						{
							if (level > LEVEL_MARIO_SMAIL)
							{
								level = LEVEL_MARIO_SMAIL;
								StartUntouchable();
							}
							else 
								SetState(STATE_MARIO_DIE);
						}
					}
				}
			} // if Goomba
			else if (dynamic_cast<CPortal *>(e->obj))
			{
				CPortal *p = dynamic_cast<CPortal *>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}



void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case STATE_MARIO_WALKING_RIGHT:
		isRunning = false;
		isJump = false;
		isTurn = false;
		vx = SPEED_MARIO_WALKING;
		nx = DIRECTION_MARIO_RIGHT;
		break;
	case STATE_MARIO_WALKING_LEFT: 
		isJump = false;
		isTurn = false;
		vx = -SPEED_MARIO_WALKING;
		nx = DIRECTION_MARIO_LEFT;
		break;
	case STATE_MARIO_RUNNING_RIGHT:
		isJump = false;
		isRunning = true;
		vx = SPEED_MARIO_RUNNING;
		nx = DIRECTION_MARIO_RIGHT;
		break;
	case STATE_MARIO_RUNNING_LEFT:
		isRunning = true;
		isJump = false;
		vx = -SPEED_MARIO_RUNNING;
		nx = DIRECTION_MARIO_LEFT;
		break;		
	case STATE_MARIO_SITDOWN:
		isStateSitDown = true;
		isJump = false;
		break;
	case STATE_MARIO_JUMP:
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
		isJump = true;
		vy = -MARIO_JUMP_SPEED_Y;
		break; 
	case STATE_MARIO_IDLE:
		isTurn = false;
		vx = 0;
		break;
	case STATE_MARIO_TURN_RIGHT:
		isTurn = true;
		isRunning = false;
		vx = SPEED_MARIO_WALKING;
		if (isRunning) vx = SPEED_MARIO_WALKING + 0.5f;
		break;
	case STATE_MARIO_TURN_LEFT:
		isTurn = true;
		isRunning = false;
		vx = -SPEED_MARIO_WALKING;
		if (isRunning) vx = -SPEED_MARIO_WALKING - 0.5f;
		break;
	case STATE_MARIO_DIE:
		vy = -SPEED_MARIO_DIE_DEFLECT;
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
		break;	
	case LEVEL_MARIO_FIRE:
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
		break;	
	default:
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
		break;
	}

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
			ani = ANI_MARIO_SMALL_IDLE_RIGHT;	
		} 
		else {
			ani = ANI_MARIO_SMALL_IDLE_LEFT;
		}
	}
	else if (vx > 0){
		if(isRunning) ani = ANI_MARIO_SMALL_RUN_RIGHT; 
		else ani = ANI_MARIO_SMALL_WALK_RIGHT; 
	} 
	else {
		if(isRunning) ani = ANI_MARIO_SMALL_RUN_LEFT;
		else ani = ANI_MARIO_SMALL_WALK_LEFT; 
	}
	
	return ani;
}

int CMario::RenderLevelMarioBig() {
	if (vx == 0)
	{
		if (nx > 0) {
			if (isStateSitDown) ani = ANI_MARIO_BIG_SIT_RIGHT;
			else if (isJump == 1 && !isStandingFloor || isJump == -1 && !isStandingFloor) ani = ANI_MARIO_BIG_JUMP_RIGHT;
			else ani = ANI_MARIO_BIG_IDLE_RIGHT;	
		} 
		else {
			if (isStateSitDown) ani = ANI_MARIO_BIG_SIT_LEFT;
			else if (isJump == 1 && !isStandingFloor || isJump == -1 && !isStandingFloor) ani = ANI_MARIO_BIG_JUMP_LEFT;
			else ani = ANI_MARIO_BIG_IDLE_LEFT;
		}
	}
	else if (vx > 0){
		if(isRunning) ani = ANI_MARIO_BIG_RUN_RIGHT; 
		else if (isTurn) ani = ANI_MARIO_BIG_TURN_LEFT;
		else if (isJump == 1 && !isStandingFloor || isJump == -1 && !isStandingFloor) ani = ANI_MARIO_BIG_JUMP_RIGHT;
		else ani = ANI_MARIO_BIG_WALK_RIGHT; 
	} 
	else {
		if(isRunning) ani = ANI_MARIO_BIG_RUN_LEFT;
		else if (isJump == 1 && !isStandingFloor || isJump == -1 && !isStandingFloor) ani = ANI_MARIO_BIG_JUMP_LEFT;
		else if (isTurn) ani = ANI_MARIO_BIG_TURN_RIGHT;
		else ani = ANI_MARIO_BIG_WALK_LEFT; 
	}
	
	return ani;
}

int CMario::RenderLevelMarioTail() {
	return 20;
}

int CMario::RenderLevelMarioFire() {
	return 30;
}

