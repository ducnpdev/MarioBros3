#include "CardText.h"
#include "Utils.h"

CCardText::CCardText(CCard* c)
{
	card = c;
	card->SetState(CARD_STATE_HIDEN);
}

void CCardText::SetState(int state)
{
	CGameObject::SetState(state);
}

void CCardText::Render()
{
	// DebugOut(L"card text render \n");
	// int i = 0;
	// if (i == 0) return;
	int ani = CARDTEXT_STATE_HIDEN;
	if (state == CARDTEXT_STATE_HIDEN) return;
	animation_set->at(ani)->Render(x, y);
}
