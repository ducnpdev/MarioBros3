#include "MarioWorldmap.h"
#include "MarioConfig.h"

CMarioWorldmap::CMarioWorldmap()
{
	isMoving = false;
	move_right = 0;
	move_top = 0;
	move_left = 0;
	move_bottom = 0;
	destination_x = 0;
	destination_y = 0;
}

void CMarioWorldmap::SetState(int state)
{
	CGameObject::SetState(state);
}

void CMarioWorldmap::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt);

}

void CMarioWorldmap::Render()
{
	animation_set->at(0)->Render(x, y);
}

int CMarioWorldmap::GetTypeCurrentNode()
{
	return 0;
}

void CMarioWorldmap::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + MARIO_WORLD_MAP_SMALL_BBOX_WIDTH;
	b = y + MARIO_WORLD_MAP_SMALL_BBOX_HEIGHT;
}
bool CMarioWorldmap::GetIsMoving()
{
	return isMoving;
}