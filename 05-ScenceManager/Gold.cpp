#include "Gold.h"


CGold::CGold()
{
	// SetState(GOOMBA_STATE_WALKING);
}

void CGold::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

}

void CGold::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;

	if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}

	if (vx > 0 && x > 290) {
		x = 290; vx = -vx;
	}
}

void CGold::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CGold::SetState(int state)
{
	CGameObject::SetState(state);
}
