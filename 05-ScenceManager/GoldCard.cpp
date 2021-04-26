#include "GoldCard.h"
#include "Game.h"
#include "Utils.h"

void CGoalCard::SetState(int state)
{
	CGameObject::SetState(state);
}

void CGoalCard::Render()
{
	int ani = GOALCARD_ANI_STAR;
	if (state == GOALCARD_STATE_HIDEN) return;
	else if (state == GOALCARD_STATE_FLOWER) ani = GOALCARD_ANI_FLOWER;
	else if (state == GOALCARD_STATE_STAR) ani = GOALCARD_ANI_STAR;
	else if (state == GOALCARD_STATE_MUSHROOM) ani = GOALCARD_ANI_MUSHROOM;

	animation_set->at(ani)->Render(x, y);
}

void CGoalCard::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	if (GetTickCount64() - create_start > 100 && state != GOALCARD_STATE_HIDEN)
	{
		if (state <= GOALCARD_STATE_MUSHROOM) state += 1;
		if (state == GOALCARD_STATE_MUSHROOM + 1) state = 1;

		create_start = (DWORD)GetTickCount64();
	}
	if (isHiden)
	{
		if (GetTickCount64() - hiden_start > GOALCARD_SWITCH_TIME)
		{
			//DebugOut(L"da vao switch scene\n");
			CGame::GetInstance()->SetCamPos(0, 0);
			CGame::GetInstance()->SwitchScene(3);
			isHiden = 0;
		}
	}
}

void CGoalCard::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state == GOALCARD_STATE_HIDEN) return;
	else
	{
		l = x;
		t = y;
		r = x + GOALCARD_BBOX_WIDTH;
		b = y + GOALCARD_BBOX_HEIGHT;
	}

}