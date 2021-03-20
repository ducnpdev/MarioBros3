#pragma once
#include "GameObject.h"
#include "Number.h"

#define TIME_MAX 300;

#define TIME_X_PLUS_8			8
#define TIME_X_PLUS_16			16
#define TIME_COUNT				1000


class CTime : public CGameObject
{
	vector<CNumber*> numbers;
	int time;
	DWORD time_start_game_hub;

public:
	CTime();
	CTime(vector<CNumber*> num) {
		time = TIME_MAX;
		time_start_game_hub = (DWORD)GetTickCount64();
		for (unsigned int i = 0; i < num.size(); i++)
			numbers.push_back(num[i]);
	}
	void PushNumbers(CNumber* num);
	void SetPositionNumbers(float x, float y);

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {};
};

