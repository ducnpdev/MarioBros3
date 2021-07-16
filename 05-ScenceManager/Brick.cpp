#include "Brick.h"
#include "Utils.h"
#include "PlayScence.h"
#include "ItemCoin.h"
#include "GoombaConfig.h"

CBrick::CBrick()
{
	state = BRICK_STATE_NORMAL;
	for (int i = 0; i < ITEM_AMOUNT; i++)
	{
		item[i] = NULL;
	}
}

void CBrick::Render()
{
	if (state == BRICK_STATE_HIDEN) return;
	animation_set->at(0)->Render(x, y);
}

void CBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	// DebugOut(L"state birck %d \n", state);
	if (state == BRICK_STATE_HIDEN || state == BRICK_STATE_DESTROY) return;
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}

void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
//	DebugOut(L"mario with brick \n");
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->GetMarioIsFight() && state == BRICK_STATE_NORMAL)
	{
		// xử lí va chạm với gạch trong khoảng cách so với Y của mario
		if (abs(mario->y - y) > DISTANCE_BRICK_WITH_MARIO_MIN && abs(mario->y - y) < DISTANCE_BRICK_WITH_MARIO_MAX)
		{
			// xử lí va chạm với gạch trong khoảng cách so với X của mario
			if (mario->x - x < 0 && abs(x - mario->x) <= GOOMBA_AUTO_DEAD_ZONE)
			{
				SetState(BRICK_STATE_HIDEN);
				((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->CreatePieceBrick(x, y, (DWORD)GetTickCount64());
			}
			else
			{
				if (abs(mario->x - x) <= GOOMBA_AUTO_DEAD_ZONE)
				{
					SetState(BRICK_STATE_HIDEN);
					((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->CreatePieceBrick(x, y, (DWORD)GetTickCount64());
				}

			}
		}
	}
}

void CBrick::SetState(int state)
{
	CGameObject::SetState(state);
}

void CBrick::AddItemBrick(CGameObject* obj)
{
	for (int i = 0; i < ITEM_AMOUNT; i++)
	{
		if (item[i] == NULL)
		{
			item[i] = obj;
			return;
		}
	}
}

void CBrick::SetItem()
{
	item[0]->SetState(COIN_STATE_NORMAL);
	item[0]->SetPosition(x, y);
}

CGameObject* CBrick::GetItem()
{
	return item[0];
}
