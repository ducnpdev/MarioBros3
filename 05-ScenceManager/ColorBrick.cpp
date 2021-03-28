#include "ColorBrick.h"
#include "Utils.h"

void CColorBrick::Render() {
	animation_set->at(0)->Render(x, y);
	// RenderBoundingBox();
}

void CColorBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + COLOR_BRICK_BBOX_WIDTH;
	b = y + COLOR_BRICK_BBOX_HEIGHT;
}

void CColorBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// DebugOut(L"class color brick \n");
	CGameObject::Update(dt, coObjects);
}
