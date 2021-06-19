#pragma once
#include "GameObject.h"
#include "CardText.h"

#define GOALCARD_BBOX_HEIGHT		16
#define GOALCARD_BBOX_WIDTH			16

#define GOALCARD_STATE_HIDEN		0
#define GOALCARD_STATE_STAR			1
#define GOALCARD_STATE_FLOWER		2
#define GOALCARD_STATE_MUSHROOM		3

#define GOALCARD_ANI_STAR			0
#define GOALCARD_ANI_FLOWER			1
#define GOALCARD_ANI_MUSHROOM		2

#define GOALCARD_SWITCH_TIME		1000


class CGoalCard : public CGameObject
{
	int isHiden;
	DWORD hiden_start;
	DWORD create_start;
	CCardText* cardtext;
public:
	CGoalCard(CCardText* c);
	void SetState(int state);
	void SetSwitchScene(DWORD t) {
		isHiden = 1;
		hiden_start = t;
	}
	void SetStateCardText(int s) { cardtext->SetState(s); }
	void SetCardText(CCardText* c) { cardtext = c; }
	CCardText* GetCardText() { return cardtext; }
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

