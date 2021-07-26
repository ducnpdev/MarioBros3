#include "MarioWorldmap.h"
#include "MarioConfig.h"
#include "Utils.h"

CMarioWorldmap::CMarioWorldmap()
{
	isMoving = false;
	move_right = 0;
	move_top = 0;
	move_left = 0;
	move_bottom = 0;
	move_bottom = 0;
	destination_x = 0;
	destination_y = 0;
}


void CMarioWorldmap::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt);
	if (state == MARIO_STATE_MOVING_RIGHT)
	{
		for (size_t i = 0; i < nodes.size(); i++)
		{
			if (nodes[i] == NULL) {
				continue;
			}
			float lefttemp = nodes[i]->GetLeft();
			if (x + MARIO_INTRO_SMALL_BBOX_WIDTH == nodes[i]->GetLeft() && y == nodes[i]->GetTop())
			{
				move_right = 1;
				destination_x = x + 2 * MARIO_INTRO_SMALL_BBOX_WIDTH;
				destination_y = y;
				break;
			}
		}
	}
	else if (state == MARIO_STATE_MOVING_LEFT)
	{
		for (size_t i = 0; i < nodes.size(); i++)
		{
			if (nodes[i] == NULL) {
				DebugOut(L"node == null");
				continue;
			}
			if (x - MARIO_INTRO_SMALL_BBOX_WIDTH == nodes[i]->GetLeft() && y == nodes[i]->GetTop())
			{
				move_left = 1;
				destination_x = x - 2 * MARIO_INTRO_SMALL_BBOX_WIDTH;
				destination_y = y;
				break;
			}
		}
	}
	else if (state == MARIO_STATE_MOVING_UP)
	{
		for (size_t i = 0; i < nodes.size(); i++)
		{
			if (nodes[i] == NULL) {
				DebugOut(L"node == null");
				continue;
			}
			if (x == nodes[i]->GetLeft() && y - MARIO_INTRO_SMALL_BBOX_HEIGHT == nodes[i]->GetTop())
			{
				move_top = 1;
				destination_x = x;
				destination_y = y - 2 * MARIO_INTRO_SMALL_BBOX_HEIGHT;
				break;
			}
		}
	}
	else if (state == MARIO_STATE_MOVING_DOWN)
	{
		for (size_t i = 0; i < nodes.size(); i++)
		{
			if (nodes[i] == NULL) {
				DebugOut(L"node == null");
				continue;
			}
			if (x == nodes[i]->GetLeft() && y + MARIO_INTRO_SMALL_BBOX_HEIGHT == nodes[i]->GetTop())
			{
				move_bottom = 1;
				destination_x = x;
				destination_y = y + 2 * MARIO_INTRO_SMALL_BBOX_HEIGHT;
				break;
			}
		}
	}

	if (move_right) {
		x += vx * dt;
		if (x > destination_x)
		{
			SetPosition(destination_x, destination_y);
			SetState(MARIO_STATE_IDLE);
		}
	}

	else if (move_left) {
		x += vx * dt;
		if (x < destination_x)
		{
			SetPosition(destination_x, destination_y);
			SetState(MARIO_STATE_IDLE);
		}
	}
	else if (move_top) {
		y += vy * dt;
		if (y < destination_y)
		{
			SetPosition(destination_x, destination_y);
			SetState(MARIO_STATE_IDLE);
		}
	}
	else if (move_bottom) {
		y += vy * dt;
		if (y > destination_y)
		{
			SetPosition(destination_x, destination_y);
			SetState(MARIO_STATE_IDLE);
		}
	}
	else SetState(MARIO_STATE_IDLE);
}

void CMarioWorldmap::Render()
{
	animation_set->at(0)->Render(x, y);
}


void CMarioWorldmap::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MARIO_STATE_MOVING_RIGHT:
		isMoving = true;
		vx = MARIO_INTRO_WALKING_SPEED;
		break;
	case MARIO_STATE_MOVING_UP:
		isMoving = true;
		vy = -MARIO_INTRO_WALKING_SPEED;
		break;
	case MARIO_STATE_MOVING_DOWN:
		isMoving = true;
		vy = MARIO_INTRO_WALKING_SPEED;
		break;
	case MARIO_STATE_MOVING_LEFT:
		isMoving = true;
		vx = -MARIO_INTRO_WALKING_SPEED;
		break;
	case MARIO_STATE_IDLE:
		isMoving = false;
		move_bottom = 0;
		move_right = 0;
		move_left = 0;
		move_top = 0;
		vx = 0;
		vy = 0;
		break;
	}
}

// 
int CMarioWorldmap::GetTypeCurrentNode()
{
	for (size_t i = 0; i < nodes.size(); i++)
	{
		if (nodes[i] == NULL) {
			continue;
		}
		if (x == nodes[i]->GetLeft() && y == nodes[i]->GetTop())
		{
			return nodes[i]->GetType();
		}
	}
	return 0;
}

void CMarioWorldmap::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + MARIO_WORLD_MAP_SMALL_BBOX_WIDTH;
	b = y + MARIO_WORLD_MAP_SMALL_BBOX_HEIGHT;
}
void CMarioWorldmap::PushNode(CNode* node)
{
	if (node == NULL) {
		return;
	}
	nodes.push_back(node);
}

bool CMarioWorldmap::GetIsMoving()
{
	return isMoving;
}