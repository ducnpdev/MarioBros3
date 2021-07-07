#include "FirePiranhaPlant.h"
#include "Mario.h"
#include "FirePlantBullet.h"

CFirePiranhaPlant::CFirePiranhaPlant(CGameObject* mario)
{
	check_y_limit = false;
	found_player = false;
	SetState(FIREPIRANHAPLANT_STATE_HIDE);
	player = mario;
	y_limit = FIREPIRANHAPLANT_Y_LIMIT;
}

void CFirePiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	if (state == FIREPIRANHAPLANT_STATE_DESTROY) return;
	// accept đi xuống
	if (y >= y_limit) {
		y += dy;
	}
	else {
		y = y_limit;
	}

	sx = x - player->x;
	sy = y - player->y;
	tan = sx / sy;

	if (abs(x - player->x) < FIREPIRANHAPLANT_RANGE_EFFECT_MARIO)
	{
		if (state == FIREPIRANHAPLANT_STATE_HIDE && tan > 1.0f || state == FIREPIRANHAPLANT_STATE_HIDE && tan < -1.0f)
			found_player = true;
	}
	if (found_player)
	{
		if (stateHide)
		{
			// start move up
			stateMoveUp = true;
			timeMoveUp = (DWORD)GetTickCount64();
		}

		if (stateMoveUp)
		{
			if (GetTickCount64() - timeMoveUp < FIREPIRANHAPLANT_MOVE_UP_TIME)
				SetState(FIREPIRANHAPLANT_STATE_MOVE_UP);
			else SetState(FIREPIRANHAPLANT_STATE_APPEARANCE);
		}

		if (appearance_state)
		{
			if (GetTickCount64() - timeMoveUp < 3000)
				SetState(FIREPIRANHAPLANT_STATE_APPEARANCE);
			if (GetTickCount64() - timeMoveUp > FIREPIRANHAPLANT_SHOOT_TIME && firePlantBullet->GetState() == FIREPLANTBULLET_TRANSPARENT_STATE)
				ShootFirePlantBullet();
			if (GetTickCount64() - timeMoveUp > 3500)
				SetState(FIREPIRANHAPLANT_STATE_MOVE_DOWN);
		}
		if (move_down_state)
		{
			if (GetTickCount64() - timeMoveUp < 4420)
				SetState(FIREPIRANHAPLANT_STATE_MOVE_DOWN);
			else
			{
				SetState(FIREPIRANHAPLANT_STATE_HIDE);
				found_player = false;
			}
		}
	}
	// cao hơn mario
	if (y - player->y < 0)
	{
		// bên trái của mario
		if (x - player->x < 0)
		{
			nx = 1;
			down_right_state = 1;
			top_right_state = 0;
			down_left_state = 0;
			top_left_state = 0;
		}
		else
		{
			nx = 0;
			down_left_state = 1;
			top_left_state = 0;
			down_right_state = 0;
			top_right_state = 0;
		}
	}
	else
	{
		if ((x - player->x < 0))
		{
			nx = 1;
			down_right_state = 0;
			top_right_state = 1;
			down_left_state = 0;
			top_left_state = 0;
		}
		else
		{
			nx = 0;
			top_left_state = 1;
			down_left_state = 0;
			down_right_state = 0;
			top_right_state = 0;
		}
	}
}

void CFirePiranhaPlant::Render()
{
	if (stateHide) return;
	else if (state == FIREPIRANHAPLANT_STATE_DESTROY) return;
	else {
		int ani = FIREPIRANHAPLANT_ANI_DOWN_LEFT;
		if (down_left_state) ani = FIREPIRANHAPLANT_ANI_DOWN_LEFT;
		else if (down_right_state) ani = FIREPIRANHAPLANT_ANI_DOWN_RIGHT;
		else if (top_right_state) ani = FIREPIRANHAPLANT_ANI_TOP_RIGHT;
		else ani = FIREPIRANHAPLANT_ANI_TOP_LEFT;
		animation_set->at(ani)->Render(x, y);
	}
}

