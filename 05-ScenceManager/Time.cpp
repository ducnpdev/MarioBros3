#include "Time.h"

CTime::CTime()
{
	time = TIME_MAX;
	time_start_game_hub = (DWORD)GetTickCount64();
}

void CTime::PushNumbers(CNumber* numb)
{
	numbers.push_back(numb);
}

void CTime::SetPositionNumbers(float x, float y)
{
	numbers[0]->SetPosition(x, y);
	numbers[1]->SetPosition(x + TIME_X_PLUS_8, y);
	numbers[2]->SetPosition(x + TIME_X_PLUS_16, y);
}

void CTime::Render()
{
	int num = time;
	int temp;

	for (int i = numbers.size() - 1; i >= 0; i--)
	{
		temp = num % 10;
		//DebugOut(L"temp: %d\n", temp);
		num = num / 10;
		numbers[i]->SetState(temp);
	}
}

void CTime::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// DebugOut(L"class time \n");
	if (GetTickCount64() - time_start_game_hub > TIME_COUNT)
	{
		time -= 1;
		time_start_game_hub = (DWORD)GetTickCount64();
	}
}
