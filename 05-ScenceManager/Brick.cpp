#include "Brick.h"
#include "Utils.h"
#include "PlayScence.h"

CBrick::CBrick()
{
}

void CBrick::Render()
{
	animation_set->at(0)->Render(x, y);
}

void CBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}

void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
//	DebugOut(L"class brick \n");
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
}

void CBrick::SetState(int state)
{
	CGameObject::SetState(state);
}
