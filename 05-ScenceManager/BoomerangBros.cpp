#include "BoomerangBros.h"
#include "ColorBrick.h"

CBoomerangBro::CBoomerangBro(CBoomerang* weapon[BOOMERANG_AMOUNT])
{
	timeBoomerangBrosStart = (DWORD)GetTickCount64();
	for (int i = 0; i < BOOMERANG_AMOUNT; i++)
	{
		boomerang[i] = weapon[i];
	}
	timeBoomerangBrosDead = (DWORD)0.0f;
	timeBoomerangBrosStart = (DWORD)0.0f;
	foundMario = false;
	isBoomerangBrosIdle = false;
	isBoomerangBrosBack = false;
	isBoomerangBrosShoot = false;
	isBoomerangBrosGoBack = false;
	isBoomerangBrosRight = false;
}


void CBoomerangBro::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CGameObject::Update(dt);
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	vy += BOOMERANGBRO_GRAVITY * dt;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

	}
	else if (state != STATE_BOOMERANGBRO_DIE)
	{
		float min_tx, min_ty, nx, ny;
		float rdx = 0;
		float rdy = 0;
		CBoomerangBro::FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		x += min_tx * dx + nx * 0.1f;
		y += min_ty * dy + ny * 0.1f;
		if (ny != 0) vy = 0;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
		}
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];


		if (GetTickCount64() - timeBoomerangBrosStart < BOOMERANGBRO_SHOOT_TIME) {
			isBoomerangBrosGoBack = false;
			// mario bên phải của boomerangbros
			if (mario->x - x >= BOOMERANGBRO_RANGE_ATTACK_PLAYER_RIGHT)
			{
				SetState(STATE_BOOMERANGBRO_WALKING);
				isBoomerangBrosRight = true;
				vx = BOOMERANG_WALKING_SPEED;
			}
			// mario bên phải của boomerangbros
			else if (mario->x - x <= BOOMERANGBRO_RANGE_ATTACK_PLAYER_LEFT)
			{
				isBoomerangBrosRight = false;
				SetState(STATE_BOOMERANGBRO_WALKING);
				vx = -BOOMERANG_WALKING_SPEED;
			}

			if (!isBoomerangBrosShoot) {
				for (int i = 0; i < BOOMERANG_AMOUNT; i++)
				{
					DebugOut(L"boomerang \n");
					if (boomerang[i]->GetState() == STATE_BOOMERANG_HIDEN)
					{
						DebugOut(L"boomerang  11 \n");
						isBoomerangBrosShoot = true;
						if (isBoomerangBrosRight)
						{
							boomerang[i]->SetPosition(x , y - BOOMERANGBRO_BOOMERANG_LEFT_Y);
							boomerang[i]->SetState(STATE_BOOMERANG_FLYING_RIGHT);
							boomerang[i]->SetShootStartBoomerang((DWORD)GetTickCount64());
							break;
						}
						else
						{
							boomerang[i]->SetPosition(x ,y - BOOMERANGBRO_BOOMERANG_RIGHT_Y);
							boomerang[i]->SetState(STATE_BOOMERANG_FLYING_LEFT);
							boomerang[i]->SetShootStartBoomerang((DWORD)GetTickCount64());
							break;
						}
					}
				}
			}
		}

		else if (GetTickCount64() - timeBoomerangBrosStart < BOOMERANGBRO_SHOOT_BACH_TIME) {
			isBoomerangBrosShoot = false;
			isBoomerangBrosGoBack = true;
			if (mario->x - x >= BOOMERANGBRO_RANGE_ATTACK_PLAYER_RIGHT)
			{
				SetState(STATE_BOOMERANGBRO_WALKING);
				isBoomerangBrosRight = true;
				vx = -BOOMERANG_WALKING_SPEED;
			}
			// mario bên phải của boomerangbros
			else if (mario->x - x <= BOOMERANGBRO_RANGE_ATTACK_PLAYER_LEFT)
			{
				isBoomerangBrosRight = false;
				SetState(STATE_BOOMERANGBRO_WALKING);
				vx = BOOMERANG_WALKING_SPEED;
			}
		}
		else if (GetTickCount64() - timeBoomerangBrosStart < BOOMERANGBRO_SHOOT_TWO_TIME) {
			// mario bên phải của boomerangbros 
			if (!isBoomerangBrosShoot) {
				for (int i = 0; i < BOOMERANG_AMOUNT; i++)
				{
					if (boomerang[i]->GetState() == STATE_BOOMERANG_HIDEN)
					{
						isBoomerangBrosShoot = true;
						if (isBoomerangBrosRight)
						{
							boomerang[i]->SetPosition(x, y - BOOMERANGBRO_BOOMERANG_LEFT_Y);
							boomerang[i]->SetState(STATE_BOOMERANG_FLYING_RIGHT);
							boomerang[i]->SetShootStartBoomerang((DWORD)GetTickCount64());
							break;
						}
						else
						{
							boomerang[i]->SetPosition(x, y - BOOMERANGBRO_BOOMERANG_RIGHT_Y);
							boomerang[i]->SetState(STATE_BOOMERANG_FLYING_LEFT);
							boomerang[i]->SetShootStartBoomerang((DWORD)GetTickCount64());
							break;
						}
					}
				}
			}
		}
		// set biến check để được bắn lại
		else if (GetTickCount64() - timeBoomerangBrosStart < BOOMERANGBRO_START_SHOOT_TIME) {
			vx = 0;
			vy = 0;
			isBoomerangBrosShoot = false;
			timeBoomerangBrosStart = (DWORD)GetTickCount64();
		}
		else {
			timeBoomerangBrosStart = (DWORD)GetTickCount64();
		}
	
	}
	handleDead();
}

