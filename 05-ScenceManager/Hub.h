#pragma once

#include "GameObject.h"
#include "Arrow.h"
#include "Time.h"
#include "Coin.h"
#include "Card.h"
#include "Camera.h"


#define MaxShowCards 3

class CHub : public CGameObject
{
	CCamera* camera;
	CListArrow* arrows;
	CTime* time;
	CCoinPlay* coin;
	CCard* cards[MaxShowCards];
public:
	CHub();
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	void SetTimeHub(CTime* t);
	void SetCoinHub(CCoinPlay* c);
	void SetArrowHub(CListArrow* a);
	void SetCardHub(CCard* c[MaxShowCards]);
	void SetCameraHub(CCamera* c);

};