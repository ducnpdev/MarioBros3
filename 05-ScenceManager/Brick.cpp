#include "Brick.h"
#include "Utils.h"
#include "PlayScence.h"

CBrick::CBrick()
{
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

	if (state == BRICK_STATE_HIDEN || state == BRICK_STATE_DESTROY) return;
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}

void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->GetMarioIsFight() && state == BRICK_STATE_NORMAL)
	{
		// xử lí va chạm với gạch trong khoảng cách so với Y của mario
		if (abs(mario->y - y) > DISTANCE_BRICK_WITH_MARIO_MIN && abs(mario->y - y) < DISTANCE_BRICK_WITH_MARIO_MAX)
		{
			// xử lí va chạm với gạch trong khoảng cách so với X của mario
			if (mario->x - x < 0 && abs(x - mario->x) <= GOOMBA_AUTO_DEAD_ZONE)
			{
				DebugOut(L"111 \n");
				SetState(BRICK_STATE_HIDEN);
				((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->CreatePieceBrick(x, y, (DWORD)GetTickCount64());
			}
			else
			{
				DebugOut(L"222 \n");

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
