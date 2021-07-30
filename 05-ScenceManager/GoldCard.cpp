#include "GoldCard.h"
#include "Game.h"
#include "Utils.h"
#include "All.h"
#include "PlayScence.h"
#include "MarioConfig.h"

CGoalCard::CGoalCard(CCardText* c)
{
	create_start = (DWORD)GetTickCount64(); 
	state = 1;
	cardtext = c;
	isHiden = 0; 
	hiden_start = 0;
	pre_end = (DWORD)GetTickCount64();

}

void CGoalCard::SetState(int state)
{
	CGameObject::SetState(state);
}

void CGoalCard::Render()
{
	int ani = GOALCARD_ANI_STAR;
	if (checkPreen) {
		ani = 0;
	}
	else {
		if (state == GOALCARD_STATE_HIDEN) return;
		else if (state == GOALCARD_STATE_FLOWER) ani = GOALCARD_ANI_FLOWER;
		else if (state == GOALCARD_STATE_STAR) ani = GOALCARD_ANI_STAR;
		else if (state == GOALCARD_STATE_MUSHROOM) ani = GOALCARD_ANI_MUSHROOM;
	}
	 animation_set->at(ani)->Render(x, y);
}

void CGoalCard::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt, colliable_objects);
	ProcessSwithc();
	if (checkPreen) {
		vy = -0.5f;
		y += dy;
		x += dx;
		//if (GetTickCount64() - pre_end > GOALCARD_SWITCH_PRE_END) {

		if (GetTickCount64() - pre_end > 100) {
			okscene = true;
			checkPreen = false;
		}
		return;
	}
	vx = 0.0f;
	vy = 0.0f;
	
	if (checkPreen) {
		// okscene
	}

	// process change state for mario collision
	if (GetTickCount64() - create_start > 100 && state != GOALCARD_STATE_HIDEN)
	{
		if (state <= GOALCARD_STATE_MUSHROOM) state += 1;
		// reset state
		if (state == GOALCARD_STATE_MUSHROOM + 1) state = 1;
		create_start = (DWORD)GetTickCount64();
	}
	if (isHiden)
	{
		CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		if (GetTickCount64() - hiden_start > GOALCARD_SWITCH_TIME)
		{
			mario->SetMarioPreEndScene(false);
			CGame::GetInstance()->SetCamPos(0, 0);
			CGame::GetInstance()->SwitchScene(SCENE_0);
			isHiden = 0;
		}else {
			mario->SetMarioPreEndScene(true);
		}
	}
	
}

void CGoalCard::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state == GOALCARD_STATE_HIDEN) return;
	l = x;
	t = y;
	r = x + GOALCARD_BBOX_WIDTH;
	b = y + GOALCARD_BBOX_HEIGHT;
	

}

void CGoalCard::ProcessSwithc()
{
	if (!okscene) return;
	DebugOut(L"state %d \n", state);
	SetSwitchScene((DWORD)GetTickCount64());
	GetCardText()->SetState(1);
	GetCardText()->GetCard()->SetState(GetState());
	SetState(0);
}
