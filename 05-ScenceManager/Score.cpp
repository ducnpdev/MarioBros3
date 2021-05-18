#include "Score.h"

CScore::CScore()
{
}

CScore::CScore(vector<CNumber*> num)
{
	score = 0;
	for (unsigned int i = 0; i < num.size(); i++)
	{
		numbers.push_back(num[i]);
	}
}

void CScore::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGame* game = CGame::GetInstance();
	int num = game->GetScore();
	int temp;
	for (int i = numbers.size() - 1; i >= 0; i--)
	{
		temp = num % 10;
		num = num / 10;
		numbers[i]->SetState(temp);
	}
}

void CScore::PushNumbers(CNumber* num)
{
	numbers.push_back(num);
}

void CScore::SetPositionNumbers(float x, float y)
{
	numbers[0]->SetPosition(x, y);
	numbers[1]->SetPosition(x + SCORE_X_PlUS_8, y);
	numbers[2]->SetPosition(x + SCORE_X_PlUS_16, y);
	numbers[3]->SetPosition(x + SCORE_X_PlUS_24, y);
	numbers[4]->SetPosition(x + SCORE_X_PlUS_32, y);
	numbers[5]->SetPosition(x + SCORE_X_PlUS_40, y);
	numbers[6]->SetPosition(x + SCORE_X_PlUS_48, y);
}

void CScore::Render()
{
}
void CScore::SetScore(int s)
{
	CGame* game = CGame::GetInstance();
	game->SetScore(game->GetScore() + s);
}
void CScore::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}
