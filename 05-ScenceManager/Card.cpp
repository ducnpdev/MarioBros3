#include "Card.h"
#include "Utils.h"

void CCard::SetState(int state)
{
	CGameObject::SetState(state);
}

void CCard::Render()
{
	int ani = CARD_ANI_EMPTY;
	if (state == CARD_STATE_HIDEN) return;
	if (state == CARD_STATE_EMPTY) ani = CARD_ANI_EMPTY;
	else if (state == CARD_STATE_STAR) ani = CARD_ANI_STAR;
	else if (state == CARD_STATE_FLOWER) ani = CARD_ANI_FLOWER;
	else if (state == CARD_STATE_MUSHROOM) ani = CARD_ANI_MUSHROOM;
	animation_set->at(ani)->Render(x, y);
}

void CCard::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
}

void CCard::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}
