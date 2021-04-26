#include "Lives.h"

void CLives::SetPositionNumbers(float x, float y)
{
	numbers[0]->SetPosition(x, y);
	numbers[1]->SetPosition(x + LIVE_X_PLUS, y);
}

void CLives::Render()
{
}


void CLives::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	int num = lives;
	int temp;
	for (int i = numbers.size() - 1; i >= 0; i--)
	{
		temp = num % 10;
		num = num / 10;
		numbers[i]->SetState(temp);
	}
}