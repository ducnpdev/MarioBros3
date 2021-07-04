#include "WoodBlock.h"
#include "Utils.h"
#include "Mario.h"
#include "PlayScence.h"
#include "MarioConfig.h"

CWoodBlock::CWoodBlock(int _type, CMushroom* _mushroom, CLeaf* _leaf)
{
	type = _type;
	mushroom = _mushroom;
	leaf = _leaf;
}

void CWoodBlock::Render()
{
	animation_set->at(0)->Render(x, y);
}

void CWoodBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + WOOD_BLOCK_BBOX_WIDTH;
	b = y + WOOD_BLOCK_BBOX_HEIGHT;
}

void CWoodBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* objects)
{
	CGameObject::Update(dt);
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (isActive) {
		DebugOut(L"WoodBlock update %d \n", mario->GetMarioLevel());
		isActive = false;
		int marioLevel = mario->GetMarioLevel();
		if (marioLevel == LEVEL_MARIO_SMAIL) {
			if (mushroom == NULL) return;
			mushroom->SetPosition(x +10, y - 10);
			mushroom->SetState(MUSHROOM_STATE_MOVING);
			mushroom = NULL;
			leaf = NULL;
		}
		else {
			if (leaf == NULL) return;
			leaf->SetPosition(x + 10, y-10);
			leaf->SetState(LEAF_STATE_FLY);
			mushroom = NULL;
			leaf = NULL;
		}
	}
}
