#pragma once
#include "GameObject.h"

#define CARD_STATE_HIDEN			-1
#define CARD_STATE_EMPTY			0
#define CARD_STATE_STAR				1
#define CARD_STATE_FLOWER			2
#define CARD_STATE_MUSHROOM			3

#define CARD_ANI_EMPTY				0
#define CARD_ANI_STAR				1
#define CARD_ANI_FLOWER				2
#define CARD_ANI_MUSHROOM			3
class CCard : public CGameObject
{
public:
	virtual void SetState(int state);
	virtual void Render();	
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* objects);
	virtual void GetBoundingBox(float& lelf, float& t, float& r, float& b);
};