void CFirePiranhaPlant::ShootFirePlantBullet()
{
	if (firePlantBullet->GetState() != FIREPLANTBULLET_TRANSPARENT_STATE) return;
	// handler shoot bên phải
	if (nx > 0)
	{
		firePlantBullet->SetPosition(x + FIREPIRANHAPLANT_POSITION_RIGHT_X, y);
		if (tan > 0 && tan < FIREPIRANHAPLANT_TAN_ANGLE_ATTACK)
		{
			firePlantBullet->SetState(FIREPLANTBULLET_SHOOTED_RIGHT_30_STATE);
		}
		else if (tan > FIREPIRANHAPLANT_TAN_ANGLE_ATTACK)
		{
			firePlantBullet->SetState(FIREPLANTBULLET_SHOOTED_RIGHT_60_STATE);
		}
		else if (tan < 0 && tan > -FIREPIRANHAPLANT_TAN_ANGLE_ATTACK)
		{
			firePlantBullet->SetState(FIREPLANTBULLET_SHOOTED_RIGHT_120_STATE);
		}
		else
		{
			firePlantBullet->SetState(FIREPLANTBULLET_SHOOTED_RIGHT_150_STATE);
		}
		return;
	}
	// handler shoot bên trái

	firePlantBullet->SetPosition(x, y);
	if (tan > 0 && tan < FIREPIRANHAPLANT_TAN_ANGLE_ATTACK)
	{
		firePlantBullet->SetState(FIREPLANTBULLET_SHOOTED_LEFT_120_STATE);
	}
	else if (tan > FIREPIRANHAPLANT_TAN_ANGLE_ATTACK)
	{
		firePlantBullet->SetState(FIREPLANTBULLET_SHOOTED_LEFT_150_STATE);
	}
	else if (tan < 0 && tan > -FIREPIRANHAPLANT_TAN_ANGLE_ATTACK)
	{
		firePlantBullet->SetState(FIREPLANTBULLET_SHOOTED_LEFT_30_STATE);
	}
	else
	{
		firePlantBullet->SetState(FIREPLANTBULLET_SHOOTED_LEFT_60_STATE);
	}
}

void CFirePiranhaPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + 16;
	bottom = y + 33;
	if (state == FIREPIRANHAPLANT_STATE_DESTROY || state == FIREPIRANHAPLANT_STATE_HIDE)
	{
		right = 0;
		bottom = 0;
	}
}

void CFirePiranhaPlant::CreateFirePlantBullet(CGameObject* fireplantbullet)
{
	if (firePlantBullet == NULL)
		firePlantBullet = fireplantbullet;
	else if (firePlantBullet->GetState() == FIREPLANTBULLET_DESTROY_STATE)
		firePlantBullet->SetState(FIREPLANTBULLET_TRANSPARENT_STATE);
}

void CFirePiranhaPlant::FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy)
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
		if (dynamic_cast<CMario*>(c->obj)) {
			min_tx = c->t; nx = c->nx; min_ix = i; rdx = c->dx;
			min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
		}
	}
	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}

void CFirePiranhaPlant::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FIREPIRANHAPLANT_STATE_DESTROY:
		break;
	case FIREPIRANHAPLANT_STATE_MOVE_UP:
		stateHide = 0;
		stateMoveUp = true;
		vy = -0.035f;
		break;
	case FIREPIRANHAPLANT_STATE_MOVE_DOWN:
		stateHide = 0;
		move_down_state = 1;
		vy = 0.035f;
		break;
	case FIREPIRANHAPLANT_STATE_HIDE:
		CreateFirePlantBullet(NULL);
		is_shoot = 0;
		stateHide = 1;
		appearance_state = 0;
		stateMoveUp = false;
		move_down_state = 0;
		vy = 0;
		break;
	case FIREPIRANHAPLANT_STATE_APPEARANCE:
		appearance_state = 1;
		stateHide = 0;
		stateMoveUp = false;
		move_down_state = 0;
		vy = 0;
		break;
	default:
		break;
	}
}

int CFirePiranhaPlant::GetState()
{
	return CGameObject::GetState();
}