void CBoomerangBro::Render()
{
	// ani: 0 = walking left, 1 =  walking right, 2 = idle left, 3 = idle right, 4 = dead left, 5 = dead right
	int ani = ANI_BOOMERANGBRO_WALKING_RIGHT;
	if (state == STATE_BOOMERANGBRO_DIE) {
		 ani = ANI_BOOMERANGBRO_DIE_LEFT;
		 if (nx < 0) ani = ANI_BOOMERANGBRO_DIE_RIGHT;
	}

	else if ( vx > 0 && state == STATE_BOOMERANGBRO_WALKING ) {
		ani = ANI_BOOMERANGBRO_WALKING_RIGHT;
		if (isBoomerangBrosGoBack) {

		ani = ANI_BOOMERANGBRO_WALKING_LEFT;
		}
	}
	else if (vx < 0 && state == STATE_BOOMERANGBRO_WALKING) {
			ani = ANI_BOOMERANGBRO_WALKING_LEFT;
		if (isBoomerangBrosGoBack) {
		ani = ANI_BOOMERANGBRO_WALKING_RIGHT;

		}
	}
	else if (!isBoomerangBrosRight) ani = ANI_BOOMERANGBRO_THROWING_LEFT;
	else if (isBoomerangBrosRight) ani = ANI_BOOMERANGBRO_THROWING_RIGHT;
	
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CBoomerangBro::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case STATE_BOOMERANGBRO_WALKING:
		isBoomerangBrosIdle = false;
		break;
	case STATE_BOOMERANGBRO_THROWING:
		isBoomerangBrosIdle = true;
		vx = 0;
		break;
	case STATE_BOOMERANGBRO_DIE:
		isBoomerangBrosIdle = true;
		break;
	default:
		break;
	}
}

void CBoomerangBro::FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy)
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
			// chỉ va chạm khi ở phía trên của ...
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
}

void CBoomerangBro::handleDead()
{
	if (state != STATE_BOOMERANGBRO_DIE) return;
	vx = 0;
	if (GetTickCount64() - timeBoomerangBrosDead < STATE_BOOMERANGBRO_DEAD) {
		vy = -BOOMERANGBRO_DEAD;
	}
	else {
		vy = BOOMERANGBRO_DEAD;
	}
}

void CBoomerangBro::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == STATE_BOOMERANGBRO_DIE) return;

	left = x;
	top = y;
	right = x + BOOMERANGBRO_BBOX_WIDTH;
	bottom = y + BOOMERANGBRO_BBOX_HEIGHT;
}
