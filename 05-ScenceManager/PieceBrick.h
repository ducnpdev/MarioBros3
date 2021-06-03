#pragma once
#include "GameObject.h"
#define STATE_PIECE_BRICK_HIDEN			0
#define STATE_PIECE_BRICK_DISPLAY		1
#define STATE_PIECE_BRICK_HIDEN_TIME	400

class CPieceBrick : public CGameObject
{
	DWORD timeDisplay;
public:
	CPieceBrick();
	void SetDisplay(float x, float y, float vx, float vy, DWORD t);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};