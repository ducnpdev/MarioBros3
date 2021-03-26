#include "Pipe.h"
#include "Utils.h"

void CPipe::Render() {
	//DebugOut(L"class pipe \n");
	animation_set->at(0)->Render(x, y);
}

void CPipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + PIPE_BBOX_WIDTH;
	b = y + PIPE_BBOX_HEIGHT;
}