#pragma once
#include "GameObject.h"
#include "Number.h"
#include "Game.h"

#define COINPLAY_X_PLUS			8

class CCoinPlay : public CGameObject
{
	vector<CNumber*> numbers;
	int coin;

public:
	CCoinPlay(vector<CNumber*> num) {
		CGame* game = CGame::GetInstance();
		coin = game->GetCoinGame();
		for (int i = 0; i < (int)num.size(); i++)
		{
			numbers.push_back(num[i]);
		}
	}
	void SetPositionNumbers(float x, float y);
	void AddCoinHub();

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {};
};

