#include "GridResource.h"
#include "PlayScence.h"

CGridResource::CGridResource(LPCWSTR path) {
	numRow = 0;
	numCol = 0;
	cellWidth = 0;
	cellHeight = 0;
	// GridLoadResource must place end func
	GridLoadResource(path);
}

void CGridResource::_ParseSection_Grid_INITIAL(string line) {
	vector<string> tokens = split(line);

	if (tokens.size() < 4) return; // skip invalid lines

	cellWidth = atoi(tokens[0].c_str());
	cellHeight = atoi(tokens[1].c_str());
	numCol = atoi(tokens[2].c_str());
	numRow = atoi(tokens[3].c_str());

	cellResource = new LPCCellResource[numCol];
	for (int i = 0; i < numCol; i++)
	{
		cellResource[i] = new CCellResource[numRow];
	}
}

void CGridResource::_ParseSection_Grid_ITEMS(string line) {
	vector<string> tokens = split(line);

	 CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	 if (tokens.size() < 6) return; // skip invalid lines

	 int x = atoi(tokens[1].c_str());
	 int y = atoi(tokens[2].c_str());

	 int cellX = atoi(tokens[4].c_str());
	 int cellY = atoi(tokens[5].c_str());

	 int type = atoi(tokens[0].c_str());

	 int ani_set_id = atoi(tokens[3].c_str());

	 CGameObject* obj = NULL;
	 switch (type)
	 {
	 	case 2:
	 	{
	 		int type = atoi(tokens[6].c_str());
	 		obj = new CRoad(type);
	 	}
	 	break;
	 }

	 LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	 if (obj != NULL)
	 {
	 	int add = 0;
	 	obj->SetPosition((float)x, (float)y);
	 	obj->SetAnimationSet(ani_set);
	 	obj->SetOriginObject((float)x, (float)y, obj->GetState());
	 	cellResource[cellX][cellY].PushObjectToCellResource(obj);
	 }
}

void CGridResource::_ParseSection_Grid_ENEMIES(string line) {
	vector<string> tokens = split(line);
	if (tokens.size() < 7) return;
	int type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());
	int ani_set_id = atoi(tokens[3].c_str());
	int XCell = atoi(tokens[4].c_str());
	int YCell = atoi(tokens[5].c_str());
	int state = atoi(tokens[6].c_str());
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	CGameObject* obj = NULL;
	switch (type)
	{
	case OBJECT_TYPE_GOOMBA: {
		obj = new CGoomba(state);
		break;
	}
	case OBJECT_TYPE_KOOPAS: {
		obj = new CKoopas(); 
		break; 
	}
	default:
		return;
	}
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	if (obj == NULL) return;
	
	int add = 0;
	obj->SetPosition(x, y);
	obj->SetAnimationSet(ani_set);
	cellResource[XCell][YCell].PushObjectToCellResource(obj);
}

void CGridResource::GridLoadResource(LPCWSTR path) {

	ifstream fStream;
	fStream.open(path);
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (fStream.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[OBJECTITEM]") { section = GRID_RESOURCE_OBJECT_ITEM; continue; }

		if (line == "[GRIDENEMIES]") { section = GRID_RESOURCE_ENEMIES; continue; }
		
		if (line == "[INITIAL]") { section = GRID_RESOURCE_INITIAL; continue; }
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case GRID_RESOURCE_OBJECT_ITEM: _ParseSection_Grid_ITEMS(line); break;
		case GRID_RESOURCE_ENEMIES: _ParseSection_Grid_ENEMIES(line); break;
		case GRID_RESOURCE_INITIAL: _ParseSection_Grid_INITIAL(line); break;
		}
	}

	fStream.close();

}

// unload all resource in cell, cell in grid
void CGridResource::GirdRemoveResource(){
	// DebugOut(L"CGrid GirdRemoveResource;\n");
}

// load all object in game need to render
void CGridResource::GirdPushResource(vector<LPGAMEOBJECT>& listResource, int xcam, int ycam) {
	int left, top, right, bottom;
	int i, j;
	//int k;

	left = ((xcam) / cellWidth);
	right = (xcam + GRID_CELL_WIDTH) / cellWidth;
	if (((xcam + GRID_CELL_WIDTH) % cellWidth) != 0)
		right++;
	top = (ycam) / cellHeight;
	bottom = (ycam + GRID_CELL_HEIGHT) / cellHeight;

	for (i = left; i <= right; i++)
	{
		for (j = top; j <= bottom; j++)
		{
			if (!cellResource[i][j].GetListResourceInCell().empty())
			{
				for (unsigned int k = 0; k < cellResource[i][j].GetListResourceInCell().size(); k++)
				{
				//	DebugOut(L"size %d \n", cellResource[i][j].GetListResourceInCell().size());
					if (!cellResource[i][j].GetListResourceInCell().at(k)->visible)
					{
						float tempOriginX, tempOriginY;
						cellResource[i][j].GetListResourceInCell().at(k)->GetOriginObject(tempOriginX, tempOriginY);
						if (!((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->ObjectInUsing(tempOriginX, tempOriginY) && cellResource[i][j].GetListResourceInCell().at(k)->GetState() > 10)
							cellResource[i][j].GetListResourceInCell().at(k)->ResetOriginObject();

						listResource.push_back(cellResource[i][j].GetListResourceInCell().at(k));
						cellResource[i][j].GetListResourceInCell().at(k)->SetVisible(true);
					}
				}
			}
		}

	}
}