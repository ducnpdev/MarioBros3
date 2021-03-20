#include "Coin.h"

void CCoinPlay::SetPositionNumbers(float x, float y)
{
	numbers[0]->SetPosition(x, y);
	numbers[1]->SetPosition(x + COINPLAY_X_PLUS, y);
}

void CCoinPlay::AddCoinHub()
{
	CGame* game = CGame::GetInstance();
	game->SetCoinGame(game->GetCoinGame() + 1);
}

void CCoinPlay::Render()
{
}

void CCoinPlay::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGame* game = CGame::GetInstance();
	int num = game->GetCoinGame();
	int temp;

	for (int i = numbers.size() - 1; i >= 0; i--)
	{
		temp = num % 10;
		num = num / 10;
		numbers[i]->SetState(temp);
	}
}
