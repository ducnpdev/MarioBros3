#include "Goomba.h"

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

}

void CGoomba::Render()
{
	int ani = 0;
	animation_set->at(ani)->Render(x, y);

}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	default: 
		break;
	}

}
