#include "Brick.h"
#include "Utils.h"
#include "PlayScence.h"
#include "ItemCoin.h"
#include "GoombaConfig.h"

CBrick::CBrick(float _originX, float _originY, int _type)
{
	state = BRICK_STATE_NORMAL;
	for (int i = 0; i < ITEM_AMOUNT; i++)
	{
		item[i] = NULL;
	}
	originX = _originX;
	originY = _originY;
	type = _type;
	timeBack = (DWORD)0.0f;
	timeAttach = (DWORD)0.0f;
}

void CBrick::Render()
{
	if (state == BRICK_STATE_HIDEN) return;
	int ani = 0;
	if (state == BRICK_STATE_NEED_CRETE) ani = 1;
	animation_set->at(ani)->Render(x, y);
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
	if (state == BRICK_STATE_NEED_CRETE) return;

	if (state == BRICK_STATE_FLY_UP && GetTickCount64() - timeBack > 70) {
		SetState(BRICK_STATE_FLY_DOWN);
	}
	if ( y > originY) {
		y = originY;
		SetState(BRICK_STATE_NORMAL);
	}

	/*if (state == BRICK_STATE_FLY_UP && originY - y < 5) {
		SetState(BRICK_STATE_FLY_DOWN);
	}*/

	CGameObject::Update(dt);
	y += vy * dt;
	x += vx * dt;

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->GetMarioIsFight() && state == BRICK_STATE_NORMAL)
	{
			// xử lí va chạm với gạch trong khoảng cách so với Y của mario
			if (abs(mario->y - y) > DISTANCE_BRICK_WITH_MARIO_MIN && abs(mario->y - y) < DISTANCE_BRICK_WITH_MARIO_MAX)
			{
				bool temcheck = false;
				// xử lí va chạm với gạch trong khoảng cách so với X của mario
				if (mario->x - x < 0 && abs(x - mario->x) <= GOOMBA_AUTO_DEAD_ZONE)
				{
					if (GetTickCount64() - timeAttach > GOOMBA_TIME_ATTACHT) {
						timeAttach = (DWORD)GetTickCount64();
						if (type == BRICK_TYPE_HAVE_ITEM) { 
							temcheck = true;
						}else{
							SetState(BRICK_STATE_HIDEN);
							((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->CreatePieceBrick(x, y, (DWORD)GetTickCount64());
						}
					}
				}
				else
				{
					if (abs(mario->x - x) <= GOOMBA_AUTO_DEAD_ZONE)
					{
						if (GetTickCount64() - timeAttach > GOOMBA_TIME_ATTACHT) {
							timeAttach = (DWORD)GetTickCount64();
							if (type == BRICK_TYPE_HAVE_ITEM) {
								temcheck = true;
							}
							else {
								SetState(BRICK_STATE_HIDEN);
								((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->CreatePieceBrick(x, y, (DWORD)GetTickCount64());
							}	
						}
					}
				}
				if (temcheck) HaveItem();
				temcheck = false;
			
		}
	}

	if (state == BRICK_STATE_310) {
		/*if (type == BRICK_TYPE_NOT_ITEM) {
			if (mario->GetMarioLevel() == LEVEL_MARIO_SMAIL) {
				SetState(BRICK_STATE_FLY_UP);
				timeBack = (DWORD)GetTickCount64();
				mario->SetMarioNotJump(true);
			}
			else {
				float mariovx, mariovy;
				mario->GetSpeed(mariovx, mariovy);
				mario->SetSpeed(mariovx, 0.01f);
				SetState(BRICK_STATE_HIDEN);
				((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->CreatePieceBrick(x, y, (DWORD)GetTickCount64());
			}
		}*/

		for (int i = 0; i < ITEM_AMOUNT; i++)
		{
			SetState(BRICK_STATE_320);
			if (type == BRICK_TYPE_HAVE_ITEM) {
				mario->SetMarioNotJump(true);

				if (item[i] != NULL) {
					item[i]->SetPosition(originX, originY);
					item[i]->SetState(COIN_STATE_MOVING_OF_BRICK);
					item[i] = NULL;
					break;
				}
				if (i == ITEM_AMOUNT - 1 && item[i] == NULL) {
					SetState(BRICK_STATE_NEED_CRETE);
					((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->CreatePieceBrick(x, y, (DWORD)GetTickCount64());
					mario->SetMarioIsRenderMusic(true);
					break;
				}
			}
			if (type == BRICK_TYPE_NOT_ITEM) {
				mario->SetMarioNotJump(true);
				if ( mario->GetMarioLevel() == LEVEL_MARIO_SMAIL ) {
					if (GetTickCount64() - timeBack > 200) {
						SetState(BRICK_STATE_FLY_UP);
						timeBack = (DWORD)GetTickCount64();
					}
					break;
				}

				else {
					float mariovx, mariovy;
					mario->GetSpeed(mariovx, mariovy);
					mario->SetSpeed(mariovx, 0.01f);
					SetState(BRICK_STATE_HIDEN);
					// ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->CreatePieceBrick(x, y, (DWORD)GetTickCount64());
					break;
				}
			}
		
		}
	
	}


}

void CBrick::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BRICK_STATE_FLY_UP:
		DebugOut(L"222 \n");

		vy = -0.2f;
		break;
	case BRICK_STATE_FLY_DOWN:
		vy = 0.2f;
		break;
	case BRICK_STATE_NORMAL:

		vx = 0.0f;
		vy = 0.0f;
		break;
	default:
		break;
	}
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

CGameObject* CBrick::GetItemLatest()
{
	return item[9];
}

void CBrick::HaveItem() {
	if (type == BRICK_TYPE_HAVE_ITEM) {
		CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			for (int i = 0; i < ITEM_AMOUNT; i++)
			{
				DebugOut(L"aaaaaaaa\n");
				if (item[i] != NULL) {
					item[i]->SetPosition(originX, originY);
					item[i]->SetState(COIN_STATE_MOVING_OF_BRICK);
					item[i] = NULL;
					break;
				}
				if (i == ITEM_AMOUNT - 1 && item[i] == NULL) {
					SetState(BRICK_STATE_NEED_CRETE);
					((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->CreatePieceBrick(x, y, (DWORD)GetTickCount64());
					mario->SetMarioIsRenderMusic(true);
					break;
				}
			}
		}
}
