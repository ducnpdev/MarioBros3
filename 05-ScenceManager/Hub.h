#pragma once

#include "GameObject.h"
#include "Arrow.h"
#include "Time.h"
#include "Coin.h"
#include "Card.h"
#include "Camera.h"
#include "Lives.h"



#define MaxShowCards 3
#define BOARD_TIME_X_PLUS				152.0f
#define BOARD_TIME_Y_PLUS				185.0f

#define BOARD_CAM_X_PLUS				8.0f
#define BOARD_CAM_Y_PLUS				165.0f

class CHub : public CGameObject
{
	CCamera* camera;
	CListArrow* arrows;
	CLives* lives;
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
	void SetLives(CLives* l);
};