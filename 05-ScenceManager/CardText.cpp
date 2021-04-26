#include "CardText.h"

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
	int ani = CARDTEXT_STATE_HIDEN;
	if (state == CARDTEXT_STATE_HIDEN) return;
	animation_set->at(ani)->Render(x, y);
}
