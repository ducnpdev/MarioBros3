#pragma once
#include"GameObject.h"
#include "Utils.h"

#define ARROW_STATE_WHITE		0
#define ARROW_STATE_BLACK		1
#define ARROW_STATE_WHITE_P		2
#define ARROW_STATE_BLACK_P		3

#define ARROW_ANI_WHITE			0
#define ARROW_ANI_BLACK			1
#define ARROW_ANI_WHITE_P		2
#define ARROW_ANI_BLACK_P		3

#define ARROW_POS_X_PLUS_8		8
#define ARROW_POS_X_PLUS_16		16
#define ARROW_POS_X_PLUS_24		24
#define ARROW_POS_X_PLUS_32		32
#define ARROW_POS_X_PLUS_40		40
#define ARROW_POS_X_PLUS_48		48

#define ARROW_RESET_P_TIME		2500

class CArrow : public CGameObject
{
	int isP;
	int isBlack;
public:
	CArrow(int p) { isP = p; isBlack = 1; };

	void SetWhiteState() { isBlack = 0; }
	void SetBlackState() { isBlack = 1; }
	
	// return arrow is the arrow black
	int GetArrowIsBlack() { return isBlack; }
	int GetIsP() { return isP; }
	
	void SetState(int state);

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {};
};

class CListArrow : public CGameObject
{
	// list arrow have a length 6
	vector<CArrow*> listArrow;
	DWORD timePStart;

public:

	CListArrow(vector<CArrow*> arr) {
		timePStart = 0;
		for (unsigned int i = 0; i < arr.size(); i++)
		{
			listArrow.push_back(arr[i]);
		}

	}


	//void PushArrows(CArrow* num);
	void SetPositionListArrow(float x, float y);
	void SetWhiteListArrow();
	void SetBlackListArrow();
	void SetPStart(DWORD t) { timePStart = t; }

	void ResetListArrow();

	int GetPState();

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};