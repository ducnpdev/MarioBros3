#include "BackgroundWorldmap.h"
#include "Game.h"
#include "Sprites.h"
#include "Utils.h"

void CBackgroundWorldmap::Render() {

	animation_set->at(0)->Render(x, y);
	if (x == DARK_BACKGROUND_POSITION_X && y == DARK_BACKGROUND_POSITION_Y) {
		for (int i = 0; i < BACKGROUND_OBJECTS_HEIGHT_AMOUNT; i++) {
			for (int j = 0; j < BACKGROUND_OBJECTS_WIDTH_AMOUNT; j++) {
				if (x + BACKGROUND_OBJECT_SIZE * j == DARK_BACKGROUND_POSITION_X) animation_set->at(1)->Render(x + BACKGROUND_OBJECT_SIZE * j, y - BACKGROUND_OBJECT_SIZE * i);
				else animation_set->at(0)->Render(x + BACKGROUND_OBJECT_SIZE * j, y - BACKGROUND_OBJECT_SIZE * i);
			}
		}
	}
}

void CBackgroundWorldmap::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = 0;
	b = 0;
}