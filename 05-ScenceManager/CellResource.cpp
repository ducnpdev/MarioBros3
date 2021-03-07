#include "CellResource.h"

void CCellResource::PushObjectToCellResource(CGameObject* resource) {
	listResourceInCell.push_back(resource);
}

void CCellResource::RemoveResource() {
	listResourceInCell.clear();
}

vector<LPGAMEOBJECT> CCellResource::GetListResourceInCell() {
	return listResourceInCell;
}

