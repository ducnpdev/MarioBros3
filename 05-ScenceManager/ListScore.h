#pragma once
#include "GameObject.h"
#include "Score.h"

#define SCORE_STATE_100				0
#define SCORE_STATE_200				1
#define SCORE_STATE_400				2
#define SCORE_STATE_800				3
#define SCORE_STATE_1000			4
#define SCORE_STATE_2000			5
#define SCORE_STATE_4000			6
#define SCORE_STATE_8000			7
#define SCORE_STATE_LEVELUP			8

#define SCORE_ANI_100				0
#define SCORE_ANI_200				1
#define SCORE_ANI_400				2
#define SCORE_ANI_800				3
#define SCORE_ANI_1000				4
#define SCORE_ANI_2000				5
#define SCORE_ANI_4000				6
#define SCORE_ANI_8000				7
#define SCORE_ANI_LEVELUP			8

#define SCORE_DISPLAY_TIME			500
#define SCORE_MINIMUN				100

#define SCORE_200					200
#define SCORE_400					400
#define SCORE_800					800
#define SCORE_1000					1000
#define SCORE_2000					2000
#define SCORE_4000					4000
#define SCORE_8000					8000
#define SCORE_STATE_HIDEN			-1
#define SCORE_SPEED_Y				0.05f

class CListScore : public CGameObject
{
	int score;
	CScore* itemScore;
//	int display_state;
	DWORD display_start;
public:
	CListScore() { SetState(SCORE_STATE_HIDEN); }

	void SetState(int state);
	void SetScoresPlay(CScore* s);
	void DisplayScores(int score, float x, float y, DWORD t);
	void AddScorePlay(int s);

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};