#pragma once
#include "GameObject.h"
#include "Number.h"
#include "All.h"

#define LIVE_X_PLUS				8

class CLives : public CGameObject
{
	vector<CNumber*> numbers;
	int lives;

public:
	CLives() { lives = 4; }
	CLives(vector<CNumber*> num) {
		lives = MARIO_LIVE_ALL;
		for (int i = 0; i < (int)num.size(); i++)
		{
			numbers.push_back(num[i]);
		}

	}
	void SetPositionNumbers(float x, float y);
	void AddLives() { lives += 1; }
	void MinusLives() { lives -= 1; }

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {};
};
