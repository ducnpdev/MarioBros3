#include "FirePlantBullet.h"
#include "Mario.h"

CFirePlantBullet::CFirePlantBullet()
{
	state = FIREPLANTBULLET_TRANSPARENT_STATE;
	vx = 0;
}


void CFirePlantBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt);
	
	// DebugOut(L"class fire plant bullet state: %d \n",state);
	if (state != FIREPLANTBULLET_TRANSPARENT_STATE && state != FIREPLANTBULLET_DESTROY_STATE)
	{
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		CalcPotentialCollisions(colliable_objects, coEvents);

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

			x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.4f;


		}
		if (GetTickCount() - shoot_start > FIREPLANTBULLET_DESTROY_TIME)
		{
			SetState(FIREPLANTBULLET_DESTROY_STATE);
		}
	}
	else
	{
		shoot_start = GetTickCount();
	}
}

void CFirePlantBullet::Render()
{
	if (state != FIREPLANTBULLET_TRANSPARENT_STATE && state != FIREPLANTBULLET_DESTROY_STATE)
		animation_set->at(0)->Render(x, y);
}

void CFirePlantBullet::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FIREPLANTBULLET_DESTROY_STATE:
		break;
	case FIREPLANTBULLET_SHOOTED_LEFT_30_STATE:
		vx = -FIRE_PLANT_BULLET_FLYING_SPEECH_X + FIREPLANTBULLET_SPEED_PLUS_X;
		vy = FIRE_PLANT_BULLET_FLYING_SPEECH_Y;
		break;
	case FIREPLANTBULLET_SHOOTED_LEFT_60_STATE:
		vx = -FIRE_PLANT_BULLET_FLYING_SPEECH_X - FIREPLANTBULLET_SPEED_PLUS_X;
		vy = FIRE_PLANT_BULLET_FLYING_SPEECH_Y - FIREPLANTBULLET_SPEED_PLUS_Y;
		break;
	case FIREPLANTBULLET_SHOOTED_LEFT_120_STATE:
		vx = -FIRE_PLANT_BULLET_FLYING_SPEECH_X;
		vy = -FIRE_PLANT_BULLET_FLYING_SPEECH_Y;
		break;
	case FIREPLANTBULLET_SHOOTED_LEFT_150_STATE:
		vx = -FIRE_PLANT_BULLET_FLYING_SPEECH_X + FIREPLANTBULLET_SPEED_PLUS_X;
		vy = -FIRE_PLANT_BULLET_FLYING_SPEECH_Y + FIREPLANTBULLET_SPEED_PLUS_Y;
		break;
	case FIREPLANTBULLET_SHOOTED_RIGHT_30_STATE:
		vx = FIRE_PLANT_BULLET_FLYING_SPEECH_X - FIREPLANTBULLET_SPEED_PLUS_X;
		vy = FIRE_PLANT_BULLET_FLYING_SPEECH_Y;
		break;
	case FIREPLANTBULLET_SHOOTED_RIGHT_60_STATE:
		vx = FIRE_PLANT_BULLET_FLYING_SPEECH_X + FIREPLANTBULLET_SPEED_PLUS_X;
		vy = FIRE_PLANT_BULLET_FLYING_SPEECH_Y - FIREPLANTBULLET_SPEED_PLUS_Y;
		break;
	case FIREPLANTBULLET_SHOOTED_RIGHT_120_STATE:
		vx = FIRE_PLANT_BULLET_FLYING_SPEECH_X;
		vy = -FIRE_PLANT_BULLET_FLYING_SPEECH_Y;
		break;
	case FIREPLANTBULLET_SHOOTED_RIGHT_150_STATE:
		vx = FIRE_PLANT_BULLET_FLYING_SPEECH_X - FIREPLANTBULLET_SPEED_PLUS_X;
		vy = -FIRE_PLANT_BULLET_FLYING_SPEECH_Y + FIREPLANTBULLET_SPEED_PLUS_Y;
		break;
	case FIREPLANTBULLET_TRANSPARENT_STATE:
		vx = 0;
		vy = 0;
		break;
	default:
		break;
	}
}

void CFirePlantBullet::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;

	if (state == FIREPLANTBULLET_TRANSPARENT_STATE || state == FIREPLANTBULLET_DESTROY_STATE)
	{
		r = 0;
		b = 0;
	}
	else
	{
		r = x + FIREPLANTBULLET_BBOX_WIDTH;
		b = y + FIREPLANTBULLET_BBOX_HEIGHT;
	}
}

void CFirePlantBullet::FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy)
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

void CFirePlantBullet::SetShootStart(DWORD t)
{
	shoot_start = t;
}