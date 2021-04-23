#include "Number.h"
#include "PlayScence.h"
#include "Game.h"
#include "GameObject.h"





void CNumber::Render()
{
	int ani;
	switch (number)
	{
	case NUMBER_STATE_1:
		ani = NUMBER_ANI_1;
		break;
	case NUMBER_STATE_2:
		ani = NUMBER_ANI_2;
		break;
	case NUMBER_STATE_3:
		ani = NUMBER_ANI_3;
		break;
	case NUMBER_STATE_4:
		ani = NUMBER_ANI_4;
		break;
	case NUMBER_STATE_5:
		ani = NUMBER_ANI_5;
		break;
	case NUMBER_STATE_6:
		ani = NUMBER_ANI_6;
		break;
	case NUMBER_STATE_7:
		ani = NUMBER_ANI_7;
		break;
	case NUMBER_STATE_8:
		ani = NUMBER_ANI_8;
		break;
	case NUMBER_STATE_9:
		ani = NUMBER_ANI_9;
		break;
	default:
		ani = NUMBER_ANI_0;
		break;
	}
	animation_set->at(ani)->Render(x, y);
}

void CNumber::SetState(int state)
{
	CGameObject::SetState(state);
	number = state;
}

void CNumber::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//CGameObject::Update(dt, coObjects);
	//x = 200;
	//y = 50;
	
}

void CNumber::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

