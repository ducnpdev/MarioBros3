#include "Enemies.h"

CEnemies::CEnemies()
{
	// SetState(GOOMBA_STATE_WALKING);
}

void CEnemies::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	/*left = x;
	top = y;
	right = x + GOOMBA_BBOX_WIDTH;

	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else
		bottom = y + GOOMBA_BBOX_HEIGHT;*/
}

void CEnemies::Render()
{

	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CEnemies::SetState(int state)
{
	CGameObject::SetState(state);
}
