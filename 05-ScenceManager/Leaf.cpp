#include "Leaf.h"
#include "Utils.h"
#include "Animations.h"

CLeaf::CLeaf()
{
	SetState(LEAF_STATE_HIDEN);
	effect_state = 0;
	// hiden_state = 1;
	effect_left_time_start = (DWORD)0.0f;
	effect_left_time = (DWORD)0.0f;
	effect_right_time = (DWORD)0.0f;
	effect_right_time_start = (DWORD)0.0f;
	effect_left_time_start = (DWORD)0.0f;
	isRight = 0;
	intro_state = 0;
	create_time = (DWORD)GetTickCount64();
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	// if (intro_state) CreateIntroAnimationLeaf();
	if (hiden_state) return;
	if (state == LEAF_STATE_FLY)
	{
		DebugOut(L"CLeaf class \n");

		if (!effect_left_time_start && !effect_right_time_start)
		{
			if (GetTickCount64() - effect_time_start < LEAF_EFFECT_RIGHT_TIME)
			{
				isRight = 1;
				vy = -LEAF_SPEED_Y;
			}
			else
			{
				effect_right_time_start = (DWORD)GetTickCount64();
			}
		}


		if (GetTickCount64() - effect_right_time_start < EFFECT_LIMIT_TIME && effect_right_time_start != 0)
		{
			isRight = 1;
			vy = LEAF_GRAVITY_Y;
			vx = LEAF_SPEED_X;
		}
		else if (GetTickCount64() - effect_right_time_start >= EFFECT_LIMIT_TIME && effect_right_time_start != 0)
		{
			effect_right_time_start = 0;
			effect_left_time_start = (DWORD)GetTickCount64();
		}

		if (GetTickCount64() - effect_left_time_start < EFFECT_LIMIT_TIME && effect_left_time_start != 0)
		{
			isRight = 0;
			vy = LEAF_GRAVITY_Y;
			vx = -LEAF_SPEED_X;
		}
		else if (GetTickCount64() - effect_left_time_start >= EFFECT_LIMIT_TIME && effect_left_time_start != 0)
		{
			effect_left_time_start = 0;
			effect_right_time_start = (DWORD)GetTickCount64();
		}

		y += dy;
		x += dx;

	}

}

void CLeaf::Render()
{
	int ani;
	if (GetState() == LEAF_STATE_HIDEN || hiden_state) return;
	if (isRight) ani = LEAF_EFFECT_RIGHT_ANI;
	else ani = LEAF_EFFECT_LEFT_ANI;
	animation_set->at(ani)->Render(x, y);
}



void CLeaf::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case LEAF_STATE_FLY:
		hiden_state = 0;
		if (effect_state == 0)
		{
			effect_state = 1;
			effect_time_start = (DWORD)GetTickCount64();
		}
		break;
	case LEAF_STATE_HIDEN:
		effect_state = 0;
		hiden_state = 1;
		break;
	case LEAF_STATE_NORMAL:
		hiden_state = 0;
		effect_state = 0;
		break;
	default:
		break;
	}
}

void CLeaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + LEAF_BBOX_WIDTH;
	b = y + LEAF_BBOX_HEIGHT;
	if (state == LEAF_STATE_HIDEN)
	{
		r = 0;
		b = 0;
	}
}