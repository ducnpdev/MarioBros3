#include "WoodBlock.h"
#include "Utils.h"
#include "Mario.h"
#include "PlayScence.h"
#include "MarioConfig.h"

CWoodBlock::CWoodBlock(int _type, CMushroom* _mushroom, CLeaf* _leaf, float _originX)
{
	type = _type;
	mushroom = _mushroom;
	leaf = _leaf;
	woodBlockOriginX = _originX;
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

void CWoodBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	RenderItemUpLevelMario();

	if (type == 0) return;
	
	if (type == WOOD_BLOCK_EFFECT_MARIO) {
		x += dx;
		if (state == WOOD_BLOCK_MOVING_RIGHT) {
			if (x - woodBlockOriginX > 14) {
				SetState(WOOD_BLOCK_MOVING_LEFT);
				SetState(WOOD_BLOCK_TOP_LEFT);
			}
		}
		if (state == WOOD_BLOCK_MOVING_LEFT ) {
			x += dx;
			if (woodBlockOriginX - x > 14) {
				SetState(WOOD_BLOCK_MOVING_RIGHT);
				SetState(WOOD_BLOCK_TOP_RIGHT);
			}
		}
		if (state == WOOD_BLOCK_TOP_LEFT) {
			if (x < woodBlockOriginX) {
				x = woodBlockOriginX;
			}
		}
		if (state == WOOD_BLOCK_TOP_RIGHT) {
			if (x > woodBlockOriginX) {
				x = woodBlockOriginX;
			}
		}
	}
}

void CWoodBlock::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case WOOD_BLOCK_MOVING_LEFT:
		vx =-0.05f;
		break;
	case WOOD_BLOCK_MOVING_RIGHT:
		vx = 0.05f;
		break;
	case WOOD_BLOCK_TOP_LEFT:
		break;
	case WOOD_BLOCK_TOP_RIGHT:
	//	vx = 0; vy = 0;
		break;
	default:
		break;
	}
}

void CWoodBlock::RenderItemUpLevelMario()
{
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (isActive) {
		isActive = false;
		int marioLevel = mario->GetMarioLevel();
		if (marioLevel == LEVEL_MARIO_SMAIL) {
			if (mushroom == NULL) return;
			mushroom->SetPosition(x + 10, y - 10);
			mushroom->SetState(MUSHROOM_STATE_MOVING);
			mushroom = NULL;
			leaf = NULL;
		}
		else {
			if (leaf == NULL) return;
			leaf->SetPosition(x + 10, y - 10);
			leaf->SetState(LEAF_STATE_FLY);
			mushroom = NULL;
			leaf = NULL;
		}
	}
}
