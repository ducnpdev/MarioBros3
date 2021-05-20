#include "ListScore.h"

void CListScore::SetState(int state)
{
	CGameObject::SetState(state);
}

void CListScore::SetScoresPlay(CScore* type)
{
	itemScore = type; 
}

void CListScore::DisplayScores(int typeNumber, float posX, float posY, DWORD time)
{
	SetState(typeNumber);
	SetPosition(posX, posY);
	display_start = time;
	vy = -SCORE_SPEED_Y;

	if (state == SCORE_STATE_100) AddScorePlay(SCORE_MINIMUN);
	else if (state == SCORE_STATE_200) AddScorePlay(SCORE_200);
	else if (state == SCORE_STATE_400) AddScorePlay(SCORE_400);
	else if (state == SCORE_STATE_800) AddScorePlay(SCORE_800);
	else if (state == SCORE_STATE_1000) AddScorePlay(SCORE_1000);
	else if (state == SCORE_STATE_2000) AddScorePlay(SCORE_2000);
	else if (state == SCORE_STATE_4000) AddScorePlay(SCORE_4000);
	else if (state == SCORE_STATE_8000) AddScorePlay(SCORE_8000);

}

void CListScore::AddScorePlay(int typeNumber)
{ 
	itemScore->SetScore(typeNumber);
}

void CListScore::Render()
{
	int ani = SCORE_ANI_100;
	if (state == SCORE_STATE_HIDEN) return;
	else if (state == SCORE_STATE_200) ani = SCORE_ANI_200;
	else if (state == SCORE_STATE_400) ani = SCORE_ANI_400;
	else if (state == SCORE_STATE_800) ani = SCORE_ANI_800;
	else if (state == SCORE_STATE_1000) ani = SCORE_ANI_1000;
	else if (state == SCORE_STATE_2000) ani = SCORE_ANI_2000;
	else if (state == SCORE_STATE_4000) ani = SCORE_ANI_4000;
	else if (state == SCORE_STATE_8000) ani = SCORE_ANI_8000;
	else if (state == SCORE_STATE_LEVELUP) ani = SCORE_ANI_LEVELUP;

	animation_set->at(ani)->Render(x, y);
}

void CListScore::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == SCORE_STATE_HIDEN) return;
	
	if (GetTickCount64() - display_start < SCORE_DISPLAY_TIME)
	{
		y += vy * dt;
	}
	else SetState(SCORE_STATE_HIDEN);

}

void CListScore::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}
